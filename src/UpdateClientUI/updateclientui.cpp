#include "updateclientui.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>

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
    isUpdate = true;
}

/*UI defined*/
void UpdateClientUI::UI()
{
    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:rgb(75, 75, 75)");
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
    outputEdit->setGeometry(20, 20, 330, this->height() - 40);
    outputEdit->setStyleSheet("background-color:rgb(75, 75, 75)");
    outputEdit->setTextColor(QColor(255, 255, 255, 255));


    //line edit widget to varsion notify
    QFont font( "Microsoft YaHei", 20, 75);
    vNotifyLabel = new QLabel(this);
    vNotifyLabel->setGeometry(0, 0, this->width(), this->height());
    vNotifyLabel->setFont(font);
    vNotifyLabel->setText("Already the latest version\nCurrent: V1.0.1\n\n");
    vNotifyLabel->setAlignment(Qt::AlignCenter);
    vNotifyLabel->setScaledContents(true);
    vNotifyLabel->setStyleSheet("color:white");
    vNotifyLabel->setStyleSheet("background-color:rgb(75, 75, 75)");
    vNotifyLabel->hide();

    //splitter for update and cansel button and style
    btnUpdate = new QPushButton(this);
    btnUpdate->setText("UPDATE");
    btnUpdate->setGeometry(350, this->height() - 100, 100, 50);
    btnUpdate->setWindowOpacity(0.5);
    btnUpdate->setStyleSheet("background-color:rgb(255, 255, 255)");

    btnCansel = new QPushButton(this);
    btnCansel->setText("CANSEL");
    btnCansel->setFixedSize(100, 50);
    btnCansel->setGeometry(460, this->height() - 100, 100, 50);
    btnCansel->setStyleSheet("background-color:rgb(255, 255, 255)");

    //ok
    btnOk = new QPushButton(this);
    btnOk->setText("OK");
    btnOk->setGeometry(btnUpdate->x() + 20, btnUpdate->y() - 30, btnUpdate->width() + 30, btnUpdate->height());
    btnOk->setStyleSheet("background-color:rgb(150, 150, 150)");
    btnOk->hide();

    connect(btnOk, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(btnUpdate, SIGNAL(clicked(bool)), this, SLOT(slotUpdate()));
    connect(btnCansel, SIGNAL(clicked(bool)), this, SLOT(close()));
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

/*update or not, checked update*/
bool UpdateClientUI::checkUpdate()
{
    //checked update and set isUpdate flag.
    //isUpdate = true;
    if(isUpdate)
    {
        //show update files
        outputEdit->clear();
        for(int i = 0; i < 10; ++i)
        {
            outputEdit->append(QString::asprintf("file number %d.file", i));
        }
    }
    else
    {
        vNotifyLabel->show();
        btnUpdate->hide();
        btnCansel->hide();
        btnOk->show();
    }
    return isUpdate;
}

/*update function*/
void UpdateClientUI::slotUpdate()
{
    //Update function checked update and got update files outputEdit output.
    if(checkUpdate())
    {
        if(update())
        {
            //add a slider
            QMessageBox::information(this, "update success", "Please restart app for run the laster varsion",
                                     QMessageBox::Ok, QMessageBox::NoButton);
            this->close();
            isUpdate = false;
        }
    }
}

bool UpdateClientUI::update()
{
    return true;
}

void UpdateClientUI::slotClose()
{
    if(isUpdate)
        outputEdit->clear();
    this->hide();
}
