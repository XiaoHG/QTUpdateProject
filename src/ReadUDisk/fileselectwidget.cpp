
#include "fileselectwidget.h"

#include <QMouseEvent>

FileSelectWidget::FileSelectWidget(QWidget *parent)
    :QWidget(parent)
{
    this->setStyleSheet("background-color:rgba(57, 97, 148, 255)");
}

void FileSelectWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    emit clicked();
    this->setStyleSheet("background-color:rgba(57, 97, 148, 255)");
}

void FileSelectWidget::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    this->setStyleSheet("background-color:rgba(57, 97, 148, 100)");
}
