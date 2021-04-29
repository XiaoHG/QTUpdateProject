#ifndef UPDATECLIENTUI_H
#define UPDATECLIENTUI_H

#include "cautoupdater.h"

#include <QPushButton>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QSlider>
#include <QTimer>
#include <QDialog>
#include <QList>
#include <QDialog>
#include <QMovie>
#include <QProgressBar>
#include <QMainWindow>

class CUpdateClientUI : public QMainWindow
{
    Q_OBJECT

public:
    static CUpdateClientUI *getInstance()
    {
        static CUpdateClientUI instance;
        return &instance;
    }
    ~CUpdateClientUI();

private:
    CUpdateClientUI(QWidget *parent = 0);

public:
    bool CheckUpdate();
    void ExitApp(QString name);

private:
    void Init();
    void InitUI();
    void UpdateUI();
    void UpdatingUI();
    void NotUpdateUI();
    void Updating();
    void FinishUpdate();

    void SetVisibleUpdateUI(bool b);
    void SetVisibleUpdatingUI(bool b);
    void SetVisibleNotUpdateUI(bool b);
    void SetVisibleFinishUpdateUI(bool b);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    void slotUpdateBtnClicked();
    void slotOkBtnClicked();
    void slotUpdateTimeOut();

private://鼠标拖拽事件
    bool m_bDrag;
    QPoint m_mouseStartPoint;
    QPoint m_windowTopLeftPoint;

private:
    //check update btn
    QPushButton *m_btnUpdate;
    //output files that need to update
    QTextEdit *m_outputVersionInfoEdit;
    //update flag
    bool m_isUpdate;
    //the laster version
    QLabel *m_titleLabel;
    //update prosess timer
    QTimer *m_updateProsessTimer;
    //need to update
    QStringList *m_updateFiles;
    //version server new version information
    QLabel *m_newVersionInfoLabel;
    //server laster version info
    QString m_versionServerInfo;
    QStringList m_downloadVersionInfos;
    //当前版本info
    QStringList m_currentVersionInfoList;
    QLabel *m_labelLasterVersion;
    //log info title
    QLabel *m_logTitleLabel;
    //close btn
    QPushButton *m_btnClose;
    //更新进度
    QProgressBar *m_updateProgressBar;
    QPushButton *m_btnOk;
    QPushButton *m_btnCansel;

    //界面部件集中处理
    QList<QWidget*> m_notUpdateWidgets;
    QList<QWidget*> m_updateWidgets;
    QList<QWidget*> m_updatingWidgets;
    QList<QWidget*> m_finishWidgets;

    //AutoUpdater 对象
    CAutoUpdater m_updater;
};

#endif // UPDATECLIENTUI_H
