#ifndef TABLECONTEXTMENU_H
#define TABLECONTEXTMENU_H

#include "mainwindow.h"
#include "qtablewidget.h"
#include <QMenu>

class TableContextMenu : public QMenu{
public:
    TableContextMenu(){};
    //, const QPoint &mousePos
    TableContextMenu(QTableWidget* sender, MainWindow* mainWindow);

    TableContextMenu(QTableWidget* tw, QLineEdit* le, MainWindow* mainWindow);
};

#endif // TABLECONTEXTMENU_H
