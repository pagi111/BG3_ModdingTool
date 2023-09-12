#include "lineeditintablewidget.h"
#include "globalFunctions.h"
#include "globalVars.h"
#include "qevent.h"
#include "tablecontextmenu.h"
#include "ui_mainwindow.h"

LineEditInTableWidget::~LineEditInTableWidget(){
    delete listWidget_AutoFill;
}

LineEditInTableWidget::LineEditInTableWidget(QTableWidget* tw, int row, int col, QString text, MainWindow* mw)
{
    this->setText(text);
    this->setFrame(false);
    listWidget_AutoFill = new QListWidget;

    connect(this, &LineEditInTableWidget::sglMousePress, [tw, this, mw] (QMouseEvent* event){
        if (event->button() == Qt::MouseButton::RightButton){
            new TableContextMenu(tw, this, mw);
        }
        else if (event->button() == Qt::MouseButton::LeftButton){
            if (listWidget_AutoFill->isVisible()){
                listWidget_AutoFill->hide();
            }
        }
        QLineEdit::mousePressEvent(event);
    });

    connect(this, &LineEditInTableWidget::sglKeyPress, [this] (QKeyEvent* event){
        if (listWidget_AutoFill->isVisible()){
            if (event->key() == Qt::Key_Down){
                //Cannot setFocus() directly, because apparently it gets called too early
                //That's why it's necessary to use QTimer (delaying setting focus after the regular key press function is finished)
                //https://stackoverflow.com/questions/56787975/how-to-set-focus-after-execution-qaction
                //https://stackoverflow.com/questions/526761/set-qlineedit-focus-in-qt
                QTimer::singleShot(10, listWidget_AutoFill, SLOT(setFocus()));
                listWidget_AutoFill->setCurrentRow(0); //It seems that does not throw index out of range even if there is no row
            }
            else if (event->key() == Qt::Key_Up){
                listWidget_AutoFill->hide();
            }
        }
    });

    connect(this, &QLineEdit::textChanged, [tw, this, mw](const QString &arg1) {
        //This updates text in the lineEdit_Boosts in frame_Progressions
        UpdateWidgetTextFromTableWidget(mw->ui->lineEdit_Boosts, tw);

        QString level = mw->ui->listLevels->currentItem() != nullptr ? mw->ui->listLevels->currentItem()->text() : "0";
        if (level != "0"){
            currentClass->ClassProgressions[level].ProgAttrs["Boosts"]["value"] = arg1;
        }

        //This part is about showing a list of possible items for autofill
        QPoint pos = QPoint(this->pos().x(), this->pos().y() + this->height());
        QString boost = this->text().split("(").first();
        QString boostArgsString = this->text().split("(").last();
        if (boost == boostArgsString) { return; }   //They being the same means there is no "(" to split
        boostArgsString.removeAt(boostArgsString.lastIndexOf(")"));
        QStringList boostArgs = boostArgsString.split(",");

        int cursorPosInArgs = this->cursorPosition() - boost.length();
        int boostArgNumber = 1;
        //cursor at the end of the line means at least boostArgsString.length() + 1
        if (cursorPosInArgs - 1 > boostArgsString.length()) { return; }
        for (int i = 0; i < cursorPosInArgs - 1; i++){
            if (boostArgsString[i] == ',') { boostArgNumber++; }
        }

        //It will only take the listName from the last Variant anyway - need to improve it
        QStringList listNames;
        if (boost == "ProficiencyBonus" && boostArgs[0].trimmed() == "Skill"){
            listNames.clear();
            listNames.append("Skill");
        }
        else if (boost == "ProficiencyBonus" && boostArgs[0].trimmed() == "SavingThrow"){
            listNames.clear();
            listNames.append("Ability");
        }
        else{
            for (QString boostArgsVariant : boostsHints[boost]){
                QStringList variantArgs = boostArgsVariant.split("[").first().replace("(", "").replace(")", "").split(",");
                if (boostArgNumber <= variantArgs.count()){
                    listNames = variantArgs[boostArgNumber - 1].split("/");
                }
            }
        }
        if (listNames.count() == 0) { return; }

        listWidget_AutoFill->clear();
        for (QString listName : listNames){
            for (QString arg : MenuSubitemsLists["List_" + listName.trimmed()]){
                if (arg.contains(boostArgs[boostArgNumber - 1].trimmed(), Qt::CaseInsensitive)){
                    listWidget_AutoFill->addItem(arg);
                    connect(listWidget_AutoFill, &QListWidget::itemActivated, listWidget_AutoFill, [this, boost, boostArgs, boostArgNumber] (QListWidgetItem* item){
                        QString newBoostText = boost + "(";
                        for (int i = 0; i < boostArgs.count(); i++){
                            if (i == boostArgNumber - 1) { newBoostText.append(item->text()); }
                            else { newBoostText.append(boostArgs[i]); }
                            if (i == boostArgs.count() - 1){
                                newBoostText.append(")");
                            }
                            else {
                                newBoostText.append(", ");
                            }
                        }
                        this->setText(newBoostText);
                    });
                }
            }
        }
        if (listWidget_AutoFill->count() == 0) { return; }
        listWidget_AutoFill->setParent(tw);
        listWidget_AutoFill->show();
        listWidget_AutoFill->setGeometry(pos.x(), pos.y(), 281, 91);
        listWidget_AutoFill->move(pos);

    });

    tw->setCellWidget(row, col, this);
}

void LineEditInTableWidget::mousePressEvent(QMouseEvent* event)
{
    emit sglMousePress(event);
    QLineEdit::mousePressEvent(event); //QLineEdit::mouse... not QWidget::mouse... because then it does not retain info where the cursor should be in the LineEdit

}

void LineEditInTableWidget::keyPressEvent(QKeyEvent* event)
{
    QLineEdit::keyPressEvent(event);
    emit sglKeyPress(event);
}
