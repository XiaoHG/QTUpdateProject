#include "autoupdaterui.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>
#include <QMovie>
#include <QProcess>
#include <QTextCursor>
#include <QTextBlock>

//m_btnClose->setStyleSheet("QPushButton{background-color:rgba(150, 150, 150, 100%);\
//color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}" // 按键本色
//"QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
//"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");   // 鼠标按下的色彩

AutoUpdaterUI::AutoUpdaterUI(QWidget *parent)
    :QDialog(parent)
{
    InitUI();
    qDebug() << "AutoUpdaterUI 100";
    m_updater = new AutoUpdater();
    qDebug() << "AutoUpdaterUI 200";
    connect(m_updater, SIGNAL(sigDownloadUpdaterXmlOver()), this, SLOT(slotDownloadUpdaterXmlOver()));

    m_checkTimeOut = new QTimer(this);
    connect(m_checkTimeOut, SIGNAL(timeout()), this, SLOT(slotCheckTimeOut()));
    m_checkTimeOut->stop();
}

AutoUpdaterUI::~AutoUpdaterUI()
{
    if(m_updater)
    {
        m_updater->deleteLater();
    }
}

/*UI defined*/
void AutoUpdaterUI::InitUI()
{
    qDebug() << "AutoUpdaterUI 0";
    m_updateFiles = new QStringList;
    this->setStyleSheet("background-color:rgb(100, 100, 100)");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(400, 300);
    this->setWindowTitle("Check Update");

    //line edit widget to version notify
    QFont titleLabelFont( "Microsoft YaHei", 11, 75);
    m_titleLabel = new QLabel(this);
    m_titleLabel->setGeometry(0 , 0, this->width(), 40);
    m_titleLabel->setFont(titleLabelFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setScaledContents(true);
    m_titleLabel->setStyleSheet("background-color:rgb(50, 50, 50);"
                                "color:rgb(200, 200, 200)");

    //add a Text edit widget for output file that need to update
    //QFont outputEditFont( "Microsoft YaHei", 8, 75);
    m_outputVersionInfoEdit = new QTextEdit(this);
    m_outputVersionInfoEdit->setFocusPolicy(Qt::NoFocus);
    m_outputVersionInfoEdit->setWindowFlags(Qt::FramelessWindowHint);
    m_outputVersionInfoEdit->setText("This is a test");
    m_outputVersionInfoEdit->setGeometry(20, m_titleLabel->height() + 40, this->width() - 40,
                            this->height() - m_titleLabel->height() - 100);
    m_outputVersionInfoEdit->setStyleSheet("background-color:rgb(100, 100, 100);"
                                           "color:rgb(200, 200, 200)");
    m_outputVersionInfoEdit->setTextColor(QColor(200, 200, 200, 255));



    QFont logTitleLabelFont( "Microsoft YaHei", 9, 75);
    m_logTitleLabel = new QLabel(this);
    m_logTitleLabel->setFont(logTitleLabelFont);
    m_logTitleLabel->setStyleSheet("color:white");
    m_logTitleLabel->setGeometry(m_outputVersionInfoEdit->x(),
                                 m_outputVersionInfoEdit->y() - 25,
                                 m_outputVersionInfoEdit->width(),
                                 20);
    m_logTitleLabel->setScaledContents(true);
    m_logTitleLabel->setText(QStringLiteral("更新日志 : "));
    m_logTitleLabel->setStyleSheet("color:rgb(200, 200, 200)");

    qDebug() << "AutoUpdaterUI 1";

    qDebug() << "AutoUpdaterUI 2";

    m_btnClose = new QPushButton(this);
    m_btnClose->setIcon(QIcon(":/image/close.png"));
    m_btnClose->setGeometry(this->width() - m_titleLabel->height(), 0,
                          m_titleLabel->height(), m_titleLabel->height());
    m_btnClose->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%);"
                        "color: white; border-style: outset;}" // 按键本色
                        "QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
                        "QPushButton:pressed{background-color:rgb(85, 170, 255); "
                        "border-style: inset; }");   // 鼠标按下的色彩
    m_btnClose->setFlat(true);
    connect(m_btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));

    CheckUpdateUI();
    UpdateUI();
    UpdatingUI();
    FinishUpdateUI();
    NotUpdateUI();
}

void AutoUpdaterUI::CheckUpdateUI()
{
    QFont logTitleLabelFont( "Microsoft YaHei", 9, 75);

    QProgressBar *pbCheckUpdate = new QProgressBar(this);
    pbCheckUpdate->setGeometry((this->width() - pbCheckUpdate->width()) / 2 - 50,
                               (this->height() - pbCheckUpdate->height()) / 2,
                               200, 20);
    pbCheckUpdate->setOrientation(Qt::Horizontal);  // 水平方向
    pbCheckUpdate->setMinimum(0);  // 最小值
    pbCheckUpdate->setMaximum(0);  // 最大值， 进度条设置的最小值和最大值都为0，显示繁忙
    pbCheckUpdate->setStyleSheet("QProgressBar{"
                                   "font:9pt;"
                                   "border-radius:5px;"
                                   "text-align:center;"
                                   "border:1px solid #E8EDF2;"
                                   "background-color: rgb(255, 255, 255);"
                                   "border-color: rgb(180, 180, 180);}"
                                   "QProgressBar:chunk{"
                                   "border-radius:5px;"
                                   "background-color:#1ABC9C;}");

    m_timeLabel = new QLabel(this);
    m_timeLabel->setGeometry(pbCheckUpdate->x(), pbCheckUpdate->y() + 30, 100, 20);
    m_timeLabel->setFont(logTitleLabelFont);

    m_checkUpdateWidgets.push_back(pbCheckUpdate);
    m_checkUpdateWidgets.push_back(m_btnClose);
    m_checkUpdateWidgets.push_back(m_timeLabel);

    //init false
    ShowCheckUpdateUI(false);
}

void AutoUpdaterUI::UpdateUI()
{
    //splitter for update and cansel button and style
    //QFont btnUpdateFont( "Microsoft YaHei", 7, 50);
    m_btnUpdate = new QPushButton(this);
    m_btnUpdate->setText("UPDATE");
    m_btnUpdate->setIcon(QIcon("://image/update.png"));
    m_btnUpdate->setGeometry(20, m_outputVersionInfoEdit->height() + m_titleLabel->height() + 50, 70, 25);
    m_btnUpdate->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%);"
                        "color: white;   border-radius: 5; border-style: outset;}" // 按键本色
                        "QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
                        "QPushButton:pressed{background-color:rgb(85, 170, 255); "
                        "border-style: inset; }");   // 鼠标按下的色彩

    //laster version info
    m_newVersionInfoLabel = new QLabel(this);
    m_newVersionInfoLabel->setGeometry(m_btnUpdate->x() + m_btnUpdate->width() + 10,
                                       m_btnUpdate->y(),
                                       m_outputVersionInfoEdit->width() - m_btnUpdate->width() - 10,
                                       m_btnUpdate->height());
    m_newVersionInfoLabel->setScaledContents(true);
    m_newVersionInfoLabel->setWordWrap(true);
    m_newVersionInfoLabel->setStyleSheet("color:rgb(200, 200, 200)");

    connect(m_btnUpdate, SIGNAL(clicked(bool)), this, SLOT(slotBtnUpdateClicked()));

    m_updateWidgets.push_back(m_outputVersionInfoEdit);
    m_updateWidgets.push_back(m_logTitleLabel);
    m_updateWidgets.push_back(m_btnUpdate);
    m_updateWidgets.push_back(m_newVersionInfoLabel);
    m_updateWidgets.push_back(m_btnClose);

    //init false
    ShowUpdateUI(false);
}

void AutoUpdaterUI::UpdatingUI()
{
    m_updateProgressBar = new QProgressBar(this);
    m_updateProgressBar->setGeometry(m_outputVersionInfoEdit->x(), this->height() - 50,
                                       m_outputVersionInfoEdit->width(), 20);
    m_updateProgressBar->setStyleSheet("QProgressBar{"
                                       "font:9pt;"
                                       "border-radius:5px;"
                                       "text-align:center;"
                                       "border:1px solid #E8EDF2;"
                                       "background-color: rgb(255, 255, 255);"
                                       "border-color: rgb(180, 180, 180);}"
                                       "QProgressBar:chunk{"
                                       "border-radius:5px;"
                                       "background-color:#1ABC9C;}");

    //update prosess timer
    m_updateProsessTimer = new QTimer(this);
    connect(m_updateProsessTimer, SIGNAL(timeout()), this, SLOT(slotUpdateProcess()));
    m_updateProsessTimer->stop();

    m_updatingWidgets.push_back(m_outputVersionInfoEdit);
    m_updatingWidgets.push_back(m_logTitleLabel);
    m_updatingWidgets.push_back(m_updateProgressBar);

    //init false
    ShowUpdatingUI(false);
}

void AutoUpdaterUI::FinishUpdateUI()
{
    m_btnOk = new QPushButton(this);
    m_btnOk->setText("Reboot");
    m_btnOk->setGeometry(m_btnUpdate->x() + 200, m_btnUpdate->y(),
                         m_btnUpdate->width(), m_btnUpdate->height());
    m_btnOk->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%);"
                        "color: white;   border-radius: 5; border-style: outset;}" // 按键本色
                        "QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
                        "QPushButton:pressed{background-color:rgb(85, 170, 255); "
                        "border-style: inset; }");   // 鼠标按下的色彩

    m_btnCansel = new QPushButton(this);
    m_btnCansel->setText("Cancel");
    m_btnCansel->setGeometry(m_btnUpdate->x() + m_btnOk->width() + 210, m_btnUpdate->y(),
                             m_btnUpdate->width(), m_btnUpdate->height());
    m_btnCansel->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%);"
                        "color: white; border-radius: 5; border-style: outset;}" // 按键本色
                        "QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
                        "QPushButton:pressed{background-color:rgb(85, 170, 255); "
                        "border-style: inset; }");   // 鼠标按下的色彩

    connect(m_btnOk, SIGNAL(clicked(bool)), this, SLOT(slotBtnOkClicked()));
    connect(m_btnCansel, SIGNAL(clicked(bool)), this, SLOT(close()));

    m_finishWidgets.push_back(m_outputVersionInfoEdit);
    m_finishWidgets.push_back(m_logTitleLabel);
    m_finishWidgets.push_back(m_btnClose);
    m_finishWidgets.push_back(m_btnOk);
    m_finishWidgets.push_back(m_btnCansel);

    //init false
    ShowFinishUpdateUI(false);
}

void AutoUpdaterUI::NotUpdateUI()
{
    QFont labelLasterVersionFont( "Microsoft YaHei", 10, 75);
    m_labelLasterVersion = new QLabel(this);
    m_labelLasterVersion->setFont(labelLasterVersionFont);
    m_labelLasterVersion->setGeometry(m_btnUpdate->x() + 5,
                                      m_btnUpdate->y() - 10,
                                      m_outputVersionInfoEdit->width(),
                                      30);
    m_labelLasterVersion->setStyleSheet("color:rgb(150, 150, 150)");

    m_notUpdateWidgets.push_back(m_outputVersionInfoEdit);
    m_notUpdateWidgets.push_back(m_logTitleLabel);
    m_notUpdateWidgets.push_back(m_labelLasterVersion);
    m_notUpdateWidgets.push_back(m_btnClose);

    //init false
    ShowNotUpdateUI(false);
}

void AutoUpdaterUI::CheckUpdate()
{
    m_titleLabel->setText(QStringLiteral("正在检查更新 ..."));
    m_btnClose->setVisible(true);
    m_checkTimeOut->start(1000);
    m_checkupdateTimeOut = 0;
    m_timeLabel->setText(tr("PASS : 0s"));
}

void AutoUpdaterUI::Update()
{
    m_titleLabel->setText(QStringLiteral("检查更新！"));
    m_btnClose->setVisible(true);

    m_versionServerInfo = m_updater->GetVersion(QApplication::applicationDirPath() + "/download/updater.xml");
    m_newVersionInfoLabel->setText(QStringLiteral("检查到新版本 ") + m_versionServerInfo +
                                   QStringLiteral(" 点击更新！"));
}

void AutoUpdaterUI::Updating()
{
    m_outputVersionInfoEdit->clear();
    m_titleLabel->setText(QStringLiteral("正在更新 ..."));
}

void AutoUpdaterUI::FinishUpdate()
{
    //UpdateFinishUI();
    m_updateProsessTimer->stop();
}

void AutoUpdaterUI::NotUpdate()
{
    QString strCurrentVersion = m_updater->GetVersion(QApplication::applicationDirPath() + "/updater.xml");
    m_labelLasterVersion->setText(strCurrentVersion);
    m_titleLabel->setText(QStringLiteral("当前版本是最新版本！"));

    if(m_first)
    {
        return;
    }
    this->exec();
}

void AutoUpdaterUI::ShowCheckUpdateUI(bool visible)
{
    for(int i = 0; i < m_checkUpdateWidgets.size(); ++i)
    {
        m_checkUpdateWidgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::ShowUpdateUI(bool visible)
{
    for(int i = 0; i < m_updateWidgets.size(); ++i)
    {
        m_updateWidgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::ShowUpdatingUI(bool visible)
{
    for(int i = 0; i < m_updatingWidgets.size(); ++i)
    {
        m_updatingWidgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::ShowFinishUpdateUI(bool visible)
{
    for(int i = 0; i < m_finishWidgets.size(); ++i)
    {
        m_finishWidgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::ShowNotUpdateUI(bool visible)
{
    for(int i = 0; i < m_notUpdateWidgets.size(); ++i)
    {
        m_notUpdateWidgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::CheckUpdater(bool isFirst)
{
    m_first = isFirst;
    ShowFinishUpdateUI(false);
    ShowNotUpdateUI(false);
    CheckUpdate();
    ShowCheckUpdateUI(true);
    //m_updater->DownloadUpdaterXmlFile();
    this->exec();
}

void AutoUpdaterUI::slotCheckTimeOut()
{
    QString strTime = tr("PASS : %1s").arg(++m_checkupdateTimeOut);
    m_timeLabel->setText(strTime);
}

void AutoUpdaterUI::slotDownloadUpdaterXmlOver()
{
    //检查下载到了updater.xml和versionInfo.txt，说明检查更新结束了。
    ShowCheckUpdateUI(false);

    m_outputVersionInfoEdit->clear();
    QStringList strListVersionInfo = m_updater->GetVersionInfo();
    if(strListVersionInfo.isEmpty())
        m_outputVersionInfoEdit->append(QStringLiteral("版本信息缺失！"));
    qDebug() << "strListVersionInfo.size = " << strListVersionInfo.size();
    for(int i = 0; i < strListVersionInfo.size(); ++i)
    {
        qDebug() << "version content: " << i << " : " << strListVersionInfo.at(i);
        m_outputVersionInfoEdit->append(strListVersionInfo.at(i));
    }
    m_outputVersionInfoEdit->moveCursor(QTextCursor::Start);

    //更新则isUpdate = true,否则false
    if(m_updater->CheckVersionForUpdate())
    {
        //此时需要更新，弹出对话框让客户端进行选择更新与否
        int n = m_updater->CheckUpdateFiles(QApplication::applicationDirPath() + "/download/updater.xml",
                                   QApplication::applicationDirPath() + "/updater.xml");
        if(n != 1)
        {
            NotUpdate();
            ShowNotUpdateUI(true);
        }
        else
        {
            Update();
            ShowUpdateUI(true);
        }
    }
    else
    {
        //This is the laster version so hide update button and cansel button,
        //and show the laster notify message and ok button.
        NotUpdate();
        ShowNotUpdateUI(true);
    }
}

/*update function*/
void AutoUpdaterUI::slotBtnUpdateClicked()
{
    Updating();
    ShowUpdateUI(false);
    ShowUpdatingUI(true);
    //update,and start updateProsessTimer
    m_updateProsessTimer->start(50);

    //这里执行更新，就是XML对比出来后的所有需更新文件的下载。
    m_updater->DownloadUpdateFiles();
}

void AutoUpdaterUI::slotUpdateProcess()
{
    QStringList strCurrDownloadFileList = m_updater->GetCurrDownloadFileList();
    qDebug() << "strCurrDownloadFileList.size() = " << strCurrDownloadFileList.size();
    static int i = 0;
    for(; i < strCurrDownloadFileList.size(); ++i)
    {
        m_outputVersionInfoEdit->append(QStringLiteral("正在更新文件..."));
        m_outputVersionInfoEdit->append(strCurrDownloadFileList.at(i));
    }

    QStringList strFinishDownloadFileList = m_updater->GetFinishDownloadFileList();
    qDebug() << "strFinishDownloadFileList.size() = " << strFinishDownloadFileList.size();
    static int j = 0;
    for(; j < strFinishDownloadFileList.size(); ++j)
    {
        m_outputVersionInfoEdit->append(QStringLiteral("完成更新文件..."));
        m_outputVersionInfoEdit->append(strFinishDownloadFileList.at(j));
    }

    m_updateProgressBar->setValue(m_updater->GetUpdateProcess());

    qDebug() << "m_updateProgressBar->value() = " << m_updateProgressBar->value();
    qDebug() << "m_updateProgressBar->maximum() = " << m_updateProgressBar->maximum();
    if(m_updateProgressBar->value() == m_updateProgressBar->maximum())
    {
        m_outputVersionInfoEdit->append(QStringLiteral("注意：所有文件已经更新完成，"
                                          "点击重启客户端会启动最新版本，"
                                          "点击取消保持当前版本运行，下次启动为最新版本！"));
        m_titleLabel->setText(QStringLiteral("更新完成！"));
        FinishUpdate();
        ShowUpdatingUI(false);
        ShowFinishUpdateUI(true);
    }
}

void AutoUpdaterUI::slotBtnOkClicked()
{

}

void AutoUpdaterUI::mousePressEvent(QMouseEvent *event)
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

void AutoUpdaterUI::mouseMoveEvent(QMouseEvent *event)
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

void AutoUpdaterUI::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}


