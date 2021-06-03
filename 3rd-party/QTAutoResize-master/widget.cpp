#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QPushButton *btnTest = new QPushButton(this);
    btnTest->setGeometry(20, 20, 100, 30);
    btnTest->setText("This a txt");

    m_autoResizeHandler=new AutoResize(this,this->rect().width(),this->rect().height());
    m_autoResizeHandler->setAutoResizeFlag(
            AutoResize::INCLUDE_BUTTON|AutoResize::INCLUDE_COMBOBOX|
            AutoResize::INCLUDE_EDITOR|AutoResize::INCLUDE_LABEL
            );
    //add widget* manualy
//    m_autoResizeHandler->addOtherItem(ui->frame);
    m_autoResizeHandler->pushAllResizeItem();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::resizeEvent(QResizeEvent *event)
{
    m_autoResizeHandler->doAutoResize();
}
