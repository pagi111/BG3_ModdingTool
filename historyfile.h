#include <QFile>
#include "mainwindow.h"

#ifndef HISTORYFILE_H
#define HISTORYFILE_H


class HistoryFile
{
public:
    static const int MAX_HISTORY_ITEMS;
    static QString filePath;
    static QStringList classDescriptionsList;
    static QStringList classProgressionsList;
    static void Read(MainWindow* mw);
    static void Write(MainWindow* mw);
    static void AddFileToHistoryList(QString filepath, QStringList* list, MainWindow* mw);
private:
    HistoryFile();
};

#endif // HISTORYFILE_H
