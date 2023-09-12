#include "historyfile.h"
#include "globalVars.h"
#include "ui_mainwindow.h"

HistoryFile::HistoryFile() {}

const int HistoryFile::MAX_HISTORY_ITEMS = 5;
QString HistoryFile::filePath = baseDirectoryPath + "History.txt";
QStringList HistoryFile::classDescriptionsList = QStringList();
QStringList HistoryFile::classProgressionsList = QStringList();

void HistoryFile::Read(MainWindow* mw){
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(mw, "Warning", "Cannot open file: " + file.errorString());
        return;
    }

    QTextStream stream(&file);

    QString line;
    int part = 0;
    QList<QString>* currentValueList = nullptr;
    while(!(line = stream.readLine()).isNull()){
        if (line == "Class Descriptions:")
        {
            part = 1;
            continue;
        }
        if (line == "Class Progressions:")
        {
            part = 2;
            continue;
        }
        if (part == 1) { classDescriptionsList.append(line); }
        if (part == 2) { classProgressionsList.append(line); }
    }
    mw->ui->listWidget_ClassDescFiles->addItems(classDescriptionsList);
    mw->ui->listWidget_ClassProgFiles->addItems(classProgressionsList);
}

void HistoryFile::Write(MainWindow* mw){
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODeviceBase::Truncate | QIODeviceBase::Text)){
        QMessageBox::warning(mw, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    //QTextStream stream(&filePath, QIODeviceBase::Truncate);
    QTextStream stream(&file);

    QString line;
    int part = 0;
    QList<QString>* currentValueList = nullptr;
    stream << "Class Descriptions:" << "\n";
    for (QString folder : classDescriptionsList)
    {
        stream << folder << "\n";
    }
    stream << "Class Progressions:" << "\n";
    for (QString folder : classProgressionsList)
    {
        stream << folder << "\n";
    }
}

void HistoryFile::AddFileToHistoryList(QString filepath, QStringList* list, MainWindow* mw){
    if (list->contains(filepath)) { list->removeAt(list->indexOf(filepath)); }
    list->insert(0, filepath);
    if (list->count() > MAX_HISTORY_ITEMS) { list->remove(MAX_HISTORY_ITEMS, list->count() - MAX_HISTORY_ITEMS); }

    QListWidget* listWidget;
    if (list == &classDescriptionsList) {
        listWidget = mw->ui->listWidget_ClassDescFiles;
    }
    else if (list == &classProgressionsList) {
        listWidget = mw->ui->listWidget_ClassProgFiles;
    }
    else {
        return;
    }    //ADD MESSAGE

    listWidget->clear();
    for (QString path : *list){
        listWidget->addItem(path);
    }
}


