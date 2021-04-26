#ifndef UPDATECLIENTUI_H
#define UPDATECLIENTUI_H

#include "updateclientui_global.h"
<<<<<<< HEAD
=======
#include "versioninfocontroler.h"
>>>>>>> 0dc6cca643f408d20ee0f1e43a94359f09ee6e70
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

class UPDATECLIENTUISHARED_EXPORT CUpdateClientUI : public QDialog
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
<<<<<<< HEAD
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
=======
    void init();
    void initUI();
    void updating();
    void updatingUI();
    void finishUpdate();
    void updateFinishUI();
    void needToUpdateUI();
    void notUpdateUI(VersionInfoControler *vInfoControl);
>>>>>>> 0dc6cca643f408d20ee0f1e43a94359f09ee6e70

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
<<<<<<< HEAD
    bool m_isUpdate;
    //the laster version
    QLabel *m_titleLabel;
=======
    bool isUpdate;
    //the laster version
    QLabel *titleLabel;
>>>>>>> 0dc6cca643f408d20ee0f1e43a94359f09ee6e70
    //update prosess timer
    QTimer *m_updateProsessTimer;
    //need to update
    QStringList *updateFiles;
    //version server new version information
<<<<<<< HEAD
    QLabel *m_newVersionInfoLabel;
    //server laster version info
    QString m_versionServerInfo;
    QStringList m_downloadVersionInfos;
    //当前版本info
    QStringList m_currentVersionInfoList;
=======
    QLabel *newVersionInfoLabel;
    //update process slider
    QSlider *updateProcessSlider;
    //updating title
    QLabel *updateTitleLabel;
    //laster version info
    QLabel *lasterVersionInfoLabel;
    //server laster version info
    QString versionServerInfo;
    QStringList versionServerInfos;
>>>>>>> 0dc6cca643f408d20ee0f1e43a94359f09ee6e70
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
};

#endif // UPDATECLIENTUI_H
