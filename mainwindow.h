#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtablewidget.h"
#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AddClassFile_Btn_clicked();

    void on_AddProgressionFile_Btn_clicked();

    void on_SaveAllToFiles_Btn_clicked();

    void on_listClasses_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listLevels_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_ClassAttrs_AllLineEdits_textEdited(const QString &arg1);
    void on_ProgAttrs_AllLineEdits_textEdited(const QString &arg1);
    void on_ClassAttrs_AllCheckBoxes_stateChanged(int arg1);
    void on_ProgAttrs_AllCheckBoxes_stateChanged(int arg1);

    void on_tableWidget_Boosts_cellClicked(int row, int column);

    void on_tableWidget_Boosts_itemChanged(QTableWidgetItem *item);

    void on_btn_Boosts_AddRow_clicked();

    void on_tableWidget_Boosts_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    void ConnectWidgetsInFrameClassDescription();
    void ConnectWidgetsInFrameLevelProgression();
    //void DeleteRow(QTableWidget* tw, int rowIndex);

};
#endif // MAINWINDOW_H
