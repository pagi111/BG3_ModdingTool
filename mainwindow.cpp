#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSqlTableModel>
#include <QSqlDatabase>

#include "globalVars.h"
#include "globalFunctions.h"
#include "historyfile.h"
#include "lineeditintablewidget.h"
#include <algorithm>    //for std::sort


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    HistoryFile::Read(this);
    ConnectWidgetsInFrameClassDescription();
    ConnectWidgetsInFrameLevelProgression();

    ui->tableWidget_Boosts->hide();
    ui->btn_Boosts_AddRow->hide();
    //listWidget_AutoFill = new QListWidget;
    ui->list_AutoFill->hide();

    ui->lineEdit_Boosts->installEventFilter(this);
    ui->frame_LevelProgression->installEventFilter(this);
    this->installEventFilter(this);

    QTableWidget* tableWidget = new MyTableClass();
    tableWidget->setRowCount(50);
    tableWidget->setColumnCount(30);
    tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //this->layout()->addWidget(tableWidget);

}

//Taken from: https://stackoverflow.com/questions/9261175/how-to-emit-a-signal-from-a-qpushbutton-when-the-mouse-hovers-over-it
//In mainwindow.h: bool eventFilter(QObject *obj, QEvent *event);
//To add objects: add ui->My_Object->installEventFilter(this); in MainWindow::MainWindow
//Allows for adding new signals and function calls for QObjects
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // This function repeatedly call for those QObjects
    // which have installed eventFilter (Step 2)

    if (obj == (QObject*)ui->lineEdit_Boosts) {
        if (event->type() == QEvent::MouseButtonPress)
        {
            PrintValuesToTableWidget(ui->tableWidget_Boosts, ui->lineEdit_Boosts->text(), this);
            ui->tableWidget_Boosts->show();
            ui->btn_Boosts_AddRow->show();
        }
    }
    if (obj == (QObject*)ui->frame_LevelProgression) {
        if (event->type() == QEvent::MouseButtonPress)
        {
            ui->tableWidget_Boosts->hide();
            ui->btn_Boosts_AddRow->hide();
            //delete listWidget_AutoFill;
        }
    }
    if (obj == (QObject*)this) {
        if (event->type() == QEvent::Close)
        {
            qDebug() << "Main Window closed";
            HistoryFile::Write(this);
        }
    }
    return QWidget::eventFilter(obj, event);
//    else {
//        // pass the event on to the parent class
//        return QWidget::eventFilter(obj, event);
//    }
}


//How to connect object to slots:
//https://forum.qt.io/topic/123639/how-connect-customwiget-signals-and-slots/4
//https://wiki.qt.io/New_Signal_Slot_Syntax
//https://forum.qt.io/topic/16847/how-to-connect-a-forms-signal-to-application-class-slot/4
//How automatic slot connection works:
//https://stackoverflow.com/questions/45305463/where-does-qt-designer-store-the-signal-to-slot-connections
//https://stackoverflow.com/questions/43918098/signal-to-slot-in-qt-creator-where-is-connect-function
//My notes:
//use connect(|widget that sends the signal|, |signal|, |MainForm (or class that contains the function to call)|, |function to call|)
//E.g.connect(lineEdit_Dupa, &QLineEdit::textEdited, this (=MainWindow), &MainWindow::on_ClassAttrs_AllLineEdits_textEdited);
void MainWindow::ConnectWidgetsInFrameClassDescription(){
    for (auto widget : ui->frame_ClassDescription->children()) {
        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget)) {
            //Previously it was on textEdited, not textChanged (textEdited is called only when the user manually changes the text;
            //textChanged is called when the text changes by any means (also programatically)
            //With textChanged this function (which updates the data in the class fields) is called right after the data is loaded into the lineEdit
            //from the class - the same data that is in the class is displayed in the lineEdit, then the function is called because the text
            //changed and the same text is assigned back to the class fields essentially changing nothing - just wasted resources
            //Also, the biggest problem is, that when text changes because another class or progression was chosen, it poses possible problems
            //(like assigning data to the wrong class, especially when changing level value or multiclass)
            //However with textEdited I also need to assign changes to the class if I use the tableWidget that contains data from the lineEdit

            //connect(lineEdit, &QLineEdit::textChanged, this, &MainWindow::on_ClassAttrs_AllLineEdits_textChanged);
            connect(lineEdit, &QLineEdit::textEdited, this, &MainWindow::on_ClassAttrs_AllLineEdits_textEdited);
        }
        else if (QCheckBox* chbx = qobject_cast<QCheckBox*>(widget)) {
            //connect(chbx, &QCheckBox::stateChanged, this, &MainWindow::on_ClassAttrs_AllCheckBoxes_stateChanged);
            connect(chbx, &QCheckBox::clicked, this, &MainWindow::on_ClassAttrs_AllCheckBoxes_clicked);
        }
    }
}

void MainWindow::ConnectWidgetsInFrameLevelProgression(){
    for (auto widget : ui->frame_LevelProgression->children()) {
        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget)) {
            //connect(lineEdit, &QLineEdit::textChanged, this, &MainWindow::on_ProgAttrs_AllLineEdits_textChanged);
            connect(lineEdit, &QLineEdit::textEdited, this, &MainWindow::on_ProgAttrs_AllLineEdits_textEdited);
        }
        else if (QCheckBox* chbx = qobject_cast<QCheckBox*>(widget)) {
            //connect(chbx, &QCheckBox::stateChanged, this, &MainWindow::on_ProgAttrs_AllCheckBoxes_stateChanged);
            connect(chbx, &QCheckBox::clicked, this, &MainWindow::on_ProgAttrs_AllCheckBoxes_clicked);
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_AddValueListsFile_Btn_clicked()
{
    classDescriptionsFileName = QFileDialog::getOpenFileName();
    QFile file(classDescriptionsFileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
    }

    ReadFromFile_ValueLists(&file);

//    QString text;
//    for (auto list : MenuSubitemsLists.asKeyValueRange()){
//        text.append("valuelist \"" + list.first + "\"\n");
//        for (QString value : list.second){
//            text.append("value \"" + value + "\"\n");
//        }
//    }
//    ui->textEdit->setText(text);
}

void MainWindow::on_AddClassFile_Btn_clicked()
{
    classDescriptionsFileName = QFileDialog::getOpenFileName();
    QFile file(classDescriptionsFileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
    }

    //ReadFromFile_Attributes(&file);
    HistoryFile::AddFileToHistoryList(classDescriptionsFileName, &HistoryFile::classDescriptionsList, this);

//    QString text;
//    ui->listClasses->clear();
//    for(auto c : existingClasses.asKeyValueRange()) {
//        //This adds Progressions that correspond to one of the basic classes or subclasses (which have .ClassAttrs)
//        //Other Progression still exists in exisitngClasses though and are later correctly printed but cannot be edited in the program
//        //For now this seems better I think, but later it would be good to add Races, NPC_something classes, etc.
//        QString* className = &c.second.ClassAttrs["Name"]["value"];
////        if (!className->isEmpty()) {
////            ui->listClasses->addItem(*className);
////        }

//        ui->listClasses->addItem(*className);

//        //or... this adds all items from existingClasses (it includes Progressions that do not have 'base class' added - they have no .ClassAttrs
//        //this happens e.g. for races Progressions, as only Classes are added with .ClassAttrs (also for many NPC_something Progressions)
//        //ui->listClasses->addItem(c.first);

////        text.append("-------Class Name: " + c.ClassAttrs["Name"]["value"] + "--------\n");
////        for (QString attrID : c.ClassAttrs.keys()){
////            text.append(attrID + " :: ");
////            for (QString string : c.ClassAttrs[attrID].keys()){
////                text.append(string + "=" + c.ClassAttrs[attrID][string] + " ");
////            }
////            text.append("\n");
////        }
//    }
//    ui->textEdit->setText(text);
}

void MainWindow::on_AddProgressionFile_Btn_clicked()
{
    progressionsFileName = QFileDialog::getOpenFileName();
    QFile file(progressionsFileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
    }

    //ReadFromFile_Attributes(&file);
    HistoryFile::AddFileToHistoryList(progressionsFileName, &HistoryFile::classProgressionsList, this);
}

void MainWindow::on_ImportAllFiles_Btn_clicked(){
    existingClasses.clear();
    for(int i = 0; i < ui->listWidget_ClassDescFiles->count(); ++i) {
        QString filepath = ui->listWidget_ClassDescFiles->item(i)->text();
        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        }
        ReadFromFile_Attributes(&file);
    }
    QString text;
    ui->listClasses->clear();
    for(auto c : existingClasses.asKeyValueRange()) {
        QString* className = &c.second.ClassAttrs["Name"]["value"];
        //        if (!className->isEmpty()) {
        //            ui->listClasses->addItem(*className);
        //        }
        ui->listClasses->addItem(*className);
    }

    for(int i = 0; i < ui->listWidget_ClassProgFiles->count(); ++i) {
        QString filepath = ui->listWidget_ClassProgFiles->item(i)->text();
        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        }
        ReadFromFile_Attributes(&file);
    }

}


void MainWindow::on_ResetHistory_Btn_clicked(){
    HistoryFile::classDescriptionsList.clear();
    HistoryFile::classProgressionsList.clear();
    ui->listWidget_ClassDescFiles->clear();
    ui->listWidget_ClassProgFiles->clear();
}

void MainWindow::on_listClasses_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current == nullptr) {
        currentClass = nullptr;
        ui->label_CurrentClass->setText("");
    }
    else {
        currentClass = &existingClasses[current->text()];
        ui->label_CurrentClass->setText(current->text());
        ui->listLevels->clear();
        QStringList tempList;
        for (auto prog : currentClass->ClassProgressions.asKeyValueRange()){
            tempList.append(prog.first);

        }
        QCollator collator;
        collator.setNumericMode(true);
        std::sort(tempList.begin(), tempList.end(), collator);
//        std::sort(list.begin(), list.end(), [](const QString &lhs, const QString &rhs)
//                  {
//                      int num_lhs = lhs[0].digitValue();
//                      int num_rhs = rhs.split(' ').last().toInt();
//                      return num_lhs < num_rhs;
//                  });
        for (auto item : tempList){
            ui->listLevels->addItem(item);
        }
    }
    QString level = ui->listLevels->currentItem() != nullptr ? ui->listLevels->currentItem()->text() : "0";

    PrintValuesToWidgetsInFrame(ui->frame_ClassDescription, this);
    PrintValuesToWidgetsInFrame(ui->frame_LevelProgression, this, level);
}

void MainWindow::on_listLevels_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current == nullptr){
        ui->label_CurrentLevel->setText("");
        PrintValuesToWidgetsInFrame(ui->frame_LevelProgression, this,  "0");
    }
    else {
        ui->label_CurrentLevel->setText(current->text());
        PrintValuesToWidgetsInFrame(ui->frame_LevelProgression, this,  current->text());
    }

//    if (currentClass == nullptr) { return; }
//    QString text;
//    text.append("Level: " + current->text() + "\n");
//    QMap<QString, QMap<QString, QString>> *currentProgAttribs = &currentClass->ClassProgressions[current->text().toInt()].ProgAttrs;
//    for (auto attrKey : (*currentProgAttribs).keys()){
//        if (attrKey.isEmpty()) { continue; }
//        text.append(attrKey + " : ");
//        for (auto linePart : (*currentProgAttribs)[attrKey].keys()){
//            text.append(linePart + "=" + (*currentProgAttribs)[attrKey][linePart] + " ");
//        }
//        text.append("\n");
//    }
//    ui->textEdit->setText(text);
}

void MainWindow::on_SaveAllToFiles_Btn_clicked()
{
    QString filename = QFileDialog::getSaveFileName();
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QFile::Truncate | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
    }

    PrintToFile_Attributes(&file, "ClassDescriptions", 4, 0, 9, 314);
    //PrintToFile_Attributes(&file, "Progressions", 4, 0, 9, 314);
}

//OPTION 1 - textChanged and stateChanged
void MainWindow::on_ClassAttrs_AllLineEdits_textChanged(const QString &arg1)
{
    if (currentClass == nullptr) { return; }
    QString attrName = QObject::sender()->objectName().replace("lineEdit_", "");
    currentClass->ClassAttrs[attrName]["value"] = arg1;
}

void MainWindow::on_ProgAttrs_AllLineEdits_textChanged(const QString &arg1)
{
    if (currentClass == nullptr) { return; }
    QString attrName = QObject::sender()->objectName().replace("lineEdit_", "");
    QString level = ui->listLevels->currentItem() != nullptr ? ui->listLevels->currentItem()->text() : "0";
    if (level == "0") { return; }
    if (attrName == "Level"){
        if (arg1.isEmpty()) { return; }
        currentClass->ClassProgressions[arg1] = currentClass->ClassProgressions[level];
        currentClass->ClassProgressions[arg1].ProgAttrs[attrName]["value"] = arg1;
    }
    else {
        currentClass->ClassProgressions[level].ProgAttrs[attrName]["value"] = arg1;
    }

}

void MainWindow::on_ClassAttrs_AllCheckBoxes_stateChanged(int arg1)
{
    if (currentClass == nullptr) { return; }
    QString value;
    if      (arg1 == 0) { value = "false"; }
    else if (arg1 == 2) { value = "true"; }
    QString attrName = QObject::sender()->objectName().replace("chbx_", "");
    currentClass->ClassAttrs[attrName]["value"] = value;
}

void MainWindow::on_ProgAttrs_AllCheckBoxes_stateChanged(int arg1)
{
    if (currentClass == nullptr) { return; }
    QString value;
    if      (arg1 == 0) { value = "false"; }
    else if (arg1 == 2) { value = "true"; }
    QString attrName = QObject::sender()->objectName().replace("chbx_", "");
    QString level = ui->listLevels->currentItem() != nullptr ? ui->listLevels->currentItem()->text() : "0";
    if (level == "0") { return; }
    if (attrName == "IsMulticlass"){
        if (value == "true"){
            //assign current level prog (so the one that is currently displaying) to the [level + "m"] index (creating new if it does not exist)
            currentClass->ClassProgressions[level + "m"] = currentClass->ClassProgressions[level];
            currentClass->ClassProgressions[level + "m"].ProgAttrs[attrName]["value"] = value;
        }
        else {
            currentClass->ClassProgressions[level] = currentClass->ClassProgressions[level + "m"];
            currentClass->ClassProgressions[level].ProgAttrs[attrName]["value"] = value;
        }

    }
    else{
        currentClass->ClassProgressions[level].ProgAttrs[attrName]["value"] = value;
    }
}

//OPTION 2 - textEdited and clicked
void MainWindow::on_ClassAttrs_AllLineEdits_textEdited(const QString &arg1)
{
    if (currentClass == nullptr) { return; }
    QString attrName = QObject::sender()->objectName().replace("lineEdit_", "");
    currentClass->ClassAttrs[attrName]["value"] = arg1;
}

void MainWindow::on_ProgAttrs_AllLineEdits_textEdited(const QString &arg1)
{
    if (currentClass == nullptr) { return; }
    QString attrName = QObject::sender()->objectName().replace("lineEdit_", "");
    QString level = ui->listLevels->currentItem() != nullptr ? ui->listLevels->currentItem()->text() : "0";
    if (level == "0") { return; }
    if (attrName == "Level"){
        if (arg1.isEmpty()) { return; }
        currentClass->ClassProgressions[arg1] = currentClass->ClassProgressions[level];
        currentClass->ClassProgressions[arg1].ProgAttrs[attrName]["value"] = arg1;
    }
    else {
        currentClass->ClassProgressions[level].ProgAttrs[attrName]["value"] = arg1;
    }
}

void MainWindow::on_ClassAttrs_AllCheckBoxes_clicked()
{
    if (currentClass == nullptr) { return; }
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(QObject::sender());
    QString value;
    if      (checkBox->checkState() == Qt::CheckState::Unchecked) { value = "false"; }
    else if (checkBox->checkState() == Qt::CheckState::Checked) { value = "true"; }
    QString attrName = QObject::sender()->objectName().replace("chbx_", "");
    currentClass->ClassAttrs[attrName]["value"] = value;
}

void MainWindow::on_ProgAttrs_AllCheckBoxes_clicked()
{
    if (currentClass == nullptr) { return; }
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(QObject::sender());
    QString value;
    if      (checkBox->checkState() == Qt::CheckState::Unchecked) { value = "false"; }
    else if (checkBox->checkState() == Qt::CheckState::Checked) { value = "true"; }
    QString attrName = QObject::sender()->objectName().replace("chbx_", "");
    QString level = ui->listLevels->currentItem() != nullptr ? ui->listLevels->currentItem()->text() : "0";
    if (level == "0") { return; }
    if (attrName == "IsMulticlass"){
        if (value == "true"){
            //assign current level prog (so the one that is currently displaying) to the [level + "m"] index (creating new if it does not exist)
            currentClass->ClassProgressions[level + "m"] = currentClass->ClassProgressions[level];
            currentClass->ClassProgressions[level + "m"].ProgAttrs[attrName]["value"] = value;
        }
        else {
            currentClass->ClassProgressions[level] = currentClass->ClassProgressions[level + "m"];
            currentClass->ClassProgressions[level].ProgAttrs[attrName]["value"] = value;
        }

    }
    else{
        currentClass->ClassProgressions[level].ProgAttrs[attrName]["value"] = value;
    }
}


void MainWindow::on_btn_Boosts_AddRow_clicked()
{
    int rowIndex = ui->tableWidget_Boosts->rowCount();
    ui->tableWidget_Boosts->insertRow(rowIndex);
    new LineEditInTableWidget(ui->tableWidget_Boosts, rowIndex, 0, "", this);
}

void MainWindow::TableDeleteRow(QTableWidget* tw, int rowIndex){
    QWidget* widget = nullptr;
    if (tw == ui->tableWidget_Boosts){
        widget = ui->lineEdit_Boosts;
    }
    tw->removeRow(rowIndex);
    UpdateWidgetTextFromTableWidget(widget, tw);

    //ui->textEdit->setText(QString::number(rowIndex));
    //int progLevel = ui->label_CurrentLevel->text() != nullptr ? ui->label_CurrentLevel->text().toInt() : 0;
    //if (progLevel <= 0) { return; }
    //UpdateWidgetTextFromTableWidget(widget, tw, progLevel);
}

//How to pass a parameter to a function with connect:
//https://forum.qt.io/topic/129966/connect-how-to-pass-a-parameter-to-the-function

//How to create a context menu:
//https://forum.qt.io/topic/111691/create-context-menu-when-right-click-on-header-of-qtreewidget
//https://www.qtcentre.org/threads/24556-Adding-actions-to-a-context-menu
//https://stackoverflow.com/questions/2711267/context-menu-creation-with-qt-designerqt-creator-ide


void MainWindow::on_listWidget_ClassDescFiles_customContextMenuRequested(const QPoint &pos)
{
    QListWidget* lw = qobject_cast<QListWidget*>(QObject::sender());
    int rowIndex = lw->indexAt(pos).row();

    QMenu* menu = new QMenu();
    QAction *action_DeleteRow = menu->addAction("Delete Row");
    connect(action_DeleteRow, &QAction::triggered, menu, [lw, menu, rowIndex](){
        QString itemName = lw->item(rowIndex)->text();
        lw->model()->removeRow(rowIndex);
        int indexHistory = HistoryFile::classDescriptionsList.indexOf(itemName);
        HistoryFile::classDescriptionsList.removeAt(indexHistory);
        delete menu;
    });

    menu->popup(lw->viewport()->mapToGlobal(pos));
}

