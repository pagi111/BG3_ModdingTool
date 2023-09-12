#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include "mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QtWidgets>
#include <QToolTip>

void ReadFromFile_ValueLists(QFile *file);

QString FindAttrInLine(QString attr, QString *line);

QMap<QString, QString> FindAttrsInLine(QString* id, QString* line);

void CreateLineEditAsTableItem(QTableWidget* tw, int row, int col, QString text);

//void UpdateWidgetTextFromTableWidget(QWidget* widget, QTableWidget* tableWidget, int progLevel = 0);
void UpdateWidgetTextFromTableWidget(QWidget* widget, QTableWidget* tableWidget);

void PrintValuesToTableWidget(QTableWidget* tw, QString attrValue, MainWindow* mw);

void PrintValuesToWidgetsInFrame(QFrame* frame, MainWindow *mw, QString level = "1");

void ReadFromFile_Attributes(QFile *file);

void PrintToFile_Attributes(QFile *file, QString region, int verMajor, int verMinor, int verRev, int verBuild);

#endif // GLOBALFUNCTIONS_H
