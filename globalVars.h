#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QFile>
#include <QFileDialog>
#include <QtWidgets>

class Child{
public:
    QString Id;
    QString Type;
    QString Value;

    Child(){ };

    Child(QString id, QString type, QString value){
        Id = id;
        Type = type;
        Value = value;
    }
};

class Progression{
public:
    QMap<QString, QMap<QString, QString>> ProgAttrs;
    QMap<QString, QList<Child>> Children;
};

class CharacterClass{
public:
    QMap<QString, QMap<QString, QString>> ClassAttrs;
    QMap<int, Progression> ClassProgressions;
    QMap<QString, QList<Child>> Children;
};

QMap<QString, CharacterClass> existingClasses;
CharacterClass* currentClass;
QString classDescriptionsFileName;
QString progressionsFileName;

QString FindAttrInLine(QString attr, QString *line){
    int AttrStartIndex = line->indexOf(attr + "=\"") + attr.length() + 2;
    int AttrEndIndex = line->indexOf("\"", AttrStartIndex);
    QString attrValue = line->mid(AttrStartIndex, AttrEndIndex - AttrStartIndex);
    return attrValue;
}

QMap<QString, QString> FindAttrsInLine(QString* id, QString* line){
    int lineContentStartIndex = line->indexOf("<attribute ") + 11;
    int lineContentEndIndex = line->indexOf("/>");
    //if (lineContentStartIndex >= lineContentEndIndex) { return "-1"; } //Wrong line syntax
    QString lineContent = line->mid(lineContentStartIndex, lineContentEndIndex - lineContentStartIndex);
    QList<QString> lineContents = lineContent.split("\"");
    QMap<QString, QString> map;

    for (int i = 0; i < lineContents.count() - 1; i++){ //.count()-1 because last one is an empty string
        if (i % 2 == 0) {
            lineContents[i].replace("=", "").replace(" ", "");

            if (lineContents[i] == "id"){
                *id = lineContents[i + 1];
            }
            else {
                map[lineContents[i]] = lineContents[i+1];
            }
        }
    }
    return map;
}

void PrintValuesToWidgetsInFrame(QFrame *frame, int level = 1)
{
    QMap<QString, QMap<QString, QString>> *attributes;
    if (frame->objectName() == "frame_ClassDescription") {
        attributes = &currentClass->ClassAttrs;
    }
    else if (frame->objectName() == "frame_LevelProgression") {
        attributes = &currentClass->ClassProgressions[level].ProgAttrs;
    }
    else { return; }

    for (auto widget : frame->children()) {
        //https://forum.qt.io/topic/117141/check-if-widget-is-pushbutton/6
        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget)) {
            QString attrName = lineEdit->objectName().replace("lineEdit_", "");
            lineEdit->setText((*attributes).value(attrName).value("value"));
        }
        else if (QCheckBox* chbx = qobject_cast<QCheckBox*>(widget)) {
            QString attrName = chbx->objectName().replace("chbx_", "");
            if ((*attributes).value(attrName).value("value") == "true"){
                chbx->setCheckState(Qt::Checked);
            }
            else { chbx->setCheckState(Qt::Unchecked); }
        }
        else if (QTableWidget* tw = qobject_cast<QTableWidget*>(widget)) {
            tw->setRowCount(0);
            tw->setColumnCount(1);
            if (attributes->value("Boosts").value("value").isEmpty()) { return; }
            QStringList boostsList = attributes->value("Boosts").value("value").split(";");

            for (auto boost : boostsList){
                tw->insertRow(tw->rowCount());
                QAbstractItemModel* model = tw->model();
                QModelIndex idx = model->index(tw->rowCount() - 1, 0);
                model->setData(idx, boost);
            }
        }
    }


}

void ReadFromFile_Attributes(QFile *file)
{
    QTextStream stream(file);

    CharacterClass currentClassInFile;
    Progression currentProgressionInFile;
    QString line;
    QString text;
    int lineChildrenScopeLevel = 0; //0 means the line is not inside <children> scope
    QString currentRegion;
    QString currentNode;
    Child currentChild;
    while(!(line = stream.readLine()).isNull()){
        if (line.contains("<node ")){
            currentNode = FindAttrInLine("id", &line);
            if (currentNode == "ClassDescription") {
                currentClassInFile = CharacterClass();
                currentRegion = "ClassDescription";
                lineChildrenScopeLevel = 0;
            }
            else if (currentNode == "Progression") {
                currentProgressionInFile = Progression();
                currentRegion = "Progression";
                lineChildrenScopeLevel = 0;
            }
        }
        else if (line.contains("<attribute ")){
            QString id;
            QMap<QString, QString> map = FindAttrsInLine(&id, &line);

            if (currentNode == "ClassDescription") {
                currentClassInFile.ClassAttrs[id] = map;
            }
            else if (currentNode == "Progression") {
                currentProgressionInFile.ProgAttrs[id] = map;
            }
            else if (lineChildrenScopeLevel > 0) {
                currentChild = Child(id, map.value("type"), map.value("value"));
            }
        }
        else if (line.contains("<children>")){
            lineChildrenScopeLevel++;
        }
        else if (line.contains("</children>")){
            lineChildrenScopeLevel--;
        }
        else if (line.contains("</node>"))   {
            if (currentRegion == "ClassDescription"){
                if (lineChildrenScopeLevel == 0) {
                    QString className = currentClassInFile.ClassAttrs["Name"]["value"];
                    existingClasses[className] = currentClassInFile;
                }
                else {
                    currentClassInFile.Children[currentNode].append(currentChild);
                }
            }
            else if (currentRegion == "Progression"){
                if (lineChildrenScopeLevel == 0) {
                    QString className = currentProgressionInFile.ProgAttrs["Name"]["value"];
                    int progressionLevel = currentProgressionInFile.ProgAttrs["Level"]["value"].toInt();
                    existingClasses[className].ClassProgressions[progressionLevel] = currentProgressionInFile;
                }
                else {
                    currentProgressionInFile.Children[currentNode].append(currentChild);
                }
            }
        }
    }
}

void PrintToFile_Attributes(QFile *file, QString region, int verMajor, int verMinor, int verRev, int verBuild){
    QTextStream stream(file);
    stream  << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\n"
           << "<save>" << "\n"
           << "    <version major=\"" + QString::number(verMajor) +
                  "\" minor=\"" + QString::number(verMinor) +
                  "\" revision=\"" + QString::number(verRev) +
                  "\" build=\"" + QString::number(verBuild) + "\"/>" << "\n"
           << "    <region id=\"" + region +"\">" << "\n"
           << "        <node id=\"root\">" << "\n"
           << "            <children>" << "\n";

    QString regionNoS = region;
    regionNoS.removeLast();
    for(auto charClass : existingClasses.asKeyValueRange()){
        if (region == "ClassDescriptions" && charClass.second.ClassAttrs.count() == 0) { continue; }
        if (region == "Progressions" && charClass.second.ClassProgressions.count() == 0) { continue; }
        stream << "                <node id=\"" + regionNoS + "\">" << "\n";
        if (region == "ClassDescriptions"){
            for (auto classAttr : charClass.second.ClassAttrs.asKeyValueRange()){
                stream << "                    <attribute id=\"" + classAttr.first + "\" ";
                for (auto linePart : classAttr.second.asKeyValueRange()){
                    stream << linePart.first + "=\"" + linePart.second + "\" ";
                }
                stream << "/>\n";
            }
            if (charClass.second.Children.count() > 0){
                stream << "                    <children>" << "\n";
            }
            for (auto childrenList : charClass.second.Children.asKeyValueRange()){
                for (auto child : childrenList.second){
                    stream  << "                        <node id=\"" + childrenList.first + "\">" << "\n";
                    stream  << "                            <attribute id=\"" + child.Id + "\" "
                                  + "type=\"" + child.Type + "\" "
                                  + "value=\"" + child.Value + "\"/>" << "\n";
                    stream  << "                        </node>" << "\n";
                }
            }
            if (charClass.second.Children.count() > 0){
                stream << "                    </children>" << "\n";
            }
            stream << "                </node>" << "\n";
        }
        else if (region == "Progressions"){
            for (auto prog : charClass.second.ClassProgressions.asKeyValueRange()){
                stream << "                <node id=\"" + regionNoS + "\">" << "\n";
                for (auto progAttr : prog.second.ProgAttrs.asKeyValueRange()){
                    stream << "                    <attribute id=\"" + progAttr.first + "\" ";
                    for (auto linePart : progAttr.second.asKeyValueRange()){
                        stream << linePart.first + "=\"" + linePart.second + "\" ";
                    }
                    stream << "/>\n";
                }
                stream << "                </node>" << "\n";
            }
        }
    }

    stream  << "            </children>" << "\n"
           << "        </node>" << "\n"
           << "    </region>" << "\n"
           << "</save>";

}

//A replacement class for QTableWidget with wheelEvent function that allows e.g. to invert scrolling direction with mousewheel
class MyTableClass : public QTableWidget//subclass standard QTableView
{
    //Scroll event detection: https://stackoverflow.com/questions/12261267/scroll-event-detection-in-qt
    //This class taken from: https://stackoverflow.com/questions/50794225/scrolling-qtablewidget-smoothly-with-the-mousewheel
protected:
    void wheelEvent(QWheelEvent* e)
    {
        QString result;
        int items = 0; //round away from zero
        //        if (e->angleDelta().y() > 0){
        //            items = (e->angleDelta().y() + (e->DefaultDeltasPerStep - 1)) / e->DefaultDeltasPerStep;
        //        }
        //        else if (e->angleDelta().y() < 0){
        //            items = (e->angleDelta().y() - (e->DefaultDeltasPerStep - 1)) / e->DefaultDeltasPerStep;
        //        }
        //        else if (e->angleDelta().x() > 0){
        //            items = (e->angleDelta().x() + (e->DefaultDeltasPerStep - 1)) / e->DefaultDeltasPerStep;
        //        }
        //        else if (e->angleDelta().x() < 0){
        //            items = (e->angleDelta().x() - (e->DefaultDeltasPerStep - 1)) / e->DefaultDeltasPerStep;
        //        }

        items = e->angleDelta().y() != 0 ?  (e->angleDelta().y() / e->DefaultDeltasPerStep) * 3 :
                    (e->angleDelta().x() / e->DefaultDeltasPerStep) * 5;

        qDebug() << QString("deltaX %1").arg(e->angleDelta().x());
        qDebug() << QString("deltaY %1").arg(e->angleDelta().y());
        qDebug() << QString("items %1").arg(items);
        if (e->angleDelta().y() != 0) {
            //this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-items); //Original
            qDebug() << QString("HorBar value %1").arg(this->horizontalScrollBar()->value());
            this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value()-items); //Inverted
            qDebug() << QString("HorBar value %1").arg(this->horizontalScrollBar()->value());
            if (items > 0) { result = "Mouse Wheel Event: UP"; }
            else { result = "Mouse Wheel Event: DOWN"; }
        }
        else if (e->angleDelta().x() != 0) {
            this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-items); //Inverted
            if (items > 0) { result = "Mouse Wheel Event: LEFT"; }
            else { result = "Mouse Wheel Event: RIGHT"; }
        }
        qDebug() << result;
    }
};

#endif // GLOBALVARS_H
