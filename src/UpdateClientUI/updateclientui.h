#ifndef UPDATECLIENTUI_H
#define UPDATECLIENTUI_H

#include "updateclientui_global.h"
#include <QPushButton>
#include <QWidget>
#include <QTextEdit>

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

protected:
    UpdateClientUI(QWidget *parent = 0);

private:
    void init();
    void UI();
    void update();

public:
    bool checkUpdate();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    void slotPrintUpdateFilesName();
    void close();

private://drag event
    bool m_bDrag;
    QPoint mouseStartPoint;
    QPoint windowTopLeftPoint;

private:
    //check update btn
    QPushButton *btnOk;
    //cansel update
    QPushButton *btnCansel;
    //output files that need to update
    QTextEdit *outputEdit;
    //update flag
    bool isUpdate;
};

#endif // UPDATECLIENTUI_H
