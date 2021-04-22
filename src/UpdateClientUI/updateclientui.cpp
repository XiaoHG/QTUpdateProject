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
    :QDialog(parent)
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
    updateFiles = new QStringList;
//    QWidget *centerWidget = new QWidget(this);
    //this->setStyleSheet("background-color:rgb(100, 100, 100);border-radius: 10px;");
    this->setStyleSheet("background-color:rgb(100, 100, 100)");
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setStyleSheet("border-radius: 10px;");
    //Set update client dialog fix size
    this->resize(400, 300);
    this->setWindowTitle("Check Update");

    //line edit widget to varsion notify
    QFont vNotifyLabelFont( "Microsoft YaHei", 10, 75);
    titleLabel = new QLabel(this);
    titleLabel->setGeometry(0 , 0, this->width(), 40);
    titleLabel->setFont(vNotifyLabelFont);
    titleLabel->setText("Current Version V1.0.1");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setScaledContents(true);
    titleLabel->setStyleSheet("background-color:rgb(50, 50, 50);color:rgb(200, 200, 200)");

    //add a Text edit widget for output file that need to update
    //QFont outputEditFont( "Microsoft YaHei", 8, 75);
    outputEdit = new QTextEdit(this);
    //outputEdit->setFont(outputEditFont);
    //outputEdit->setEnabled(false);
    outputEdit->setFocusPolicy(Qt::NoFocus);
    //outputEdit->setWindowFlags(Qt::FramelessWindowHint);
    outputEdit->setText("This is a test");
    //outputEdit->setFrameShape(QFrame::NoFrame);
    outputEdit->setGeometry(20, titleLabel->height() + 40, this->width() - 40,
                            this->height() - titleLabel->height() - 100);
    outputEdit->setStyleSheet("background-color:rgb(100, 100, 100);color:rgb(200, 200, 200)");
    //outputEdit->setStyleSheet("border-radius: 0px;");
    outputEdit->setTextColor(QColor(200, 200, 200, 255));

    QFont logTitleLabelFont( "Microsoft YaHei", 8, 75);
    QLabel *logTitleLabel = new QLabel(this);
    logTitleLabel->setFont(logTitleLabelFont);
    logTitleLabel->setStyleSheet("color:white");
    logTitleLabel->setGeometry(outputEdit->x(), outputEdit->y() - 25, outputEdit->width(), 20);
    logTitleLabel->setScaledContents(true);
    logTitleLabel->setText("Varsion Log : ");
    logTitleLabel->setStyleSheet("color:rgb(200, 200, 200)");

    //splitter for update and cansel button and style
    btnUpdate = new QPushButton(this);
    btnUpdate->setText("UPDATE");
    btnUpdate->setGeometry(20, outputEdit->height() + titleLabel->height() + 50, 60, 30);
    btnUpdate->setStyleSheet("background-color:rgb(50, 50, 50);color:rgb(200, 200, 200)");

    //laster varsion info
    newVarsionInfoLabel = new QLabel(this);
    newVarsionInfoLabel->setGeometry(btnUpdate->x() + btnUpdate->width() + 10, btnUpdate->y(),
                                     outputEdit->width() - btnUpdate->width() - 10,
                                     btnUpdate->height());
    newVarsionInfoLabel->setScaledContents(true);
    newVarsionInfoLabel->setWordWrap(true);
    newVarsionInfoLabel->setStyleSheet("color:rgb(200, 200, 200)");
    newVarsionInfoLabel->setText("Varsion server have new varsion V2.0.1, "
                                 "click update button to update.");

    QPushButton *btnClose = new QPushButton(this);
    //btnClose->setText("X");
    btnClose->setGeometry(this->width() - titleLabel->height(), 0, titleLabel->height(), titleLabel->height());
    btnClose->setStyleSheet("background-color:rgb(150, 150, 150)");
    btnClose->setIcon(QIcon(":/image/close.png"));
    connect(btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));

    //update prosess timer
    updateProsessTimer = new QTimer(this);
    connect(updateProsessTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimeOut()));
    updateProsessTimer->stop();

    connect(btnUpdate, SIGNAL(clicked(bool)), this, SLOT(slotUpdateBtnClicked()));
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
        //need to update show update message, wait client clicked update button.
        //this->exec();
        outputEdit->clear();
        for(int i = 0; i < 20; ++i)
        {
            updateFiles->push_back(QString::asprintf("file number %d.file", i));
            outputEdit->append(QString::asprintf("file number %d.file", i));
        }
        this->exec();
    }
    else
    {
        //It is first star application or not.
        static int atTheOneStart = 1;
        if(atTheOneStart == 0)
            this->exec();
        atTheOneStart = 0;

        //This is the laster varsion so hide update button and cansel button,
        //and show the laster notify message and ok button.
        btnUpdate->setEnabled(false);
    }
    return isUpdate;
}

//just test code
void UpdateClientUI::testUpdate(bool isU)
{
    isUpdate = isU;
}

/*update function*/
void UpdateClientUI::slotUpdateBtnClicked()
{
    //update,and start updateProsessTimer
    outputEdit->clear();
    updateProsessTimer->start(100);
    update();
}

bool UpdateClientUI::update()
{
    //start update prosess timer at the begining update.
    this->setWindowTitle("Updating...");
    return true;
}

void UpdateClientUI::slotUpdateTimeOut()
{
    static int process = 0;
    outputEdit->append(updateFiles->at(process++));
    if(process == updateFiles->size() - 1)
    {
        QMessageBox::information(this, "Update Finish", "Finish", QMessageBox::Ok);
        this->close();
        sigCloseMainWindow();
        updateProsessTimer->stop();
    }
}
