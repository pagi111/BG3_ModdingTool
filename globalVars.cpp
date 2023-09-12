#include "globalVars.h"

//Scroll event detection: https://stackoverflow.com/questions/12261267/scroll-event-detection-in-qt
//This class taken from: https://stackoverflow.com/questions/50794225/scrolling-qtablewidget-smoothly-with-the-mousewheel
void MyTableClass::wheelEvent(QWheelEvent* e)
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
};

QMap<QString, CharacterClass> existingClasses;
CharacterClass* currentClass;
QString classDescriptionsFileName;
QString progressionsFileName;
QString historyFileName;
QString baseDirectoryPath = QCoreApplication::applicationDirPath();
//QListWidget* listWidget_AutoFill;

QList<QString> List_Interrupt {
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

QList<QString> List_ActionResource {
    "ActionPoint",
    "BonusActionPoint",
    "ReactionActionPoint",
    "SpellSlot",
    "Movement",
    "Rage",
    "BardicInspiration",
    "ChannelDivinity",
    "SuperiorityDie",
    "KiPoint",
    "SorceryPoint",
    "InspirationPoint",
    "WarlockSpellSlot",
    "EyeStalkActionPoint",
    "HitDice",
    "ExtraActionPoint",
    "ShortRestPoint",
    "ArcaneRecoveryPoint",
    "NaturalRecoveryPoint",
    "WildShape",
    "RitualPoint",
    "WeaponActionPoint",
    "TidesOfChaos",
    "LayOnHandsCharge",
    "ChannelOath",
    "Interrupt_HellishRebukeTiefling_Charge",
    "Interrupt_HellishRebukeWarlockMI_Charge",
    "SneakAttack_Charge",
    "ShadowSpellSlot",

    "AstralPlanePoint",
    "TadpolePowerPoint",
    "FungalInfestationCharge",
    "WarPriestActionPoint",
    "LuckPoint",
    "Interrupt_LuckOfTheFarRealms_Charge",
    "DeflectMissiles_Charge",
    "LegendaryResistanceCharge",
    "Interrupt_TAD_PsionicDominance_Charge",
    "Interrupt_EntropicWard_Charge",
    "Interrupt_IllusorySelf_Charge",
    "Interrupt_Indomitable",
    "Interrupt_DivineStrike",
    "Interrupt_Portent_1",
    "Interrupt_MistyEscape",
    "Interupt_CounterSpell_MindFlayer",
    "Interupt_Shield_MindFlayer",
};

QList<QString> List_ReplenishType {
    "Rest",
    "ShortRest",
    "Turn",
    "FullRest",
    "Never",
    "Combat",
};

//Lists with double __ in front of the name mean the lists that contain this word as an autofill hint, not a real set of values to choose from
QList<QString> List__Skill {
    "Skill",
};

QList<QString> List__SavingThrow {
    "SavingThrow",
};

QList<QString> List__Resistant {
    "Resistant",
};

/// This QMap contains lists by their names with possible subitems for the BoostTable ContextMenu.
/// Some of the lists are added in code, some others are imported from ValueLists.txt file
QMap<QString, QList<QString>> MenuSubitemsLists
{
    {"List_UnlockInterrupt",     List_Interrupt },
    {"List_ActionResource",      List_ActionResource },
    {"List_ReplenishType",       List_ReplenishType },
    {"List__Skill",              List__Skill },
    {"List__SavingThrow",        List__SavingThrow },
    {"List__Resistant",          List__Resistant },
};

/// This QMap matches boosts' names of the BoostTable ContextMenu with the right lists' names that are in MenuSubitemsLists.
QMap<QString, QString> BoostType_SubitemsList_Match
{
    {"Ability",             "Ability" },
    {"ActionResource",      "List_ActionResource" },
    {"ActionResourceReplenishTypeOverride",     "List_ReplenishType" },
    {"UnlockSpell",         "List_Interrupt" },
    {"Proficiency",         "Skill" },
    {"ProficiencyBonus",    "Skill" },
    {"ExpertiseBonus",      "Skill" },
    {"DamageReduction",     "Damage Type" },
    {"Resistance",          "Damage Type" },
    {"StatusImmunity",      "StatusGroupFlags" },
    {"UnlockInterrupt",     "List_Interrupt" },
    {"ArmorAbilityModifierCapOverride",     "ArmorType" },
    {"ACOverrideFormula",   "Ability" },
    {"Tag",                 "Ability" },
    {"Advantage",           "Ability" },
    {"RollBonus",           "Ability" },
    {"Reroll",              "Ability" },
};

///This QMap assigns to each boost name a QList containing possible variants of that boost
/// In each item of the QList there are possible arguments written inside ( )
/// Based on those arguments, autofill options are presented while the user is inserting the arguments (taken from the corresponding list in BoostType_SubitemsList_Match)
/// Note: Argument with a '_' in front (e.g. _Skill) means that this is the word that should be written as an argument, not the list of arguments to choose from
/// Each item in the QList also contains an example usage (inside [ ]). This shows as a tooltip in the context menu.
QMap<QString, QList<QString>> boostsHints {
  {"Ability",                               QList<QString>{ "(Ability, Increase) [(Strength, 1)]", "(Ability, Increase, MaxLevel) [(Strength, 1, 20)]" } },
  {"ActionResource",                        QList<QString>{ "(ActionResource, ResourceCountBonus, ResourceLevel) [(SpellSlot,1,3) or (Movement, -1.5, 0)] " } },
  {"ActionResourceOverride",                QList<QString>{ "(ActionResource, NewResourceCount(??), ResourceLevel) [(Interrupt_HellishRebukeWarlockMI_Charge,1,0)]" } },
  {"ActionResourceReplenishTypeOverride",   QList<QString>{ "(ActionResource, ReplenishType) [(BardicInspiration,ShortRest)]" } },
  {"UnlockSpell",                           QList<QString>{ "(Spell) [(Target_BardicInspiration)]", "(Spell, Singular(??)) [(Shout_MageArmor_ArmorOfShadows, Singular)]", "(Spell, Singular/AddChildre, Children, Replenish, Ability) [(Target_TrueStrike,,,,Wisdom) or (Target_Grease,Singular,None,UntilRest,Wisdom) or (Target_GaseousForm,AddChildren,d136c5d9-0ff0-43da-acce-a74a07f8d6bf,,Wisdom)]" } },
  {"Proficiency",                           QList<QString>{ "(ProficiencyGroupFlags) [(Daggers)]" } },
  {"ProficiencyBonus",                      QList<QString>{ "(_Skill, Skill) [(Skill, History)]", "(_SavingThrow, Ability) [(SavingThrow, Charisma)]" } },
  {"ExpertiseBonus",                        QList<QString>{ "(Skill) [(Arcana)]" } },
  {"Resistance",                            QList<QString>{ "(Damage Type, _Resistant) [(Poison, Resistant)]" } },
  {"StatusImmunity",                        QList<QString>{ "(StatusGroupFlags) [(SG_Blinded)]" } },
  {"UnlockInterrupt",                       QList<QString>{ "(Interrupt)" } },
  {"Tag",                                   QList<QString>{ "(Tag)" } },
  {"Advantage",                             QList<QString>{ "(int)" } },
  {"RollBonus",                             QList<QString>{ "(int)" } },
  {"Reroll",                                QList<QString>{ "(int)" } },

  {"Initiative",                            QList<QString>{ "(int) [(5)]" } },
  {"FallDamageMultiplier",                  QList<QString>{ "(float) [(0.5)]" } },
  {"JumpMaxDistanceMultiplier",             QList<QString>{ "(float) [(1.5)]" } },
  {"DamageReduction",                       QList<QString>{ "(DamageType,Multiplier,) [(Slashing, Half, )]", "(DamageType, Flat, int) [(All, Flat, 8)]" } },
  {"ArmorAbilityModifierCapOverride",       QList<QString>{ "(ArmorType, MaxBonus) [(HalfPlate, 3)" } },
  {"AC",                                    QList<QString>{ "(int) [(2)]" } },
  {"ACOverrideFormula",                     QList<QString>{ "(BaseArmorClass, ??, BonusACAbility) [(13,true,Dexterity)]", "(BaseArmorClass, ??, 1st BonusACAbility, 2nd BonusACAbility) [(10,true,Dexterity,Constitution)]" } },
  {"CharacterWeaponDamage",                 QList<QString>{ "(AdditionalDmg) [(2)]" } },
  {"EntityThrowDamage",                     QList<QString>{ "(AdditionalDmg) [(2)]" } },
  {"IncreaseMaxHP",                         QList<QString>{ "(PercentageHpIncrease) [(50%)]", "(SomeFormula) [(Level*2) - as in 'Tough' feat]" } },
  {"CannotBeDisarmed",                      QList<QString>{ "()" } },
  {"AddProficiencyToAC",                    QList<QString>{ "()" } },
  {"AddProficiencyToDamage",                QList<QString>{ "()" } },
  {"DarkvisionRangeMin",                    QList<QString>{ "(int) [(12) or (24)]" } },
  {"WeightCategory",                        QList<QString>{ "(CategoryIncrease) [(+1)]" } },
  {"Weight",                                QList<QString>{ "(NewWeight) [(100)]" } },
  {"IgnoreLowGroundPenalty",                QList<QString>{ "(RangedWeaponAttack)" } },
  {"IgnorePointBlankDisadvantage",          QList<QString>{ "(Loading)" } },
};

//Unnecessary now
QMap<QString, QList<QString>> boostsArguments {
  {"Ability",                               QList<QString>{ "(Ability, Increase) [(Strength, 1)]", "(Ability, Increase, MaxLevel) [(Strength, 1, 20)]" } },
  {"ActionResource",                        QList<QString>{ "(ActionResource, ResourceCountBonus, ResourceLevel) [(SpellSlot,1,3) or (Movement, -1.5, 0)] " } },
  {"ActionResourceOverride",                QList<QString>{ "(ActionResource, NewResourceCount(??), ResourceLevel) [(Interrupt_HellishRebukeWarlockMI_Charge,1,0)]" } },
  {"ActionResourceReplenishTypeOverride",   QList<QString>{ "(ActionResource, ReplenishType) [(BardicInspiration,ShortRest)]" } },
  {"UnlockSpell",                           QList<QString>{ "(Spell) [(Target_BardicInspiration)]", "(Spell, Singular(??)) [(Shout_MageArmor_ArmorOfShadows, Singular)]", "(Spell, Singular/AddChildre, Children, Replenish, Ability) [(Target_TrueStrike,,,,Wisdom) or (Target_Grease,Singular,None,UntilRest,Wisdom) or (Target_GaseousForm,AddChildren,d136c5d9-0ff0-43da-acce-a74a07f8d6bf,,Wisdom)]" } },
  {"Proficiency",                           QList<QString>{ "(Skill) [(Arcana)]" } },
  {"ProficiencyBonus",                      QList<QString>{ "(_Skill, Skill) [(Skill, History)]", "('SavingThrow', Ability) [(SavingThrow, Charisma)]" } },
  {"ExpertiseBonus",                        QList<QString>{ "(Skill) [(Arcana)]" } },
  {"Resistance",                            QList<QString>{ "(DmgType, 'Resistant') [(Poison, Resistant)]" } },
  {"StatusImmunity",                        QList<QString>{ "(Status) [(SG_Blinded)]" } },
  {"UnlockInterrupt",                       QList<QString>{ "(Interrupt_Name)" } },
  {"Tag",                                   QList<QString>{ "(Tag)" } },
  {"Advantage",                             QList<QString>{ "(int)" } },
  {"RollBonus",                             QList<QString>{ "(int)" } },
  {"Reroll",                                QList<QString>{ "(int)" } },

  {"Initiative",                            QList<QString>{ "(int) [(5)]" } },
  {"FallDamageMultiplier",                  QList<QString>{ "(float) [(0.5)]" } },
  {"JumpMaxDistanceMultiplier",             QList<QString>{ "(float) [(1.5)]" } },
  {"DamageReduction",                       QList<QString>{ "(DamageType,Multiplier,) [(Slashing, Half, )]", "(DamageType, Flat, int) [(All, Flat, 8)]" } },
  {"ArmorAbilityModifierCapOverride",       QList<QString>{ "(ArmorType, MaxBonus) [(HalfPlate, 3)" } },
  {"AC",                                    QList<QString>{ "(int) [(2)]" } },
  {"ACOverrideFormula",                     QList<QString>{ "(BaseArmorClass, ??, BonusACAbility) [(13,true,Dexterity)]", "(BaseArmorClass, ??, 1st BonusACAbility, 2nd BonusACAbility) [(10,true,Dexterity,Constitution)]" } },
  {"CharacterWeaponDamage",                 QList<QString>{ "(AdditionalDmg) [(2)]" } },
  {"EntityThrowDamage",                     QList<QString>{ "(AdditionalDmg) [(2)]" } },
  {"IncreaseMaxHP",                         QList<QString>{ "(PercentageHpIncrease) [(50%)]", "(SomeFormula) [(Level*2) - as in 'Tough' feat]" } },
  {"CannotBeDisarmed",                      QList<QString>{ "()" } },
  {"AddProficiencyToAC",                    QList<QString>{ "()" } },
  {"AddProficiencyToDamage",                QList<QString>{ "()" } },
  {"DarkvisionRangeMin",                    QList<QString>{ "(int) [(12) or (24)]" } },
  {"WeightCategory",                        QList<QString>{ "(CategoryIncrease) [(+1)]" } },
  {"Weight",                                QList<QString>{ "(NewWeight) [(100)]" } },
  {"IgnoreLowGroundPenalty",                QList<QString>{ "(RangedWeaponAttack)" } },
  {"IgnorePointBlankDisadvantage",          QList<QString>{ "(Loading)" } },
};
