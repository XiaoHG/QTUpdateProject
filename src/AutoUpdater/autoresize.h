#ifndef AUTORESIZE_H
#define AUTORESIZE_H

#include <QWidget>
#include <QRadioButton>
#include <QMap>
#include <QFont>

struct AutoResizeOriginalData
{
    QRect rectOriginal;
    QFont fontOriginal;
};

class AutoResize
{

public:

    AutoResize(QWidget* obj, float baseWidth, float baseHeight);
	~AutoResize(void);

    void doAutoResize();
    void pushAllResizeItem();

private:
    void calculateResizeRatio();
    void fontAutoResize(QWidget *obj, int fontSIze);
    void setOriginalData(QWidget *obj);

private:
    QMap<QWidget*, AutoResizeOriginalData> m_resizeMap;
    QMap<QWidget*, AutoResizeOriginalData> m_fontMap;

    float m_fHorRatio;
    float m_fVerRatio;
    float m_fFontRatio;
    float m_fBaseWidth;
    float m_fBaseHeight;

    QWidget *m_autoResizeWidget;
};

#endif//AUTORESIZE_H

