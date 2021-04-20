#include "updateclientui.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>

UpdateClientUI::UpdateClientUI(QWidget *parent)
    :QWidget(parent)
{
    init();
}

UpdateClientUI::~UpdateClientUI()
{
}

/*init all*/
void UpdateClientUI::init()
{
    UI();
    //init isUpdate = false
    isUpdate = false;
}

/*UI defined*/
void UpdateClientUI::UI()
{
    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:rgb(150, 150, 150)");
    //Set update client dialog fix size
    this->setFixedSize(600, 400);
    //title
    this->setWindowTitle("Update Client");
    this->setWindowIcon(QIcon(":/icon/image/grape.png"));

    //add a Text edit widget for output file that need to update
    outputEdit = new QTextEdit(this);
    //outputEdit->setEnabled(false);
    outputEdit->setFocusPolicy(Qt::NoFocus);
    //outputEdit->setWindowFlags(Qt::FramelessWindowHint);
    outputEdit->setText("This is a test");
    outputEdit->setFrameShape(QFrame::NoFrame);
    outputEdit->setGeometry(0, 0, 350, this->height());
    outputEdit->setStyleSheet("background-color:rgb(75, 75, 75)");
    outputEdit->setTextColor(QColor(255, 255, 255, 255));

    //splitter for ok and cansel button and style
    btnOk = new QPushButton(this);
    btnOk->setText("UPDATE");
    btnOk->setGeometry(370, this->height() - 80, 100, 50);
    btnOk->setWindowOpacity(0.5);

    btnCansel = new QPushButton(this);
    btnCansel->setText("CANSEL");
    btnCansel->setFixedSize(100, 50);
    btnCansel->setGeometry(480, this->height() - 80, 100, 50);

    connect(btnOk, SIGNAL(clicked(bool)), this, SLOT(slotPrintUpdateFilesName()));
    connect(btnCansel, SIGNAL(clicked(bool)), this, SLOT(close()));
}

/*update or not, checked update*/
bool UpdateClientUI::checkUpdate()
{
    //checked update and set isUpdate flag.
    isUpdate = true;
    slotPrintUpdateFilesName();
    return isUpdate;
}

void UpdateClientUI::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //获得鼠标的初始位置
        mouseStartPoint = event->globalPos();
        //mouseStartPoint = event->pos();
        //获得窗口的初始位置
        windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void UpdateClientUI::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        //获得鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        //QPoint distance = event->pos() - mouseStartPoint;
        //改变窗口的位置
        this->move(windowTopLeftPoint + distance);
    }
}

void UpdateClientUI::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

/*
    out put files that need to update.
    show at line edit widget.
*/
void UpdateClientUI::slotPrintUpdateFilesName()
{
    if(isUpdate)
    {
        //Update function checked update and got update files outputEdit output.
        update();
        outputEdit->clear();
        for(int i = 0; i < 10; ++i)
        {
            outputEdit->append(QString::asprintf("file number %d.file", i));
        }
    }
    else
    {
        outputEdit->clear();
        outputEdit->append("Already the latest version");
        btnOk->setEnabled(false);
    }
}

/*update function*/
void UpdateClientUI::update()
{

}

void UpdateClientUI::close()
{
    if(isUpdate)
        outputEdit->clear();
    this->hide();
}
