﻿#ifndef UPDATECLIENTUI_H
#define UPDATECLIENTUI_H

#include "updateclientui_global.h"
#include "varsioninfocontroler.h"
#include <QPushButton>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QSlider>
#include <QTimer>
#include <QDialog>
#include <QList>
#include <QDialog>

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
    void UI();
    void updating();
    void updatingUI();
    void finishUpdate();
    void updateFinishUI();
    void needToUpdateUI();
    void notUpdateUI(VarsionInfoControler *vInfoControl);

public:
    bool checkUpdate();

    void testUpdate(bool isU);

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
    //the laster varsion
    QLabel *titleLabel;
    //update prosess timer
    QTimer *updateProsessTimer;
    //need to update
    QStringList *updateFiles;
    //varsion server new varsion information
    QLabel *newVarsionInfoLabel;
    //update process slider
    QSlider *updateProcessSlider;
    //updating title
    QLabel *updateTitleLabel;
    //laster varsion info
    QLabel *lasterVarsionInfoLabel;
    //server laster varsion info
    QString varsionServerInfo;
    QStringList varsionServerInfos;
};

#endif // UPDATECLIENTUI_H
