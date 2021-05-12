
#include "mywidget.h"

#include <QMouseEvent>

MyWidget::MyWidget(QWidget *parent)
    :QWidget(parent)
{
    this->setStyleSheet("background-color:rgba(150, 150, 150, 100)");
}

void MyWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    emit clicked();
    this->setStyleSheet("background-color:rgba(150, 150, 150, 100)");
}

void MyWidget::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    this->setStyleSheet("background-color:rgba(200, 200, 200, 100)");
}
