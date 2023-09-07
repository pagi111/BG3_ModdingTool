#include "tablecontextmenu.h"
#include "globalVars.h"

QList<QString> boostsTypes {
    "Ability",
    "ActionResource",
    "ActionResourceReplenishTypeOverride",
    "UnlockSpell",
    "Proficiency",
    "ProficiencyBonus",
    "ExpertiseBonus",
    "DamageReduction",
    "Resistance",
    "StatusImmunity",
    "UnlockInterrupt",
    "ArmorAbilityModifierCapOverride",
    "Tag",
    "Advantage",
    "RollBonus",
    "Reroll",
    "Other",
};

QList<QString> boostsTypes_Other {
    "Initiative",
    "FallDamageMultiplier",
    "JumpMaxDistanceMultiplier",
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
    "Proficiency(skill)",
    "Advantage(Ability, Strength)",
    "Advantage(Skill, Athletics)",
    "Advantage(Concentration)",
    "Advantage(AttackRoll)",
    "Advantage(SavingThrow, Charisma)",                //valuelist "Skill"
    "RollBonus(RangedWeaponAttack, -5)",        //valuelist "Spell Attack Type" (maybe sth else too?)
    "RollBonus(SavingThrow,2,Dexterity)",
    "RollBonus(SkillCheck,1d6)",
    "RollBonus(RawAbility,1d6)",      //BardicInspiration_Ability
    "RollBonus(RangedWeaponAttack, 2)",
    "RollBonus(RangedOffHandWeaponAttack, 2)"  //FightingStyle_Archery
    "Reroll(Attack,1,true)",
    "Reroll(SkillCheck,1,true)",
    "Reroll(RawAbility,1,true)",
    "Reroll(SavingThrow,1,true)",
    "AC(1)"                                                     //FightingStyle_Defense
    "CharacterWeaponDamage(10)",
    "EntityThrowDamage(2)",
    "IgnoreLowGroundPenalty(RangedWeaponAttack)",
    "IncreaseMaxHP(Level*2)",
    "IncreaseMaxHP(50%)",
    "DamageReduction(All,Half,)",
    "DamageReduction(All, Flat, 8)",
    "CannotBeDisarmed()",
    "AddProficiencyToDamage()",                 //skill "CompanionsBond_Creature"
    "AddProficiencyToAC()",                     //skill "CompanionsBond_Creature"
    "DarkvisionRangeMin(12)",                   //also there: ActiveCharacterLight(051648e6-f05a-e41f-e398-ffd5cd148989)
    "DarkvisionRangeMin(24)",                   //also there: ActiveCharacterLight(c46e7ba8-e746-7020-5146-287474d7b9f7)
    "Resistance(Poison, Resistant)",            //valuelist "Damage Type" (I think)
    "StatusImmunity(SLEEP)",
    "StatusImmunity(SURPRISED)",
    "ACOverrideFormula(13,true,Dexterity)",     //DraconicResilience
    "ACOverrideFormula(10,true,Dexterity,Constitution)", //UnarmouredDefence_Barbarian
    "WeightCategory(+1)",                       //EnlargeWeightMedium
    "Weight(100)",                              //EnlargeWeightLarge
    "IgnorePointBlankDisadvantage(Loading)",
    "Initiative",
    "Ability(Strength, 1)",
    "Ability(Strength, 1, 20)", //Last param is max value (I think)

};


//Older Constructor
//, const QPoint &mousePos
TableContextMenu::TableContextMenu(QTableWidget* sender, MainWindow* mainWindow){
    QPoint pos = QCursor::pos();

    QAbstractItemModel* model = sender->model();
    //QModelIndex idx = sender->indexAt(mousePos);
    QModelIndex idx = sender->indexAt(pos);

    auto AddMenuAction = [model, idx, this](QString actionText, QMenu* submenu)
    {
        QAction* action = submenu->addAction(actionText);
        QString boostText = submenu->title() + "(" + actionText + ")";
        //action->setToolTip("ActionDupa"); //It's not possible without a lot of additional code...
        connect(action, &QAction::triggered, this, [model, idx, boostText, this]() {
            model->setData(idx,QString(boostText));
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
            for (QString subitem : MenuSubitemsLists[BoostType_SubitemsList_Match[boost]]){
                AddMenuAction(subitem, menu);
            }
        }

    }

    this->addSeparator();
    QAction *action_DeleteRow = this->addAction("Delete Row");
    connect(action_DeleteRow, &QAction::triggered, this, [sender, idx, mainWindow, this](){
        mainWindow->TableDeleteRow(sender, idx.row());
        delete this;
    });
    //connect(action_DeleteRow, &QAction::triggered, this, [tw, rowIndex, this](){ ui->tableWidget_Boosts->removeRow(rowIndex); });

    this->popup(pos);
    //this->popup(sender->viewport()->mapToGlobal(mousePos));
};


//New Constructor - with QLineEdits in Table
TableContextMenu::TableContextMenu(QTableWidget* tw, QLineEdit* le, MainWindow* mainWindow){
    QPoint pos = QCursor::pos();
    QModelIndex idx = tw->indexAt(tw->viewport()->mapFromGlobal(pos));

    auto AddMenuAction = [le, this](QString actionText, QMenu* submenu)
    {
        QAction* action = submenu->addAction(actionText);
        //QString boostText = submenu->title() + "(" + actionText + ")";
        QString boostText = submenu->title() + actionText.replace("_", "");
        connect(action, &QAction::triggered, this, [le, this, boostText]() {
            le->setText(boostText);
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
            for (QString variant : boostsHints[boost]){ //variant of a boost taken from the list corresponding to that boost
                AddMenuAction(variant.split("[").first().trimmed(), menu);    //the first item before '[' is the variant; after '[' there is an example use
            }
//            for (QString subitem : MenuSubitemsLists[BoostType_SubitemsList_Match[boost]]){
//                AddMenuAction(subitem, menu);
//            }
        }

    }

    this->addSeparator();
    QAction *action_DeleteRow = this->addAction("Delete Row");
    connect(action_DeleteRow, &QAction::triggered, this, [tw, idx, mainWindow, this](){
        mainWindow->TableDeleteRow(tw, idx.row());
        delete this;
    });

    this->popup(pos);
};
