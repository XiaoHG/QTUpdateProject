﻿#ifndef AUTOUPDATERUI_H
#define AUTOUPDATERUI_H

#include "autoupdaterui_global.h"
#include "cautoupdater.h"
#include "ftpmanager.h"

#include <QPushButton>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QSlider>
#include <QTimer>
#include <QDialog>
#include <QList>
#include <QDialog>
#include <QProgressBar>

class AUTOUPDATERUISHARED_EXPORT AutoUpdaterUI : public QDialog
{
    Q_OBJECT

public:
    static AutoUpdaterUI *getInstance()
    {
        static AutoUpdaterUI instance;
        return &instance;
    }
    ~AutoUpdaterUI();

private:
    AutoUpdaterUI(QWidget *parent = 0);

public:
    void CheckUpdater(bool isFirst);

private:
    void FinishUpdate();

    void InitUI();
    void CheckUpdateUI();
    void UpdateUI();
    void UpdatingUI();
    void FinishUpdateUI();
    void NotUpdateUI();

    void ShowCheckUpdateUI(bool b);
    void ShowUpdateUI(bool b);
    void ShowUpdatingUI(bool b);
    void ShowFinishUpdateUI(bool b);
    void ShowNotUpdateUI(bool b);

public slots:
    void slotDownloadUpdaterXmlOver();
    void slotDownloadVersionInfoFileOver();

protected slots:
    void slotBtnUpdateClicked();
    void slotBtnOkClicked();
    void slotUpdateTimeOut();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private://鼠标拖拽事件
    bool m_bDrag;
    QPoint m_mouseStartPoint;
    QPoint m_windowTopLeftPoint;

private:
    //check update btn
    QPushButton *m_btnUpdate;
    //output files that need to update
    QTextEdit *m_outputVersionInfoEdit;
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
    QList<QWidget*> m_checkUpdateWidgets;
    QList<QWidget*> m_updateWidgets;
    QList<QWidget*> m_updatingWidgets;
    QList<QWidget*> m_finishWidgets;
    QList<QWidget*> m_notUpdateWidgets;

    //AutoUpdater 对象
    CAutoUpdater m_updater;

    FtpManager *m_ftp;
    bool m_first;
};

#endif // AUTOUPDATERUI_H
