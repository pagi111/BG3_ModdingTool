#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QFile>
#include <QFileDialog>
#include <QtWidgets>

#include "mainwindow.h"
#include <QToolTip>

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
QMap<QString, QList<QString>> valueLists;

QList<QString> boostsTypes {
    "Ability",
    "ActionResource",
    "ActionResourceReplenishTypeOverride",
    "UnlockSpell",
    "Proficiency",
    "ProficiencyBonus",
    "ExpertiseBonus",
    "Resistance",
    "StatusImmunity",
    "UnlockInterrupt",
    "Other",
};

QList<QString> boostsTypes_Other {
    "Initiative",
    "FallDamageMultiplier",
    "JumpMaxDistanceMultiplier",
    "DamageReduction",
    "ArmorAbilityModifierCapOverride",
    "Tag",
    "RollBonus",
    "Reroll",
    "Advantage",
    "AC",
    "ACOverrideFormula",
    "CharacterWeaponDamage",
    "EntityThrowDamage",
    "IncreaseMaxHP",
    "CannotBeDisarmed",
    "AddProficiencyToAC",
    "AddProficiencyToDamage",
    "DarkvisionRangeMin",
    "WeightCategory",
    "Weight",
    "IgnoreLowGroundPenalty",
    "IgnorePointBlankDisadvantage",
};



QMap<QString, QList<QString>> boostsHints {
    {"Ability",                               QList<QString>{ "(Ability, Increase) [(Strength, 1)]", "(Ability, Increase, MaxLevel) [(Strength, 1, 20)]" } },
    {"ActionResource",                        QList<QString>{ "(Resource, ResourceCountBonus, ResourceLevel (e.g. spell lvl)) [(SpellSlot,1,3) or (Movement, -1.5, 0)] " } },
    {"ActionResourceOverride",                QList<QString>{ "(Resource, NewResourceCount(??), ResourceLevel) [(Interrupt_HellishRebukeWarlockMI_Charge,1,0)]" } },
    {"ActionResourceReplenishTypeOverride",   QList<QString>{ "(Resource, ReplenishType) [(BardicInspiration,ShortRest)]" } },
    {"UnlockSpell",                           QList<QString>{ "(Spell) [(Target_BardicInspiration)]", "(Spell, Singular(??)) [(Shout_MageArmor_ArmorOfShadows, Singular)]", "(Spell, Singular/AddChildre, Children, Replenish, Ability) [(Target_TrueStrike,,,,Wisdom) or (Target_Grease,Singular,None,UntilRest,Wisdom) or (Target_GaseousForm,AddChildren,d136c5d9-0ff0-43da-acce-a74a07f8d6bf,,Wisdom)]" } },
    {"Proficiency",                           QList<QString>{ "(ArmorType, MaxBonus) [overrides e.g. max Dex bonus for these armour]" } },
    {"ProficiencyBonus",                      QList<QString>{ "(Tag)" } },
    {"ExpertiseBonus",                        QList<QString>{ "(int)" } },
    {"Resistance",                            QList<QString>{ "(int)" } },
    {"StatusImmunity",                        QList<QString>{ "(int)" } },
    {"UnlockInterrupt",                       QList<QString>{ "(Interrupt_Name)" } },

    {"Initiative",                            QList<QString>{ "(int) [(5)]" } },
    {"FallDamageMultiplier",                  QList<QString>{ "(float) [(0.5)]" } },
    {"JumpMaxDistanceMultiplier",             QList<QString>{ "(float) [(1.5)]" } },
    {"DamageReduction",                       QList<QString>{ "(DamageType,Multiplier,) [(Slashing, Half, )]", "(DamageType, Flat, int) [(All, Flat, 8)]" } },
    {"ArmorAbilityModifierCapOverride",       QList<QString>{ "(ArmorType, MaxBonus) [(HalfPlate, 3)" } },
    {"Tag",                                   QList<QString>{ "(Tag)" } },
    {"RollBonus",                             QList<QString>{ "(int)" } },
    {"Reroll",                                QList<QString>{ "(int)" } },
    {"Advantage",                             QList<QString>{ "(int)" } },
    {"AC",                                    QList<QString>{ "(int) [(2)]" } },
    {"ACOverrideFormula",                     QList<QString>{ "(BaseArmorClass, ??, BonusACAbility) [(13,true,Dexterity)]", "(BaseArmorClass, ??, 1st BonusACAbility, 2nd BonusACAbility) [(10,true,Dexterity,Constitution)]" } },
    {"CharacterWeaponDamage",                 QList<QString>{ "(AdditionalDmg) [(2)]" } },
    {"EntityThrowDamage",                     QList<QString>{ "(AdditionalDmg) [(2)]" } },
    {"IncreaseMaxHP",                         QList<QString>{ "(PercentageHpIncrease) [(50%)]", "(SomeFormula) [(Level*2) - as in 'Tough' feat]" } },
    {"CannotBeDisarmed",                      QList<QString>{ "()" } },
    {"AddProficiencyToAC",                    QList<QString>{ "()" } },
    {"AddProficiencyToDamage",                QList<QString>{ "()" } },
    {"DarkvisionRangeMin",                    QList<QString>{ "(int) [(12)]" } },
    {"WeightCategory",                        QList<QString>{ "(CategoryIncrease) [(+1)]" } },
    {"Weight",                                QList<QString>{ "(NewWeight) [(100)]" } },
    {"IgnoreLowGroundPenalty",                QList<QString>{ "(RangedWeaponAttack)" } },
    {"IgnorePointBlankDisadvantage",          QList<QString>{ "(Loading)" } },
};

QList<QString> interrupts {
    "Interrupt_DefensiveDuelist",
    "Interrupt_Sentinel",
    "Interrupt_ShieldMaster",
    "Interrupt_WarCaster",
    "Interrupt_Smite_Divine",
    "Interrupt_Smite_Divine_2",
    "Interrupt_Smite_Divine_3",
    "Interrupt_AttackOfOpportunity",
    "Interrupt_Overwhelm",
    "Interrupt_CuttingWords",
    "Interrupt_FightingStyle_Protection",
    "Interrupt_GiantKiller",
    "Interrupt_Riposte",
    "Interrupt_TidesOfChaos",
    "Interrupt_WardingFlare",
};

class TableContextMenu : public QMenu{
public:
    TableContextMenu(){};

    TableContextMenu(QTableWidget* sender, const QPoint &mousePos, MainWindow* mainWindow){
        QAbstractItemModel* model = sender->model();
        QModelIndex idx = sender->indexAt(mousePos);

        auto AddMenuAction = [model, idx, this](QString text, QMenu* submenu)
        {
            QAction* action = submenu->addAction(text);
            //action->setToolTip("ActionDupa"); //It's not possible without a lot of additional code...
            connect(action, &QAction::triggered, this, [model, idx, text, this]() {
                model->setData(idx, QString(text));
                delete this;
            });
        };


        auto AddTooltip = [](QString boostName, QString* tooltipText){
            if (boostsHints[boostName].count() > 1){
                tooltipText->append(boostName + ": \n");
                for (int i = 0; i < boostsHints[boostName].count(); i++){
                    QList<QString> hintWithExample = boostsHints[boostName][i].split("[");
                    tooltipText->append("    " + QString::number(i+1) + ") " + hintWithExample.first());
                    if (hintWithExample.count() > 1) {
                        tooltipText->append("        [" + boostName + hintWithExample.last());
                    }
                    tooltipText->append("\n");
                }
            }
            else {
                QList<QString> hintWithExample = boostsHints[boostName].first().split("[");
                tooltipText->append(boostName + hintWithExample.first());
                if (hintWithExample.count() > 1) {
                    tooltipText->append("        [" + boostName + hintWithExample.last());
                }
                tooltipText->append("\n");
            }
        };

        for (QString boost : boostsTypes){
            if (boost == "Other"){
                QMenu* menu = this->addMenu(boost);
                QString tooltipText;
                for (QString boost : boostsTypes_Other){
                    AddTooltip(boost, &tooltipText);
                }
                menu->setToolTip(tooltipText);
                for (QString otherBoost : boostsTypes_Other){
                    AddMenuAction(otherBoost, menu);
                }
            }
            else {
                QMenu* menu = this->addMenu(boost);
                QString tooltipText;
                AddTooltip(boost, &tooltipText);
                menu->setToolTip(tooltipText);
                AddMenuAction(boost, menu);
            }

        }

        this->addSeparator();
            QAction *action_DeleteRow = this->addAction("Delete Row");
            connect(action_DeleteRow, &QAction::triggered, this, [sender, idx, mainWindow, this](){
                mainWindow->TableDeleteRow(sender, idx.row());
                delete this;
            });
            //connect(action_DeleteRow, &QAction::triggered, this, [tw, rowIndex, this](){ ui->tableWidget_Boosts->removeRow(rowIndex); });
        this->popup(sender->viewport()->mapToGlobal(mousePos));
    };
};

//TableContextMenu* tableContextMenu = nullptr;

QString boostsTypes2[] {
    "Action Resource(resource, 1, 1)", // "ActionResource(SpellSlot,1,3)" means 1 add. slot level 3
    "ActionResourceOverride(Interrupt_HellishRebukeWarlockMI_Charge,1,0)",
    "ActionResourceReplenishTypeOverride(BardicInspiration,ShortRest)", //valuelist "CooldownType"
    "UnlockSpell(Shout_MageArmor_ArmorOfShadows,Singular)",
    "UnlockSpell(Target_BardicInspiration)",
    "UnlockSpell(Target_ProtectionFromEvilAndGood,Singular,None,UntilRest,Wisdom)",
    "UnlockSpell(Target_TrueStrike,,,,Wisdom)",
    "UnlockSpellVariant(SpellId('Target_MageHand'),ModifyTooltipDescription())",
    "FallDamageMultiplier(0.5)",
    "JumpMaxDistanceMultiplier(1.5)",
    "DamageReduction(Slashing, Flat, 3)", //valuelist "Damage Type",
    "ArmorAbilityModifierCapOverride(Hide, 3)", //valuelist "ArmorType"     //overrides e.g. max Dex bonus for these armour
    "ProficiencyBonus(SavingThrow,Charisma)",   //valuelist "Ability"
    "ProficiencyBonus(Skill,History)",
    "ExpertiseBonus(History)",

    "StatusImmunity(SG_Blinded)",               //valuelist "StatusGroupFlags"
    "Tag(BLINDSIGHT)",
    "Tag(FRIGHTENED_ADV)",
    "Proficiency(skill)",                       //valuelist "Skill"
    "RollBonus(RangedWeaponAttack, -5)",        //valuelist "Spell Attack Type" (maybe sth else too?)
    "RollBonus(SavingThrow,2,Dexterity)",
    "RollBonus(SkillCheck,1d6)",
    "RollBonus(RawAbility,1d6)",      //BardicInspiration_Ability
    "RollBonus(RangedWeaponAttack, 2)",
    "RollBonus(RangedOffHandWeaponAttack, 2)"  //FightingStyle_Archery
    "AC(1)"                                                     //FightingStyle_Defense
    "CharacterWeaponDamage(10)",
    "EntityThrowDamage(2)",
    "IgnoreLowGroundPenalty(RangedWeaponAttack)",
    "IncreaseMaxHP(Level*2)",
    "IncreaseMaxHP(50%)",
    "DamageReduction(All,Half,)",
    "DamageReduction(All, Flat, 8)",
    "Advantage(Ability, Strength)",
    "Advantage(Skill, Athletics)",
    "Advantage(Concentration)",
    "Advantage(AttackRoll)",
    "Advantage(SavingThrow, Charisma)",
    "CannotBeDisarmed()",
    "AddProficiencyToDamage()",                 //skill "CompanionsBond_Creature"
    "AddProficiencyToAC()",                     //skill "CompanionsBond_Creature"
    "DarkvisionRangeMin(12)",                   //also there: ActiveCharacterLight(051648e6-f05a-e41f-e398-ffd5cd148989)
    "DarkvisionRangeMin(24)",                   //also there: ActiveCharacterLight(c46e7ba8-e746-7020-5146-287474d7b9f7)
    "Resistance(Poison, Resistant)",            //valuelist "Damage Type" (I think)
    "StatusImmunity(SLEEP)",
    "StatusImmunity(SURPRISED)",
    "Reroll(Attack,1,true)",
    "Reroll(SkillCheck,1,true)",
    "Reroll(RawAbility,1,true)",
    "Reroll(SavingThrow,1,true)",
    "ACOverrideFormula(13,true,Dexterity)",     //DraconicResilience
    "ACOverrideFormula(10,true,Dexterity,Constitution)", //UnarmouredDefence_Barbarian
    "WeightCategory(+1)",                       //EnlargeWeightMedium
    "Weight(100)",                              //EnlargeWeightLarge
    "IgnorePointBlankDisadvantage(Loading)",
    "Initiative",
    "Ability(Strength, 1)",
    "Ability(Strength, 1, 20)", //Last param is max value (I think)

};

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
                currentValueList = &valueLists[value];
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


void PrintValuesToTableWidget(QTableWidget* tw, QString attrValue){
    tw->setRowCount(0);
    tw->setColumnCount(1);
    if (attrValue.isEmpty()) { return; }
    QStringList boostsList = attrValue.split(";");

    for (auto boost : boostsList){
        tw->insertRow(tw->rowCount());
        QAbstractItemModel* model = tw->model();
        QModelIndex idx = model->index(tw->rowCount() - 1, 0);
        model->setData(idx, boost);
    }
}

void PrintValuesToWidgetsInFrame(QFrame* frame, int level = 1)
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
            PrintValuesToTableWidget(tw, attributes->value("Boosts").value("value"));
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
