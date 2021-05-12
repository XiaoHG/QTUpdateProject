
#ifndef FileSelectWidget_H
#define FileSelectWidget_H

#include <QWidget>

class FileSelectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileSelectWidget(QWidget *parent = nullptr);

signals:
    void clicked();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mousePressEvent(QMouseEvent *ev);
};

#endif // FileSelectWidget_H
