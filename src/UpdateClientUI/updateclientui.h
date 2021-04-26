#ifndef UPDATECLIENTUI_H
#define UPDATECLIENTUI_H

#include "updateclientui_global.h"
#include "versioninfocontroler.h"
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

class UPDATECLIENTUISHARED_EXPORT UpdateClientUI : public QDialog
{
    Q_OBJECT

public:
    static UpdateClientUI *getInstall()
    {
        static UpdateClientUI install;
        return &install;
    }
    ~UpdateClientUI();

private:
    UpdateClientUI(QWidget *parent = 0);

private:
    void init();
    void initUI();
    void updating();
    void updatingUI();
    void finishUpdate();
    void updateFinishUI();
    void needToUpdateUI();
    void notUpdateUI(VersionInfoControler *vInfoControl);

public:
    bool checkUpdate();

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
    QPoint mouseStartPoint;
    QPoint windowTopLeftPoint;

private:
    //check update btn
    QPushButton *btnUpdate;
    //output files that need to update
    QTextEdit *outputEdit;
    //update flag
    bool isUpdate;
    //the laster version
    QLabel *titleLabel;
    //update prosess timer
    QTimer *updateProsessTimer;
    //need to update
    QStringList *updateFiles;
    //version server new version information
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
    //log info title
    QLabel *logTitleLabel;
    //close btn
    QPushButton *btnClose;
    //updating label
    QLabel *updatingLabelGif;
    QLabel *updatingLabel;
    QMovie *updatingLabelGifMovie;

    //cellect to control widget
    QList<QWidget*> notUpdateWidgets;
    QList<QWidget*> updateWidgets;
    QList<QWidget*> updatingWidgets;
    QList<QWidget*> updateFinishWidgets;
};

#endif // UPDATECLIENTUI_H
