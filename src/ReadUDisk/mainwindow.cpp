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
#include <QButtonGroup>

//info: 设置objectName用于遍历找到对应控件并设置对应值

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
    NetInfoUI();
    VersionInfoUI();
    SystemSetUI();
    MoveZUI();
    DetectResinUI();
    DetectLightUI();

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/1.png")));
    this->setPalette(palette);

    g_pfShowUIMap.insert(MAIN, &MainWindow::ShowMainUI);
    g_pfShowUIMap.insert(FILELIST, &MainWindow::ShowFileListUI);
    g_pfShowUIMap.insert(SYSTEM, &MainWindow::ShowSystemUI);
    g_pfShowUIMap.insert(TOOLPAGE_1, &MainWindow::ShowToolPage_1UI);
    g_pfShowUIMap.insert(TOOLPAGE_2, &MainWindow::ShowToolPage_2UI);
    g_pfShowUIMap.insert(LANGUAGE, &MainWindow::ShowLanguageUI);
    g_pfShowUIMap.insert(NETINFO, &MainWindow::ShowNetInfoUI);
    g_pfShowUIMap.insert(VERSIONINFO, &MainWindow::ShowVersionInfoUI);
    g_pfShowUIMap.insert(SYSTEMSET, &MainWindow::ShowSystemSetUI);
    g_pfShowUIMap.insert(MOVEZ, &MainWindow::ShowMoveZUI);
    g_pfShowUIMap.insert(DETECTRESIN_1, &MainWindow::ShowDetectResinUI);
    g_pfShowUIMap.insert(DETECTLIGHT, &MainWindow::ShowDetectLightUI);
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

    m_btnNetInfo = new QPushButton(tr("m_btnNetInfo"), this);
    m_btnNetInfo->setGeometry(m_btnLanguage->x() + m_btnLanguage->width() + 14, m_btnLanguage->y(),
                         m_btnLanguage->width(), m_btnLanguage->height());
    connect(m_btnNetInfo, &QPushButton::clicked, this, [=](){WhichUI(NETINFO);});

    m_btnVersion = new QPushButton(tr("m_btnVersion"), this);
    m_btnVersion->setGeometry(m_btnLanguage->x(), m_btnLanguage->y() + m_btnLanguage->height() + 10,
                           m_btnLanguage->width(), m_btnLanguage->height());
    connect(m_btnVersion, &QPushButton::clicked, this, [=](){WhichUI(VERSIONINFO);});

    m_btnSet = new QPushButton(tr("m_btnSet"), this);
    m_btnSet->setGeometry(m_btnNetInfo->x(), m_btnVersion->y(),
                           m_btnLanguage->width(), m_btnLanguage->height());
    connect(m_btnSet, &QPushButton::clicked, this, [=](){WhichUI(SYSTEMSET);});

    m_btnPrePage = new QPushButton(tr("m_btnPrePage"), this);
    m_btnPrePage->setGeometry(m_btnLanguage->x(), m_btnVersion->y() + m_btnVersion->height() + 16,
                          m_btnLanguage->width() - 63, m_btnLanguage->height() / 2 - 5);

    m_btnNextPage = new QPushButton(tr("m_btnPrePage"), this);
    m_btnNextPage->setGeometry(m_btnLanguage->x() + m_btnPrePage->width() + 140, m_btnPrePage->y(),
                              m_btnPrePage->width(), m_btnPrePage->height());

    m_systemUIList.push_back(m_btnLanguage);
    m_systemUIList.push_back(m_btnNetInfo);
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
    connect(btnMoveZ, &QPushButton::clicked, this, [=](){WhichUI(MOVEZ);});

    QPushButton *btnDetectResin = new QPushButton(tr("btnDetectResin"), this);
    btnDetectResin->setGeometry(btnMoveZ->x() + btnMoveZ->width() + 14, btnMoveZ->y(),
                         btnMoveZ->width(), btnMoveZ->height());
    connect(btnDetectResin, &QPushButton::clicked, this, [=](){WhichUI(DETECTRESIN_1);});

    QPushButton *btnDetectLight = new QPushButton(tr("btnDetectLight"), this);
    btnDetectLight->setGeometry(btnMoveZ->x(), btnMoveZ->y() + btnMoveZ->height() + 10,
                           btnMoveZ->width(), btnMoveZ->height());
    connect(btnDetectLight, &QPushButton::clicked, this, [=](){WhichUI(DETECTLIGHT);});

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

void MainWindow::NetInfoUI()
{
    QLineEdit *leSSID = new QLineEdit(tr("leSSID"), this);
    leSSID->setGeometry(140, 66, 310, 30);

    QLineEdit *leIP = new QLineEdit(tr("leIP"), this);
    leIP->setGeometry(leSSID->x(), leSSID->y() + leSSID->height() + 20,
                           leSSID->width(), leSSID->height());

    QLineEdit *leNetStatus = new QLineEdit(tr("leNetStatus"), this);
    leNetStatus->setGeometry(leSSID->x() + 110, leIP->y() + leIP->height() + 20,
                                leSSID->width() - 110, leSSID->height());


    m_netInfoUIList.push_back(leSSID);
    m_netInfoUIList.push_back(leIP);
    m_netInfoUIList.push_back(leNetStatus);

    for(int i = 0; i < m_netInfoUIList.size(); ++i)
    {
//        m_netInfoUIList.at(i)->setStyleSheet("QLineEdit{background-color:transparent}"
//                                           "QLineEdit{border-width:0;border-style:outset}");
    }

    //Init set false
    ShowNetInfoUI(false);
}

void MainWindow::VersionInfoUI()
{
    QLineEdit *leDevName = new QLineEdit(tr("leDevName"), this);
    leDevName->setObjectName("DevName");//set object name for used in choose widget
    leDevName->setGeometry(187, 66, 263, 30);

    QLineEdit *leSliderVersion = new QLineEdit(tr("leSliderVersion"), this);
    leSliderVersion->setGeometry(leDevName->x(), leDevName->y() + leDevName->height() + 20,
                                    leDevName->width(), leDevName->height());

    QLineEdit *leUIVersion = new QLineEdit(tr("leUIVersion"), this);
    leUIVersion->setGeometry(leSliderVersion->x(), leSliderVersion->y() + leSliderVersion->height() + 20,
                                    leSliderVersion->width(), leSliderVersion->height());

    QLineEdit *leCN = new QLineEdit(tr("leCN"), this);
    leCN->setGeometry(leUIVersion->x() - 68, leUIVersion->y() + leUIVersion->height() + 20,
                                leUIVersion->width() + 68, leUIVersion->height());


    m_versionInfoUIList.push_back(leDevName);
    m_versionInfoUIList.push_back(leSliderVersion);
    m_versionInfoUIList.push_back(leUIVersion);
    m_versionInfoUIList.push_back(leCN);

    for(int i = 0; i < m_versionInfoUIList.size(); ++i)
    {
//        m_versionInfoUIList.at(i)->setStyleSheet("QLineEdit{background-color:transparent}"
//                                           "QLineEdit{border-width:0;border-style:outset}");
    }

    //Set version by object name
//    for(int i = 0; i < m_versionInfoUIList.size(); ++i)
//    {
//        if(m_versionInfoUIList.at(i)->objectName() == "DevName")
//        {
//            QLineEdit *tmp = static_cast<QLineEdit*>(m_versionInfoUIList.at(i));
//            tmp->setText("11111111111111111111");
//        }
//    }

    //Init set false
    ShowVersionInfoUI(false);
}

void MainWindow::SystemSetUI()
{
    QCheckBox *chbSound = new QCheckBox(tr("Check"), this);
    chbSound->setGeometry(this->width() - 110, 66, 100, 30);

    QCheckBox *chkDetectGate = new QCheckBox(tr("Check"), this);
    chkDetectGate->setGeometry(chbSound->x(), chbSound->y() + chbSound->height() + 20,
                                chbSound->width(), chbSound->height());

    m_systemSetUIList.push_back(chbSound);
    m_systemSetUIList.push_back(chkDetectGate);

    //Init set false
    ShowSystemSetUI(false);
}

void MainWindow::MoveZUI()
{
    //pushbutton 的按下效果没有显示
    QButtonGroup *box = new QButtonGroup;
    box->setExclusive(true);
    QPushButton *btn0_1mm = new QPushButton(tr("btn0_1mm"), this);
    btn0_1mm->setGeometry(40, 80, 125, 50);
    btn0_1mm->setCheckable(true);
    box->addButton(btn0_1mm);

    QPushButton *btn1mm = new QPushButton(tr("btn1mm"), this);
    btn1mm->setGeometry(btn0_1mm->x() + btn0_1mm->width() + 12, btn0_1mm->y(),
                         btn0_1mm->width(), btn0_1mm->height());
    btn1mm->setCheckable(true);
    box->addButton(btn1mm);

    QPushButton *btn10mm = new QPushButton(tr("btn10mm"), this);
    btn10mm->setGeometry(btn1mm->x() + btn1mm->width() + 12, btn1mm->y(),
                           btn1mm->width(), btn1mm->height());
    btn10mm->setCheckable(true);
    box->addButton(btn10mm);

    QPushButton *btnUp = new QPushButton(tr("btnUp"), this);
    btnUp->setGeometry(24, 161, 113, 65);
    connect(btnUp, SIGNAL(clicked(bool)), this, SLOT(slotTest()));//test code

    QPushButton *btnDown = new QPushButton(tr("btnDown"), this);
    btnDown->setGeometry(btnUp->x() + btnUp->width() + 47, btnUp->y(),
                         btnUp->width(), btnUp->height());

    QPushButton *btnNon = new QPushButton(tr("btnNon"), this);
    btnNon->setGeometry(btnUp->x(), btnUp->y() + btnUp->height() + 14,
                         btnUp->width(), btnUp->height());

    QPushButton *btnZ0 = new QPushButton(tr("btnZ0"), this);
    btnZ0->setGeometry(btnDown->x(), btnNon->y(),
                         btnUp->width(), btnUp->height());

    QPushButton *btnHome = new QPushButton(tr("btnHome"), this);
    btnHome->setGeometry(btnDown->x() + btnDown->width() + 47, btnDown->y(),
                         btnUp->width(), btnUp->height() * 2 + 14);
    connect(btnHome, &QPushButton::clicked, this, [=](){WhichUI(MAIN);});

    m_moveZUIList.push_back(btn0_1mm);
    m_moveZUIList.push_back(btn1mm);
    m_moveZUIList.push_back(btn10mm);
    m_moveZUIList.push_back(btnUp);
    m_moveZUIList.push_back(btnDown);
    m_moveZUIList.push_back(btnNon);
    m_moveZUIList.push_back(btnZ0);
    m_moveZUIList.push_back(btnHome);

    //Init set false
    ShowMoveZUI(false);
}

void MainWindow::DetectResinUI()
{
    QRadioButton *radioChiese = new QRadioButton(tr("Check"), this);
    radioChiese->setGeometry(this->width() - 110, 66, 100, 30);

    QRadioButton *radioEnglish = new QRadioButton(tr("Check"), this);
    radioEnglish->setGeometry(radioChiese->x(), radioChiese->y() + radioChiese->height() + 20,
                           radioChiese->width(), radioChiese->height());

    QPushButton *btnDetect = new QPushButton(tr("btnDetect"), this);
    btnDetect->setGeometry(m_btnLanguage->x() + m_btnPrePage->width() + 140, m_btnPrePage->y(),
                              m_btnPrePage->width(), m_btnPrePage->height());

    m_detectResinUIList.push_back(radioChiese);
    m_detectResinUIList.push_back(radioEnglish);
    m_detectResinUIList.push_back(btnDetect);

    //Init set false
    ShowDetectResinUI(false);
}

void MainWindow::DetectLightUI()
{
    QLineEdit *leSec = new QLineEdit(tr("leSec"), this);
    //leSec->setGeometry(this->width() - leSec->width() - 17, 57, 93, 60);
    leSec->setGeometry(this->width() - leSec->width() - 10, 72, 60, 30);
    leSec->setAlignment(Qt::AlignRight);

    QPushButton *btnUp = new QPushButton(tr("btnUp"), this);
    btnUp->setGeometry(this->width() - btnUp->width() - 15, 130, 92, 46);

    QPushButton *btnDown = new QPushButton(tr("btnDown"), this);
    btnDown->setGeometry(btnUp->x(), btnUp->y() + btnUp->height() + 14,
                           btnUp->width(), btnUp->height());

    QPushButton *btnNext = new QPushButton(tr("btnNext"), this);
    btnNext->setGeometry(btnUp->x(), btnDown->y() + btnDown->height() + 12,
                           btnUp->width(), btnUp->height() + 15);


    m_detectLightUIList.push_back(leSec);
    m_detectLightUIList.push_back(btnUp);
    m_detectLightUIList.push_back(btnDown);
    m_detectLightUIList.push_back(btnNext);

    //Init set false
    ShowDetectLightUI(false);
}

void MainWindow::slotTest()
{
    QPushButton *tmp;
    for(int i = 0; i < m_moveZUIList.size(); ++i)
    {
        tmp = static_cast<QPushButton*>(m_moveZUIList.at(i));
        if(tmp->text() == "btn0_1mm" || tmp->text() == "btn1mm" || tmp->text() == "btn10mm")
            qDebug() << tmp->text() << " :" << tmp->isChecked();
    }
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

void MainWindow::ShowNetInfoUI(bool visible)
{
    for(int i = 0; i < m_netInfoUIList.size(); ++i)
    {
        m_netInfoUIList.at(i)->setVisible(visible);
    }
}

void MainWindow::ShowVersionInfoUI(bool visible)
{
    for(int i = 0; i < m_versionInfoUIList.size(); ++i)
    {
        m_versionInfoUIList.at(i)->setVisible(visible);
    }
}

void MainWindow::ShowSystemSetUI(bool visible)
{
    for(int i = 0; i < m_systemSetUIList.size(); ++i)
    {
        m_systemSetUIList.at(i)->setVisible(visible);
    }
}

void MainWindow::ShowMoveZUI(bool visible)
{
    for(int i = 0; i < m_moveZUIList.size(); ++i)
    {
        m_moveZUIList.at(i)->setVisible(visible);
    }
}

void MainWindow::ShowDetectResinUI(bool visible)
{
    for(int i = 0; i < m_detectResinUIList.size(); ++i)
    {
        m_detectResinUIList.at(i)->setVisible(visible);
    }
}

void MainWindow::ShowDetectLightUI(bool visible)
{
    for(int i = 0; i < m_detectLightUIList.size(); ++i)
    {
        m_detectLightUIList.at(i)->setVisible(visible);
    }
}

void MainWindow::WhichUI(const EWHICHPAGE which)
{
    QPalette palette;
    m_btnBack->setVisible(true);
    m_btnBack->disconnect();
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
    case NETINFO:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/8.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(SYSTEM);});
        break;
    case VERSIONINFO:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/9.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(SYSTEM);});
        break;
    case SYSTEMSET:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/10.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(SYSTEM);});
        break;
    case MOVEZ:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/11.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(TOOLPAGE_1);});
        break;
    case DETECTRESIN_1:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/12.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(TOOLPAGE_1);});
        break;
    case DETECTLIGHT:
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/anycubic_UI_png/13.png")));
        connect(m_btnBack, &QPushButton::clicked, this, [=](){WhichUI(TOOLPAGE_1);});
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


