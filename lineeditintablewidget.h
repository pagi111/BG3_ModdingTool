#ifndef LINEEDITINTABLEWIDGET_H
#define LINEEDITINTABLEWIDGET_H


#include "mainwindow.h"
#include "qlineedit.h"
class LineEditInTableWidget : public QLineEdit
{
    Q_OBJECT
public:
    LineEditInTableWidget(QTableWidget* tw, int row, int col, QString text, MainWindow* mw);

signals:
    void sglLeftMouseButtonPressed(const QPoint &mousePos);
    void sglRightMouseButtonPressed(const QPoint &mousePos);
    void sglMousePress(QMouseEvent *event);
    void sglKeyPress(QKeyEvent *event);

protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // LINEEDITINTABLEWIDGET_H
