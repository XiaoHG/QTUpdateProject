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
    //QWidget *centerWidget = new QWidget(this);
    this->setStyleSheet("background-color:rgb(100, 100, 100)");
    //this->setStyleSheet("border-radius: 15px;");
    //Set update client dialog fix size
    this->setFixedSize(600, 400);

    //add a Text edit widget for output file that need to update
    outputEdit = new QTextEdit(this);
    //outputEdit->setEnabled(false);
    outputEdit->setFocusPolicy(Qt::NoFocus);
    //outputEdit->setWindowFlags(Qt::FramelessWindowHint);
    outputEdit->setText("This is a test");
    //outputEdit->setFrameShape(QFrame::NoFrame);
    outputEdit->setGeometry(20, 20, 330, this->height() - 40);
    outputEdit->setStyleSheet("background-color:rgb(100, 100, 100)");
    //outputEdit->setStyleSheet("border-radius: 0px;");
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
    btnUpdate->setGeometry(370, this->height() - 100, 100, 50);
    btnUpdate->setWindowOpacity(0.5);
    btnUpdate->setStyleSheet("background-color:rgb(255, 255, 255)");

    btnCansel = new QPushButton(this);
    btnCansel->setText("CANSEL");
    btnCansel->setFixedSize(100, 50);
    btnCansel->setGeometry(480, this->height() - 100, 100, 50);
    btnCansel->setStyleSheet("background-color:rgb(255, 255, 255)");

    //ok
    btnOk = new QPushButton(this);
    btnOk->setText("OK");
    btnOk->setGeometry(btnUpdate->x() + 20, btnUpdate->y() - 30, btnUpdate->width() + 30, btnUpdate->height());
    btnOk->setStyleSheet("background-color:rgb(200, 200, 200);color:white");
    btnOk->hide();

    //QSlider for update prosess
    updateSlider = new QSlider(this);
    updateSlider->setValue(1);
    updateSlider->setOrientation(Qt::Horizontal);
    updateSlider->setGeometry(100, 100, 400, 30);
    updateSlider->setWindowFlags(Qt::WindowStaysOnTopHint);
    updateSlider->hide();

    //update prosess timer
    updateProsessTimer = new QTimer(this);
    connect(updateProsessTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimeOut()));
    updateProsessTimer->stop();

    connect(btnOk, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(btnUpdate, SIGNAL(clicked(bool)), this, SLOT(slotUpdateBtnClicked()));
    connect(btnCansel, SIGNAL(clicked(bool)), this, SLOT(slotClose()));

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
        this->exec();
        outputEdit->clear();
        for(int i = 0; i < 10; ++i)
        {
            outputEdit->append(QString::asprintf("file number %d.file", i));
        }
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

        this->setStyleSheet("border-radius: 15px;");
        this->setWindowFlags(Qt::FramelessWindowHint);
        btnUpdate->hide();
        btnCansel->hide();
        vNotifyLabel->show();
        btnOk->show();
    }
    return isUpdate;
}

void UpdateClientUI::testUpdate(bool isU)
{
    isUpdate = isU;
}

/*update function*/
void UpdateClientUI::slotUpdateBtnClicked()
{
    //update,and start updateProsessTimer
    outputEdit->hide();
    update();
}

bool UpdateClientUI::update()
{
    //start update prosess timer at the begining update.
    updateProsessTimer->start(10);
    updateSlider->show();
    return true;
}

void UpdateClientUI::slotClose()
{
    //next show update dialog show update information if need to update.
    if(isUpdate)
        outputEdit->clear();
    this->hide();
}

void UpdateClientUI::slotUpdateTimeOut()
{
    static int updateSliderValue = 1;
    updateSlider->setValue(updateSliderValue++);
    if(updateSlider->value() == updateSlider->maximum())
    {
        //add a slider
        updateSliderValue = 1;
        updateSlider->hide();
        int btnFlag = QMessageBox::information(this, "update success",
                                               "Please restart app for run the laster varsion",
                                               QMessageBox::Ok);
        if(btnFlag == QMessageBox::Ok)
        {
            //sent a message to main window for close it.
            //client need to restart the application
            this->close();
            sigCloseMainWindow();
        }
        isUpdate = false;
        updateProsessTimer->stop();
    }
}
