#ifndef UPDATECLIENTUI_H
#define UPDATECLIENTUI_H

#include "updateclientui_global.h"
#include <QPushButton>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QSlider>
#include <QTimer>

class UPDATECLIENTUISHARED_EXPORT UpdateClientUI : public QWidget
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
    bool update();

public:
    bool checkUpdate();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    void slotClose();
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
    //cansel update
    QPushButton *btnCansel;
    //if the laster varsion and show ok button
    QPushButton *btnOk;
    //output files that need to update
    QTextEdit *outputEdit;
    //update flag
    bool isUpdate;
    //the laster varsion
    QLabel *vNotifyLabel;
    //update prosess
    QSlider *updateSlider;
    //update prosess timer
    QTimer *updateProsessTimer;
};

#endif // UPDATECLIENTUI_H
