#include "lineeditintablewidget.h"
#include "globalVars.h"
#include "qevent.h"
#include "tablecontextmenu.h"

LineEditInTableWidget::LineEditInTableWidget(QTableWidget* tw, int row, int col, QString text, MainWindow* mw)
{
    this->setText(text);
    this->setFrame(false);

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

    connect(this, &QLineEdit::textChanged, [tw, this, mw]() {
        QPoint pos = QPoint(this->pos().x(), this->pos().y() + this->height());
        QString leText = this->text();
        QString boost = this->text().split("(").first();
        QString boostArgsString = this->text().split("(").last();
        boostArgsString.removeAt(boostArgsString.lastIndexOf(")"));
        qDebug() << "boostArgsString: " << boostArgsString;


        QStringList boostArgs = boostArgsString.split(",");
        int cursorPosInArgs = this->cursorPosition() - boost.length();
        qDebug() << "Cursor: " << cursorPosInArgs;
        qDebug() << "BoostLength: " << boostArgsString.length();
        int boostArgNumber = 1;

        //cursor at the end of the line means at least boostArgsString.length() + 1
        if (cursorPosInArgs - 1 > boostArgsString.length()){
            return;
        }
        else {
            for (int i = 0; i < cursorPosInArgs - 1; i++){
                if (boostArgsString[i] == ',') { boostArgNumber++; }
            }
        }

        qDebug() << "boostArgNumber: " << boostArgNumber;

        QString listName;
        for (QString boostVariant : boostsHints[boost]){
            QStringList arguments = boostVariant.split("[").first().replace("(", "").replace(")", "").split(",");
            qDebug() << "arguments: " << arguments;
            if (boostArgNumber <= arguments.count()){
                listName = "List_" + arguments[boostArgNumber - 1].trimmed();
            }
        }

        qDebug() << "listName: " << listName;
        qDebug() << "boostArgs[boostArgNumber - 1]: " << boostArgs[boostArgNumber - 1];

        listWidget_AutoFill->clear();
//        for (QString arg : List_ActionResource){
//            if (arg.contains(leText, Qt::CaseInsensitive)){
//                listWidget_AutoFill->addItem(arg);
//            }
//        }
        for (QString arg : MenuSubitemsLists[listName]){
            if (arg.contains(boostArgs[boostArgNumber - 1].trimmed(), Qt::CaseInsensitive)){
                listWidget_AutoFill->addItem(arg);
            }
        }
        listWidget_AutoFill->setParent(tw);
        listWidget_AutoFill->show();
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
