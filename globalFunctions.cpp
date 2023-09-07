#include "globalFunctions.h"
#include "globalVars.h"
#include "lineeditintablewidget.h"
#include "tablecontextmenu.h"






void ReadFromFile_ValueLists(QFile *file)
{
    QTextStream stream(file);

    QString line;
    QList<QString>* currentValueList = nullptr;
    while(!(line = stream.readLine()).isNull()){
        if (line.contains("valuelist ") || line.contains("value ")){
            int valueStartIndex = line.indexOf("\"") + 1;
            int valueEndIndex = line.indexOf("\"", valueStartIndex);
            QString value = line.mid(valueStartIndex, valueEndIndex - valueStartIndex);
            if (line.contains("valuelist ")){
                currentValueList = &MenuSubitemsLists["List_" + value];
            }
            else if (line.contains("value ")){
                if (currentValueList != nullptr){
                    currentValueList->append(value);
                }
            }
        }
    }
}

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

//void CreateLineEditAsTableItem(QTableWidget* tw, int row, int col, QString text, MainWindow* mw){
//    QLineEdit* le = new QLineEdit(text);
//    le->setFrame(false);
//    QPoint pos = QCursor::pos();

//    auto ShowContextMenu = [tw, pos, mw]()
//    {
//        new TableContextMenu(tw, pos, mw);
//    };

//    connect(le, &QLineEdit::textChanged, []() {
//            ;
//        });

//    tw->setCellWidget(row, col, le);
//}

void UpdateWidgetTextFromTableWidget(QWidget* widget, QTableWidget* tableWidget, int progLevel){
    if (widget == nullptr) { return; }
    QString attrName = widget->objectName().split("_").last();
    QString string;
    for (int i = 0; i < tableWidget->rowCount(); i++){
        int rowCount = tableWidget->rowCount();
        //QString rowString = tableWidget->model()->index(i,0).data().toString();
        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(tableWidget->cellWidget(i, 0))){
            QString rowString = lineEdit->text();
            string = i ==  rowCount - 1 ? string + rowString :
                         string + rowString + ";" ;
        }
    }
    ((QLineEdit*)widget)->setText(string);
    currentClass->ClassProgressions[progLevel].ProgAttrs[attrName]["value"] = string;
}

void PrintValuesToTableWidget(QTableWidget* tw, QString attrValue, MainWindow* mw){
    tw->setRowCount(0);
    tw->setColumnCount(1);
    if (attrValue.isEmpty()) { return; }
    QStringList boostsList = attrValue.split(";");

    for (auto boost : boostsList){
        tw->insertRow(tw->rowCount());
        QAbstractItemModel* model = tw->model();
        QModelIndex idx = model->index(tw->rowCount() - 1, 0);

//        QLineEdit* le = new QLineEdit(boost);
//        le->setFrame(false);
//        tw->setCellWidget(idx.row(), idx.column(), le);
        new LineEditInTableWidget(tw, idx.row(), idx.column(), boost, mw);
        //CreateLineEditAsTableItem(tw, idx.row(), idx.column(), boost, mw);

        //model->setData(idx, boost);
    }
}

void PrintValuesToWidgetsInFrame(QFrame* frame, MainWindow* mw, int level)
{
    QMap<QString, QMap<QString, QString>>* attributes;
    if (currentClass == nullptr) { return; }
    if (frame->objectName() == "frame_ClassDescription") {
        attributes = &currentClass->ClassAttrs;
    }
    else if (frame->objectName() == "frame_LevelProgression") {
        attributes = &currentClass->ClassProgressions[level].ProgAttrs;
    }
    else { return; }
    if (attributes == nullptr) { return; }

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
            PrintValuesToTableWidget(tw, attributes->value("Boosts").value("value"), mw);
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
