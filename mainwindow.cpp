#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSqlTableModel>
#include <QSqlDatabase>

#include "globalVars.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ConnectWidgetsInFrameClassDescription();
    ConnectWidgetsInFrameLevelProgression();

    //ui->tableWidget->setIndexWidget(ui->tableWidget->model()->index(3, 1), new QPushButton("Button"));
    //ui->tableWidget->setItem(3, 2, new QTableWidgetItem("Item"));


    QTableWidget* tableWidget = new MyTableClass();
    tableWidget->setRowCount(50);
    tableWidget->setColumnCount(30);
    tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //this->layout()->addWidget(tableWidget);

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
            connect(lineEdit, &QLineEdit::textEdited, this, &MainWindow::on_ClassAttrs_AllLineEdits_textEdited);
        }
        else if (QCheckBox* chbx = qobject_cast<QCheckBox*>(widget)) {
            connect(chbx, &QCheckBox::stateChanged, this, &MainWindow::on_ClassAttrs_AllCheckBoxes_stateChanged);
        }
    }
}

void MainWindow::ConnectWidgetsInFrameLevelProgression(){
    for (auto widget : ui->frame_LevelProgression->children()) {
        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget)) {
            connect(lineEdit, &QLineEdit::textEdited, this, &MainWindow::on_ProgAttrs_AllLineEdits_textEdited);
        }
        else if (QCheckBox* chbx = qobject_cast<QCheckBox*>(widget)) {
            connect(chbx, &QCheckBox::stateChanged, this, &MainWindow::on_ProgAttrs_AllCheckBoxes_stateChanged);
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_AddClassFile_Btn_clicked()
{
    classDescriptionsFileName = QFileDialog::getOpenFileName();
    QFile file(classDescriptionsFileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
    }

    ReadFromFile_Attributes(&file);

    QString text;
    for(CharacterClass c : existingClasses) {
        text.append("-------Class Name: " + c.ClassAttrs["Name"]["value"] + "--------\n");
        for (QString attrID : c.ClassAttrs.keys()){
            text.append(attrID + " :: ");
            for (QString string : c.ClassAttrs[attrID].keys()){
                text.append(string + "=" + c.ClassAttrs[attrID][string] + " ");
            }
            text.append("\n");
        }

        ui->listClasses->addItem(c.ClassAttrs["Name"]["value"]);
    }

    ui->textEdit->setText(text);
}

void MainWindow::on_AddProgressionFile_Btn_clicked()
{
    progressionsFileName = QFileDialog::getOpenFileName();
    QFile file(progressionsFileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
    }

    ReadFromFile_Attributes(&file);
}

void MainWindow::on_listClasses_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    currentClass = &existingClasses[current->text()];
    ui->label_CurrentClass->setText(current->text());

    PrintValuesToWidgetsInFrame(ui->frame_ClassDescription);
    int level = ui->listLevels->currentItem() != nullptr ? ui->listLevels->currentItem()->text().toInt() : 1;
    PrintValuesToWidgetsInFrame(ui->frame_LevelProgression, level);
}

void MainWindow::on_listLevels_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->label_CurrentLevel->setText(current->text());
    PrintValuesToWidgetsInFrame(ui->frame_LevelProgression, current->text().toInt());


    QString text;
    text.append("Level: " + current->text() + "\n");
    QMap<QString, QMap<QString, QString>> *currentProgAttribs = &currentClass->ClassProgressions[current->text().toInt()].ProgAttrs;
    for (auto attrKey : (*currentProgAttribs).keys()){
        if (attrKey.isEmpty()) { continue; }
        text.append(attrKey + " : ");
        for (auto linePart : (*currentProgAttribs)[attrKey].keys()){
            text.append(linePart + "=" + (*currentProgAttribs)[attrKey][linePart] + " ");
        }
        text.append("\n");
    }
    ui->textEdit->setText(text);
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


void MainWindow::on_ClassAttrs_AllLineEdits_textEdited(const QString &arg1)
{
    QString attrName = QObject::sender()->objectName().replace("lineEdit_", "");
    currentClass->ClassAttrs[attrName]["value"] = arg1;
}

void MainWindow::on_ProgAttrs_AllLineEdits_textEdited(const QString &arg1)
{
    QString attrName = QObject::sender()->objectName().replace("lineEdit_", "");
    int level = ui->label_CurrentLevel->text() != nullptr ? ui->label_CurrentLevel->text().toInt() : 0;
    if (level <= 0) { return; }
    currentClass->ClassProgressions[level].ProgAttrs[attrName]["value"] = arg1;
}

void MainWindow::on_ClassAttrs_AllCheckBoxes_stateChanged(int arg1)
{
    QString value;
    if      (arg1 == 0) { value = "false"; }
    else if (arg1 == 2) { value = "true"; }
    QString attrName = QObject::sender()->objectName().replace("chbx_", "");
    currentClass->ClassAttrs[attrName]["value"] = value;
}

void MainWindow::on_ProgAttrs_AllCheckBoxes_stateChanged(int arg1)
{
    QString value;
    if      (arg1 == 0) { value = "false"; }
    else if (arg1 == 2) { value = "true"; }
    QString attrName = QObject::sender()->objectName().replace("chbx_", "");
    int level = ui->label_CurrentLevel->text() != nullptr ? ui->label_CurrentLevel->text().toInt() : 0;
    if (level <= 0) { return; }
    currentClass->ClassProgressions[level].ProgAttrs[attrName]["value"] = value;
}


void MainWindow::on_tableWidget_Boosts_cellClicked(int row, int column)
{
    //This reads (or sets) the text of the QTableWidgetItem - it needs to be added first if it doesn't exist
    //qobject_cast<QTableWidget*>(QObject::sender())->item(row, column)->setText("Dupa 2");

    //This read data (text) from the cell
    QString cellText = qobject_cast<QTableWidget*>(QObject::sender())->model()->index(row,column).data().toString();

    cellText += "_New";

    //This adds an item (text) to the cell
    QAbstractItemModel* model = qobject_cast<QTableWidget*>(QObject::sender())->model();
    QModelIndex idx = model->index(row, column);
    model->setData(idx, QString(cellText)); //or QStringLiteral("some text"); or simply "some text";


    //This changes Widget in the cell - but if widget is in the cell, then cellClicked event is not called
//    QWidget* widget = qobject_cast<QTableWidget*>(QObject::sender())->cellWidget(row + 1, column);
//    if (QPushButton* btn = qobject_cast<QPushButton*>(widget)){
//        btn->setText("Button text changed");
//    }
}


void MainWindow::on_tableWidget_Boosts_itemChanged(QTableWidgetItem *item)
{
    QString attrName = "Boosts";
    int level = ui->label_CurrentLevel->text() != nullptr ? ui->label_CurrentLevel->text().toInt() : 0;
    if (level <= 0) { return; }
    QString string;
    for (int i = 0; i < ui->tableWidget_Boosts->rowCount(); i++){
        int rowCount = ui->tableWidget_Boosts->rowCount();
        QString rowString = ui->tableWidget_Boosts->model()->index(i,0).data().toString();
        string = i ==  rowCount - 1 ? string + rowString :
           string + rowString + ";" ;
    }

    currentClass->ClassProgressions[level].ProgAttrs[attrName]["value"] = string;
}


void MainWindow::on_btn_Boosts_AddRow_clicked()
{
    ui->tableWidget_Boosts->insertRow(ui->tableWidget_Boosts->rowCount());
}

void DeleteRow(QTableWidget* tw, int rowIndex){
    tw->removeRow(rowIndex);
}

QMenu* menu = nullptr;

void MainWindow::on_tableWidget_Boosts_customContextMenuRequested(const QPoint &pos)
{
    //How to pass a parameter to a function with connect:
    //https://forum.qt.io/topic/129966/connect-how-to-pass-a-parameter-to-the-function

    //How to create a context menu:
    //https://forum.qt.io/topic/111691/create-context-menu-when-right-click-on-header-of-qtreewidget
    //https://www.qtcentre.org/threads/24556-Adding-actions-to-a-context-menu
    //https://stackoverflow.com/questions/2711267/context-menu-creation-with-qt-designerqt-creator-ide

    int rowIndex = ui->tableWidget_Boosts->indexAt(pos).row();
    //QTableWidgetItem* item = ui->tableWidget_Boosts->currentItem();
    QTableWidget* tw = ui->tableWidget_Boosts;
    //QMenu* menu = new QMenu();
    menu = new QMenu();
    QAction *action_DeleteRow = menu->addAction("Delete Row");
    //connect(myAction, &QAction::triggered, this, [this, rowIndex](){ DeleteRow(ui->tableWidget_Boosts, rowIndex); });
    connect(action_DeleteRow, &QAction::triggered, this, [tw, rowIndex, this](){ ui->tableWidget_Boosts->removeRow(rowIndex); });
    menu->popup(ui->tableWidget_Boosts->viewport()->mapToGlobal(pos));
}

