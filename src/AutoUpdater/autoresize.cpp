#include "autoresize.h"
#include "log.h"

#include <QLabel>
#include <QMapIterator>
#include <QObject>
#include <QDebug>
#include <QTextEdit>
#include <QLineEdit>
#include <QLayout>
#include <QProgressBar>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>

extern Log g_log;

AutoResize::AutoResize(QWidget* obj, float baseWidth, float baseHeight)
    :m_autoResizeWidget(obj)
{
    m_fBaseWidth = baseWidth;
    m_fBaseHeight = baseHeight;
    m_fHorRatio = 1.0;
    m_fVerRatio = 1.0;
    m_fFontRatio = 1.0;
}

AutoResize::~AutoResize(void)
{

}

void AutoResize::calculateResizeRatio()
{
    //get difference destop ratio.
    m_fHorRatio = m_autoResizeWidget->width() / m_fBaseWidth;
    m_fVerRatio = m_autoResizeWidget->height() / m_fBaseHeight;
    m_fFontRatio = m_fHorRatio < m_fVerRatio ? m_fHorRatio : m_fVerRatio;

    g_log.log(Log::INFO, QString::asprintf("HorRatio = %1, VerRatio = %2, FontRatio = %3")
              .arg(m_fHorRatio).arg(m_fVerRatio).arg(m_fFontRatio), __FILE__, __LINE__);
}

void AutoResize::pushAllResizeItem()
{
    QList<QLabel*> _labelList = m_autoResizeWidget->findChildren<QLabel*>();
    for(auto it = _labelList.begin(); it != _labelList.end(); it++)
    {
        setOriginalData(*it);
    }

    QList<QTextEdit*> _textEditList = m_autoResizeWidget->findChildren<QTextEdit*>();
    for(auto it = _textEditList.begin(); it != _textEditList.end(); it++)
    {
        setOriginalData(*it);
    }

    QList<QAbstractButton*> _buttonList = m_autoResizeWidget->findChildren<QAbstractButton*>();
    for(auto it = _buttonList.begin(); it != _buttonList.end(); it++)
    {
        setOriginalData(*it);
    }

    QList<QProgressBar*> _pbList = m_autoResizeWidget->findChildren<QProgressBar*>();
    for(auto it = _pbList.begin(); it != _pbList.end(); it++)
    {
        setOriginalData(*it);
    }

}

void AutoResize::setOriginalData(QWidget *obj)
{
    QRect _tmpRect;
    AutoResizeOriginalData _resizeOriginData;
    _tmpRect = obj->geometry();
    _tmpRect.setX(obj->x());
    _tmpRect.setY(obj->y());
    _tmpRect.setWidth(abs(_tmpRect.width()));
    _tmpRect.setHeight(abs(_tmpRect.height()));
    _resizeOriginData.rectOriginal = _tmpRect;
    _resizeOriginData.fontOriginal = obj->font();
    m_resizeMap[obj] = _resizeOriginData;
}

void AutoResize::fontAutoResize(QWidget *obj, int fontSize)
{
    if(fontSize <= 0)
    {
		return;
    }

    bool _bHasTextStyle = false;
    fontSize *= m_fFontRatio;
    QString _strFontTextReg = "font:\\s+[0-9]+pt";
    QString _strFontFormat = "font: %1pt";
    QString _strFontSizeReg = "[0-9]+";
    QRegExp _reg = QRegExp(_strFontTextReg);
    QRegExp _size = QRegExp(_strFontSizeReg);
    QString _strStyleText = obj->styleSheet();
    QString _strFontText = "";
    QFont _changedFont;
    if(_reg.indexIn(obj->styleSheet()) != -1)
	{
        _strFontText = _reg.capturedTexts().at(0);
        if(_size.indexIn(_strFontText) != -1)
		{
            _bHasTextStyle = true;
		}
	}
	//set stylesheet to change font size
    if(_bHasTextStyle)
	{
        _strStyleText.replace(_reg, _strFontFormat.arg(fontSize));
        obj->setStyleSheet(_strStyleText);
	}
	//set font pointsize to change font size
	else
	{
        _changedFont = obj->font();
        _changedFont.setPointSize(fontSize);
        obj->setFont(_changedFont);
	}
}

void AutoResize::doAutoResize()
{
    //Get the ratio
	calculateResizeRatio();

    QMapIterator<QWidget*, AutoResizeOriginalData> _itarator(m_resizeMap);
    QFont changedFont;
    while(_itarator.hasNext())
    {
        _itarator.next();
        QWidget* _item = _itarator.key();

        QRect _tmpRect = _itarator.value().rectOriginal;
        _tmpRect.setWidth(_tmpRect.width() * m_fHorRatio);
        _tmpRect.setHeight(_tmpRect.height() * m_fVerRatio);
        QRect _afterRect = QRect(_tmpRect.x() * m_fHorRatio, _tmpRect.y() * m_fVerRatio,
                                 _tmpRect.width(), _tmpRect.height());
        //set button changed icon size
        QString _strClassName = _item->metaObject()->className();
        g_log.log(Log::DEBUG, "Resize icon size, class name is: " + _strClassName, __FILE__, __LINE__);
        if(_strClassName == "QPushButton")
        {
            g_log.log(Log::DEBUG, "I am QPushButton Icon, and ready to resize!", __FILE__, __LINE__);
            QPushButton *_btnTmp = static_cast<QPushButton*>(_item);
            g_log.log(Log::DEBUG, QString::asprintf("Resize before, size = %1, %2").arg(_btnTmp->iconSize().width()).arg(_btnTmp->iconSize().height()), __FILE__, __LINE__);
            if(_btnTmp)
            {
                float _fIconRatio = m_fHorRatio > m_fVerRatio ? m_fVerRatio : m_fHorRatio;
                _btnTmp->setIconSize(QSize(_btnTmp->iconSize().width() * _fIconRatio, _btnTmp->iconSize().height() * _fIconRatio));
            }
            g_log.log(Log::DEBUG, QString::asprintf("Resize after, size = %1, %2").arg(_btnTmp->iconSize().width()).arg(_btnTmp->iconSize().height()), __FILE__, __LINE__);
        }
        //changed font
        changedFont = _itarator.value().fontOriginal;
        fontAutoResize(_item, changedFont.pointSize());
        _item->setGeometry(_afterRect);
    }
    QMapIterator<QWidget*, AutoResizeOriginalData> _fontIt(m_fontMap);
    while(_fontIt.hasNext())
    {
        _fontIt.next();
        QWidget* _item = _fontIt.key();
        changedFont = _fontIt.value().fontOriginal;
        fontAutoResize(_item,changedFont.pointSize());
    }
}
