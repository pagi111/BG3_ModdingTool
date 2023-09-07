#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QFile>
#include <QFileDialog>
#include <QtWidgets>
#include <QToolTip>

//A replacement class for QTableWidget with wheelEvent function that allows e.g. to invert scrolling direction with mousewheel
class MyTableClass : public QTableWidget//subclass standard QTableView
{
protected:
    void wheelEvent(QWheelEvent* e);
};

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



extern QMap<QString, CharacterClass> existingClasses;
extern CharacterClass* currentClass;
extern QString classDescriptionsFileName;
extern QString progressionsFileName;

extern QList<QString> List_Interrupt;
extern QList<QString> List_ActionResource;
extern QList<QString> List_ReplenishType;

extern QMap<QString, QList<QString>> MenuSubitemsLists;
extern QMap<QString, QString> BoostType_SubitemsList_Match;
extern QMap<QString, QList<QString>> boostsHints;

extern QString boostsTypes2[];

extern QListWidget* listWidget_AutoFill;



#endif // GLOBALVARS_H
