#ifndef AUTOUPDATERUI_H
#define AUTOUPDATERUI_H

#include "autoupdaterui_global.h"
#include <QDialog>
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

class AUTOUPDATERUISHARED_EXPORT AutoUpdaterUI : public QDialog
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

private:
    void Init();
    void InitUI();
    void Updating();
    void UpdatingUI();
    void FinishUpdate();
    void UpdateUI();
    void NotUpdateUI();

    void SetVisibleUpdateUI(bool b);
    void SetVisibleUpdatingUI(bool b);
    void SetVisibleNotUpdateUI(bool b);
    void SetVisibleFinishUpdateUI(bool b);

public:
    bool CheckUpdate();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    void slotUpdateBtnClicked();
    void slotOkBtnClicked();
    void slotUpdateTimeOut();

signals:
    void sigCloseMainWindow();
    void sigUpdate();

private://drag event
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
    QStringList *updateFiles;
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
    //更新进度条
    QProgressBar *m_UpdateProgressBar;
    QPushButton *m_btnOk;
    QPushButton *m_btnCansel;

    //cellect to control widget
    QList<QWidget*> m_notUpdateWidgets;
    QList<QWidget*> m_updateWidgets;
    QList<QWidget*> m_updatingWidgets;
    QList<QWidget*> m_finishWidgets;

    CAutoUpdater m_updater;
};

#endif // AUTOUPDATERUI_H
