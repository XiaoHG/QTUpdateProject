#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QMap>

class QPushButton;
class MainWindow;

namespace Ui {
class MainWindow;
}

typedef enum eWhichPage{
    MAIN,
    FILELIST,
    SYSTEM,
    TOOLPAGE_1,
    TOOLPAGE_2,
    MODELVIEW,
    LANGUAGE,
    NETINFO,
    VERSIONINFO,
    SYSTEMSET,
    MOVEZ,
    DETECTRESIN_1,
    DETECTLIGHT,
    CONNCLOUD,
    POWERSET,
    ININSIDETEST,
    PRINTTEST,
    DETECTRESIN_2,
    EXECLIGHT,
    NETTEST,
    CLOUDTEST,
    DETECTCAMERA,
    PRINTPARAMETER
}EWHICHPAGE;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void Read();
    void ListFile(QString name);

    void MainUI();
    void FileListUI();
    void SystemUI();
    void ToolPage_1UI();
    void ToolPage_2UI();
    void LanguageUI();
    void NetInfoUI();
    void VersionInfoUI();
    void SystemSetUI();
    void MoveZUI();
    void DetectResinUI();
    void DetectLightUI();

    void ShowMainUI(bool visible);
    void ShowFileListUI(bool visible);
    void ShowSystemUI(bool visible);
    void ShowToolPage_1UI(bool visible);
    void ShowToolPage_2UI(bool visible);
    void ShowLanguageUI(bool visible);
    void ShowNetInfoUI(bool visible);
    void ShowVersionInfoUI(bool visible);
    void ShowSystemSetUI(bool visible);
    void ShowMoveZUI(bool visible);
    void ShowDetectResinUI(bool visible);
    void ShowDetectLightUI(bool visible);

    void WhichUI(const EWHICHPAGE which);

public slots:

    void slotTest();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private://鼠标拖拽事件
    bool m_bDrag;
    QPoint m_mouseStartPoint;
    QPoint m_windowTopLeftPoint;

private:
    Ui::MainWindow *ui;
    QPushButton *m_btnBack;

    //main UI
    QPushButton *m_btnPrint;
    QPushButton *m_btnSystem;
    QPushButton *m_btnTool;
    QList<QWidget*> m_mainUIList;

    //print UI
    QPushButton *m_btnResFile;
    QPushButton *m_btnUp;
    QPushButton *m_btnDown;
    QPushButton *m_btnUDiskFile;
    QWidget *m_widgets[4];
    QList<QWidget*> m_fileUIList;

    //system UI
    QPushButton *m_btnLanguage;
    QPushButton *m_btnNetInfo;
    QPushButton *m_btnVersion;
    QPushButton *m_btnSet;
    QPushButton *m_btnPrePage;
    QPushButton *m_btnNextPage;
    QList<QWidget*> m_systemUIList;

    //tool page 1 UI
    QList<QWidget*> m_toolPage_1UIList;

    //tool page 2 UI
    QList<QWidget*> m_toolPage_2UIList;

    //Language UI
    QList<QWidget*> m_languageUIList;

    //Net information UI
    QList<QWidget*> m_netInfoUIList;

    //Version UI
    QList<QWidget*> m_versionInfoUIList;

    //System set UI
    QList<QWidget*> m_systemSetUIList;

    //Move Z UI
    QList<QWidget*> m_moveZUIList;

    //Detect resin UI
    QList<QWidget*> m_detectResinUIList;

    //Detect light UI
    QList<QWidget*> m_detectLightUIList;
};

#endif // MAINWINDOW_H
