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

    bool eventFilter(QObject *obj, QEvent *event);
    void TableDeleteRow(QTableWidget* tw, int rowIndex);
    Ui::MainWindow *ui;

private slots:
    void on_AddClassFile_Btn_clicked();

    void on_AddProgressionFile_Btn_clicked();

    void on_SaveAllToFiles_Btn_clicked();

    void on_listClasses_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listLevels_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_ClassAttrs_AllLineEdits_textChanged(const QString &arg1);
    void on_ProgAttrs_AllLineEdits_textChanged(const QString &arg1);
    void on_ClassAttrs_AllCheckBoxes_stateChanged(int arg1);
    void on_ProgAttrs_AllCheckBoxes_stateChanged(int arg1);

    void on_ClassAttrs_AllLineEdits_textEdited(const QString &arg1);
    void on_ProgAttrs_AllLineEdits_textEdited(const QString &arg1);
    void on_ClassAttrs_AllCheckBoxes_clicked();
    void on_ProgAttrs_AllCheckBoxes_clicked();

    void on_btn_Boosts_AddRow_clicked();

    void on_AddValueListsFile_Btn_clicked();

    void on_ImportAllFiles_Btn_clicked();

    void on_ResetHistory_Btn_clicked();

    void on_listWidget_ClassDescFiles_customContextMenuRequested(const QPoint &pos);

private:
    void ConnectWidgetsInFrameClassDescription();
    void ConnectWidgetsInFrameLevelProgression();
    void ReadFromFile_History();

};
#endif // MAINWINDOW_H
