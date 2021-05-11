#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QStorageInfo>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>

// 设置按钮样式及悬浮、按下时的状态
//ui->pushButton->setStyleSheet("QPushButton{background-color: rgb(225, 225, 225);border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;}"
//                                          "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
//                                          "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");

typedef void (MainWindow::*pfShowUI)(bool);
static QMap<EWHICHPAGE, pfShowUI> g_pfShowUIMap;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 设置按钮样式及悬浮、按下时的状态
    //debug
    this->setStyleSheet("QPushButton{border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;}"
                        "QPushButton:hover{background-color:rgba(229, 241, 251, 100); color: black;}"
                        "QPushButton:pressed{background-color:rgba(204, 228, 247, 200);border-style: inset;}");

    //release
//    this->setStyleSheet("QPushButton{border-radius:10px;border-style: outset;}"
//                        "QPushButton:hover{background-color:rgba(229, 241, 251, 100); color: black;}"
//                        "QPushButton:pressed{background-color:rgba(204, 228, 247, 200);border-style: inset;}");


    m_btnBack = new QPushButton(this);
    m_btnBack->setGeometry(this->width() - m_btnBack->width() + 20, 0, 80, 45);
    //connect(m_btnBack, SIGNAL(clicked(bool)), this, SLOT(slotMainUI()));
    connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(MAIN);});
    m_btnBack->setVisible(false);

    MainUI();
    FileListUI();
    SystemUI();
    ToolPage_1UI();
    ToolPage_2UI();
    LanguageUI();

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/1.png")));
    this->setPalette(palette);

    g_pfShowUIMap.insert(MAIN, &MainWindow::ShowMainUI);
    g_pfShowUIMap.insert(FILELIST, &MainWindow::ShowFileListUI);
    g_pfShowUIMap.insert(SYSTEM, &MainWindow::ShowSystemUI);
    g_pfShowUIMap.insert(TOOLPAGE_1, &MainWindow::ShowToolPage_1UI);
    g_pfShowUIMap.insert(TOOLPAGE_2, &MainWindow::ShowToolPage_2UI);
    g_pfShowUIMap.insert(LANGUAGE, &MainWindow::ShowLanguageUI);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Read()
{
    qDebug() << "----------start----------";
    QString UDiskPath = "";
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes())
    {
        qDebug() << storage.rootPath();
        if(storage.isValid() && storage.isReady())
        {
            UDiskPath = storage.rootPath();
            if(UDiskPath.contains("media"))
            {
                qDebug() << "qstring path:" << UDiskPath;
            }
        }
    }
    ListFile(UDiskPath);
}

void MainWindow::ListFile(QString name)
{
    QDir dir(name);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i < folder_list.size(); ++i)
        ListFile(folder_list.at(i).filePath());

    for(int i = 0; i < file_list.size(); ++i)
        qDebug() << file_list.at(i).fileName();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //获得鼠标的初始位置
        m_mouseStartPoint = event->globalPos();
        //mouseStartPoint = event->pos();
        //获得窗口的初始位置
        m_windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        //获得鼠标移动的距离
        QPoint distance = event->globalPos() - m_mouseStartPoint;
        //QPoint distance = event->pos() - mouseStartPoint;
        //改变窗口的位置
        this->move(m_windowTopLeftPoint + distance);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

void MainWindow::MainUI()
{
    m_btnPrint = new QPushButton(tr("m_btnPrint"), this);
    m_btnPrint->setGeometry(29, 80, 120, 121);
    //connect(m_btnPrint, SIGNAL(clicked(bool)), this, SLOT(slotFileListUI()));
    connect(m_btnPrint, &QPushButton::clicked, this, [=](){WhichUI(FILELIST);});

    m_btnSystem = new QPushButton(tr("m_btnSystem"), this);
    m_btnSystem->setGeometry(m_btnPrint->x() + m_btnPrint->width() + 31, 80, 120, 121);
    //connect(m_btnSystem, SIGNAL(clicked(bool)), this, SLOT(slotSystemUI()));
    connect(m_btnSystem, &QPushButton::clicked, this, [=](){WhichUI(SYSTEM);});

    m_btnTool = new QPushButton(tr("m_btnTool"), this);
    m_btnTool->setGeometry(m_btnSystem->x() + m_btnSystem->width() + 31, 80, 120, 121);
    //connect(m_btnTool, SIGNAL(clicked(bool)), this, SLOT(slotToolPage_1UI()));
    connect(m_btnTool, &QPushButton::clicked, this, [=](){WhichUI(TOOLPAGE_1);});

    m_mainUIList.push_back(m_btnPrint);
    m_mainUIList.push_back(m_btnSystem);
    m_mainUIList.push_back(m_btnTool);

    //All main widget is visible
}

void MainWindow::FileListUI()
{
    m_btnResFile = new QPushButton(tr("m_btnResFile"), this);
    m_btnResFile->setGeometry(this->width() - m_btnResFile->width() - 17, 57, 93, 60);

    m_btnUp = new QPushButton(tr("m_btnUp"), this);
    m_btnUp->setGeometry(m_btnResFile->x(), m_btnResFile->y() + m_btnResFile->height() + 13,
                         m_btnResFile->width(), m_btnResFile->height() - 13);

    m_btnDown = new QPushButton(tr("m_btnDown"), this);
    m_btnDown->setGeometry(m_btnUp->x(), m_btnUp->y() + m_btnUp->height() + 13,
                           m_btnUp->width(), m_btnUp->height());

    m_btnUDiskFile = new QPushButton(tr("m_btnUDiskFile"), this);
    m_btnUDiskFile->setGeometry(m_btnDown->x(), m_btnDown->y() + m_btnDown->height() + 12,
                           m_btnResFile->width(), m_btnResFile->height());

    QLabel *printFileLabel[4];
    QLineEdit *printFileNameLE[4];
    QVBoxLayout *vbLayout[4];
    for(int i = 0; i < 4; ++i)
    {
        m_widgets[i] = new QWidget(this);
        m_widgets[i]->setStyleSheet("background-color:rbga(100, 100, 100, 100);"
                                    "border-radius:10px");

        printFileLabel[i] = new QLabel(m_widgets[i]);

        printFileNameLE[i] = new QLineEdit(m_widgets[i]);
        printFileNameLE[i]->setAlignment(Qt::AlignHCenter);

        vbLayout[i] = new QVBoxLayout(m_widgets[i]);
        vbLayout[i]->addWidget(printFileLabel[i]);
        vbLayout[i]->addWidget(printFileNameLE[i]);

        m_widgets[i]->setLayout(vbLayout[i]);

    }
    m_widgets[0]->setGeometry(25, 58, 158, 120);
    m_widgets[1]->setGeometry(25 + m_widgets[1]->width() + 70, 58, 158, 120);
    m_widgets[2]->setGeometry(25, 58 + m_widgets[1]->height() + 12, 158, 120);
    m_widgets[3]->setGeometry(m_widgets[1]->x(), m_widgets[2]->y(), 158, 120);

    QImage *img = new QImage();
    img->load(":/anycubic_UI_png/2.png");
    QList<QLabel*> labelList = m_widgets[0]->findChildren<QLabel*>();
    labelList.at(0)->setPixmap(QPixmap::fromImage(*img));
    labelList.at(0)->setScaledContents(true);

    for(int i = 0; i < 4; ++i)
    {
        QList<QLabel*> labelList = m_widgets[i]->findChildren<QLabel*>();
        labelList.at(0)->setPixmap(QPixmap::fromImage(*img));
        labelList.at(0)->setScaledContents(true);

        QList<QLineEdit*> lineEdList = m_widgets[i]->findChildren<QLineEdit*>();
        lineEdList.at(0)->setText(tr("File %1").arg(i));
    }

    m_fileUIList.push_back(m_btnResFile);
    m_fileUIList.push_back(m_btnUp);
    m_fileUIList.push_back(m_btnDown);
    m_fileUIList.push_back(m_btnUDiskFile);
    for(int i = 0; i < 4; ++i)
    {
        m_fileUIList.push_back(m_widgets[i]);
    }

    //Init set false
    ShowFileListUI(false);
}

void MainWindow::SystemUI()
{
    m_btnLanguage = new QPushButton(tr("m_btnLanguage"), this);
    m_btnLanguage->setGeometry(24, 61, 209, 90);
    //set property for use
    m_btnLanguage->setProperty("Language", LANGUAGE);
    //connect(m_btnLanguage, SIGNAL(clicked(bool)), this, SLOT(slotWhichUI()));
    //connect(m_btnLanguage, SIGNAL(clicked(bool)), this, SLOT(slotLanguageUI()));
    connect(m_btnLanguage, &QPushButton::clicked, this, [=](){WhichUI(LANGUAGE);});

    m_btnWeb = new QPushButton(tr("m_btnWeb"), this);
    m_btnWeb->setGeometry(m_btnLanguage->x() + m_btnLanguage->width() + 14, m_btnLanguage->y(),
                         m_btnLanguage->width(), m_btnLanguage->height());

    m_btnVersion = new QPushButton(tr("m_btnVersion"), this);
    m_btnVersion->setGeometry(m_btnLanguage->x(), m_btnLanguage->y() + m_btnLanguage->height() + 10,
                           m_btnLanguage->width(), m_btnLanguage->height());

    m_btnSet = new QPushButton(tr("m_btnSet"), this);
    m_btnSet->setGeometry(m_btnWeb->x(), m_btnVersion->y(),
                           m_btnLanguage->width(), m_btnLanguage->height());

    m_btnPrePage = new QPushButton(tr("m_btnPrePage"), this);
    m_btnPrePage->setGeometry(m_btnLanguage->x(), m_btnVersion->y() + m_btnVersion->height() + 16,
                          m_btnLanguage->width() - 63, m_btnLanguage->height() / 2 - 5);

    m_btnNextPage = new QPushButton(tr("m_btnPrePage"), this);
    m_btnNextPage->setGeometry(m_btnLanguage->x() + m_btnPrePage->width() + 140, m_btnPrePage->y(),
                              m_btnPrePage->width(), m_btnPrePage->height());

    m_systemUIList.push_back(m_btnLanguage);
    m_systemUIList.push_back(m_btnWeb);
    m_systemUIList.push_back(m_btnVersion);
    m_systemUIList.push_back(m_btnSet);
    m_systemUIList.push_back(m_btnPrePage);
    m_systemUIList.push_back(m_btnNextPage);

    //Init set false
    ShowSystemUI(false);
}

void MainWindow::ToolPage_1UI()
{
    QPushButton *btnMoveZ = new QPushButton(tr("btnMoveZ"), this);
    btnMoveZ->setGeometry(24, 61, 209, 90);

    QPushButton *btnDetectResin = new QPushButton(tr("btnDetectResin"), this);
    btnDetectResin->setGeometry(btnMoveZ->x() + btnMoveZ->width() + 14, btnMoveZ->y(),
                         btnMoveZ->width(), btnMoveZ->height());

    QPushButton *btnDetectLight = new QPushButton(tr("btnDetectLight"), this);
    btnDetectLight->setGeometry(btnMoveZ->x(), btnMoveZ->y() + btnMoveZ->height() + 10,
                           btnMoveZ->width(), btnMoveZ->height());

    QPushButton *btnConWeb = new QPushButton(tr("btnConWeb"), this);
    btnConWeb->setGeometry(btnDetectResin->x(), btnDetectLight->y(),
                           btnMoveZ->width(), btnMoveZ->height());

    QPushButton *btnNextPage = new QPushButton(tr("btnNextPage"), this);
    btnNextPage->setGeometry(m_btnLanguage->x() + m_btnPrePage->width() + 140, m_btnPrePage->y(),
                              m_btnPrePage->width(), m_btnPrePage->height());
    //connect(btnNextPage, SIGNAL(clicked(bool)), this, SLOT(slotToolPage_2UI()));
    connect(btnNextPage, &QPushButton::clicked, this, [=](){WhichUI(TOOLPAGE_2);});

    m_toolPage_1UIList.push_back(btnMoveZ);
    m_toolPage_1UIList.push_back(btnDetectResin);
    m_toolPage_1UIList.push_back(btnDetectLight);
    m_toolPage_1UIList.push_back(btnConWeb);
    m_toolPage_1UIList.push_back(btnNextPage);

    //Init set false
    ShowToolPage_1UI(false);
}

void MainWindow::ToolPage_2UI()
{
    QPushButton *btnPowerSet = new QPushButton(tr("btnPowerSet"), this);
    btnPowerSet->setGeometry(24, 61, 209, 90);

    QPushButton *btnInnTest = new QPushButton(tr("btnInnTest"), this);
    btnInnTest->setGeometry(btnPowerSet->x() + btnPowerSet->width() + 14, btnPowerSet->y(),
                         btnPowerSet->width(), btnPowerSet->height());

    QPushButton *btnUnused_2 = new QPushButton(tr("btnUnused_2"), this);
    btnUnused_2->setGeometry(btnPowerSet->x(), btnPowerSet->y() + btnPowerSet->height() + 10,
                           btnPowerSet->width(), btnPowerSet->height());

    QPushButton *btnUnused_1 = new QPushButton(tr("btnUnused_1"), this);
    btnUnused_1->setGeometry(btnInnTest->x(), btnUnused_2->y(),
                           btnPowerSet->width(), btnPowerSet->height());

    QPushButton *btnPrePage = new QPushButton(tr("btnPrePage"), this);
    btnPrePage->setGeometry(m_btnPrePage->x(), m_btnPrePage->y(),
                              m_btnPrePage->width(), m_btnPrePage->height());
    //connect(btnPrePage, SIGNAL(clicked(bool)), this, SLOT(slotToolPage_1UI()));
    connect(btnPrePage, &QPushButton::clicked, this, [=](){WhichUI(TOOLPAGE_1);});

    m_toolPage_2UIList.push_back(btnPowerSet);
    m_toolPage_2UIList.push_back(btnInnTest);
    m_toolPage_2UIList.push_back(btnUnused_2);
    m_toolPage_2UIList.push_back(btnUnused_1);
    m_toolPage_2UIList.push_back(btnPrePage);

    //Init set false
    ShowToolPage_2UI(false);
}

void MainWindow::LanguageUI()
{
    QRadioButton *radioChiese = new QRadioButton(tr("Check"), this);
    radioChiese->setGeometry(this->width() - 110, 66, 100, 30);

    QRadioButton *radioEnglish = new QRadioButton(tr("Check"), this);
    radioEnglish->setGeometry(radioChiese->x(), radioChiese->y() + radioChiese->height() + 20,
                           radioChiese->width(), radioChiese->height());

    m_languageUIList.push_back(radioChiese);
    m_languageUIList.push_back(radioEnglish);

    //Init set false
    ShowLanguageUI(false);
}

void MainWindow::ShowMainUI(bool visible)
{
    for(int i = 0; i < m_mainUIList.size(); ++i)
    {
        m_mainUIList.at(i)->setVisible(visible);
    }
}

void MainWindow::ShowFileListUI(bool visible)
{
    for(int i = 0; i < m_fileUIList.size(); ++i)
    {
        m_fileUIList.at(i)->setVisible(visible);
    }
}

void MainWindow::ShowSystemUI(bool visible)
{
    for(int i = 0; i < m_systemUIList.size(); ++i)
    {
        m_systemUIList.at(i)->setVisible(visible);
    }
}

void MainWindow::ShowToolPage_1UI(bool visible)
{
    for(int i = 0; i < m_toolPage_1UIList.size(); ++i)
    {
        m_toolPage_1UIList.at(i)->setVisible(visible);
    }
}

void MainWindow::ShowToolPage_2UI(bool visible)
{
    for(int i = 0; i < m_toolPage_2UIList.size(); ++i)
    {
        m_toolPage_2UIList.at(i)->setVisible(visible);
    }
}

void MainWindow::ShowLanguageUI(bool visible)
{
    for(int i = 0; i < m_languageUIList.size(); ++i)
    {
        m_languageUIList.at(i)->setVisible(visible);
    }
}

void MainWindow::WhichUI(const EWHICHPAGE which)
{
    QPalette palette;
    m_btnBack->setVisible(true);
    qDebug() << "which = " << which;
    switch (which) {
    case MAIN:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/1.png")));
        m_btnBack->setVisible(false);
        break;
    case FILELIST:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/2.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(MAIN);});
        break;
    case SYSTEM:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/3.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(MAIN);});
        break;
    case TOOLPAGE_1:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/4.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(MAIN);});
        break;
    case TOOLPAGE_2:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/5.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(MAIN);});
        break;
    case LANGUAGE:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/7.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(SYSTEM);});
        break;
    default:
        break;
    }

    this->setPalette(palette);

    QMap<EWHICHPAGE, pfShowUI>::iterator itr = g_pfShowUIMap.begin();
    for(; itr != g_pfShowUIMap.end(); ++itr)
    {
        if(itr.key() == which)
        {
            (this->*(itr.value()))(true);
        }
        else
        {
            (this->*(itr.value()))(false);
        }
    }
}

void MainWindow::slotWhichUI()
{
    QPushButton* btn = (QPushButton*)sender();
    EWHICHPAGE which;
    QPalette palette;
    m_btnBack->setVisible(true);
    qDebug() << "which = " << which;
    switch (which) {
    case MAIN:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/1.png")));
        m_btnBack->setVisible(false);
        break;
    case FILELIST:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/2.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(MAIN);});
        break;
    case SYSTEM:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/3.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(MAIN);});
        break;
    case TOOLPAGE_1:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/4.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(MAIN);});
        break;
    case TOOLPAGE_2:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/5.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(MAIN);});
        break;
    case LANGUAGE:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/7.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(SYSTEM);});
        break;
    default:
        break;
    }

    this->setPalette(palette);

    QMap<EWHICHPAGE, pfShowUI>::iterator itr = g_pfShowUIMap.begin();
    for(; itr != g_pfShowUIMap.end(); ++itr)
    {
        if(itr.key() == which)
        {
            (this->*(itr.value()))(true);
        }
        else
        {
            (this->*(itr.value()))(false);
        }
    }
}

