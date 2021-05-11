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
    DETECTRESION_1,
    DETECTLIGHT,
    CONNCLOUD,
    POWERSET,
    ININSIDETEST,
    PRINTTEST,
    DETECTRESION_2,
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
    void ShowMainUI(bool visible);
    void ShowFileListUI(bool visible);
    void ShowSystemUI(bool visible);
    void ShowToolPage_1UI(bool visible);
    void ShowToolPage_2UI(bool visible);
    void ShowLanguageUI(bool visible);

    void WhichUI(const EWHICHPAGE which);

public slots:

    void slotWhichUI();
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
    QPushButton *m_btnWeb;
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
};

#endif // MAINWINDOW_H
