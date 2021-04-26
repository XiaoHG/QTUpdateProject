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

class CUpdateClientUI : public QDialog
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

public:
    bool CheckUpdate();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    void slotUpdateBtnClicked();
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
    //log info title
    QLabel *m_logTitleLabel;
    //close btn
    QPushButton *btnClose;
    //updating label
    QLabel *m_updatingLabelGif;
    QLabel *m_updatingProcessLabel;
    QMovie *m_updatingLabelGifMovie;

    //cellect to control widget
    QList<QWidget*> m_notUpdateWidgets;
    QList<QWidget*> m_updateWidgets;
    QList<QWidget*> m_updatingWidgets;

    CAutoUpdater m_updater;
};

#endif // UPDATECLIENTUI_H
