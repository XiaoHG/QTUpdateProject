<<<<<<< HEAD
=======
<<<<<<<< HEAD:src/AutoUpdaterUI/autoupdaterui.h
>>>>>>> home
﻿#ifndef AUTOUPDATERUI_H
#define AUTOUPDATERUI_H

#include "autoupdaterui_global.h"
<<<<<<< HEAD
#include <QDialog>
=======
#include "cautoupdater.h"
#include "ftpmanager.h"

========
﻿#ifndef UPDATECLIENTUI_H
#define UPDATECLIENTUI_H

#include "updateclientui_global.h"
>>>>>>>> home:src/UpdateClientUI/updateclientui.h
>>>>>>> home
#include <QPushButton>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QSlider>
#include <QTimer>
#include <QDialog>
#include <QList>
#include <QDialog>
<<<<<<< HEAD
#include <QMovie>
=======
#include <QProgressBar>
>>>>>>> home

class AUTOUPDATERUISHARED_EXPORT AutoUpdaterUI : public QDialog
{
    Q_OBJECT

public:
<<<<<<< HEAD
    static CUpdateClientUI *getInstance()
    {
        static CUpdateClientUI instance;
        return &instance;
    }
    ~CUpdateClientUI();

private:
    CUpdateClientUI(QWidget *parent = 0);
=======
    static AutoUpdaterUI *getInstance()
    {
        static AutoUpdaterUI instance;
        return &instance;
    }
    ~AutoUpdaterUI();

private:
    AutoUpdaterUI(QWidget *parent = 0);

public:
    bool CheckUpdate();
    void ExitApp(QString name);
>>>>>>> home

private:
    void Init();
    void InitUI();
<<<<<<< HEAD
    void Updating();
    void UpdatingUI();
    void FinishUpdate();
    void UpdateUI();
    void NotUpdateUI();
=======
    void UpdateUI();
    void UpdatingUI();
    void NotUpdateUI();
    void Updating();
    void FinishUpdate();
>>>>>>> home

    void SetVisibleUpdateUI(bool b);
    void SetVisibleUpdatingUI(bool b);
    void SetVisibleNotUpdateUI(bool b);
<<<<<<< HEAD
    void SetVisibleFinishUpdateUI(bool b);

public:
    bool CheckUpdate();

=======
<<<<<<<< HEAD:src/AutoUpdaterUI/autoupdaterui.h
    void SetVisibleFinishUpdateUI(bool b);
========
>>>>>>>> home:src/UpdateClientUI/updateclientui.h

public:
    bool GetConfigFlag();

    void testInterface();
    void CheckUpdater();
public slots:
    void slotDownloadUpdaterXmlOver();
    void slotDownloadVersionInfoFileOver();
>>>>>>> home
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    void slotUpdateBtnClicked();
    void slotOkBtnClicked();
    void slotUpdateTimeOut();
<<<<<<< HEAD

signals:
    void sigCloseMainWindow();
    void sigUpdate();

private://drag event
=======
    void slotUpdateProgess(int value);

private://鼠标拖拽事件
>>>>>>> home
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
<<<<<<< HEAD
    QStringList *updateFiles;
=======
    QStringList *m_updateFiles;
>>>>>>> home
    //version server new version information
    QLabel *m_newVersionInfoLabel;
    //server laster version info
    QString m_versionServerInfo;
    QStringList m_downloadVersionInfos;
    //当前版本info
    QStringList m_currentVersionInfoList;
<<<<<<< HEAD
    QLabel *m_labelLasterVersion;
=======
<<<<<<<< HEAD:src/AutoUpdaterUI/autoupdaterui.h
    QLabel *m_labelLasterVersion;
========
>>>>>>>> home:src/UpdateClientUI/updateclientui.h
>>>>>>> home
    //log info title
    QLabel *m_logTitleLabel;
    //close btn
    QPushButton *m_btnClose;
<<<<<<< HEAD
    //更新进度条
    QProgressBar *m_UpdateProgressBar;
    QPushButton *m_btnOk;
    QPushButton *m_btnCansel;

    //cellect to control widget
=======
    //更新进度
    QProgressBar *m_updateProgressBar;
    QPushButton *m_btnOk;
    QPushButton *m_btnCansel;

    //界面部件集中处理
>>>>>>> home
    QList<QWidget*> m_notUpdateWidgets;
    QList<QWidget*> m_updateWidgets;
    QList<QWidget*> m_updatingWidgets;
    QList<QWidget*> m_finishWidgets;

<<<<<<< HEAD
    CAutoUpdater m_updater;
=======
    //AutoUpdater 对象
    CAutoUpdater m_updater;

    FtpManager *m_ftp;

>>>>>>> home
};

#endif // AUTOUPDATERUI_H
