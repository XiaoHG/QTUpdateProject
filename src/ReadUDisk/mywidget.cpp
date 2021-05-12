
#include "mywidget.h"

#include <QMouseEvent>

MyWidget::MyWidget(QWidget *parent)
    :QWidget(parent)
{
    this->setStyleSheet("background-color:rgba(57, 97, 148, 255)");
}

void MyWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    emit clicked();
    this->setStyleSheet("background-color:rgba(57, 97, 148, 255)");
}

void MyWidget::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    this->setStyleSheet("background-color:rgba(57, 97, 148, 100)");
}
