#ifndef AUTOUPDATERUI_H
#define AUTOUPDATERUI_H

#include "autoresize.h"
#include "autoupdater.h"
#include "ftpmanager.h"

#include <QPushButton>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QSlider>
#include <QTimer>
#include <QDialog>
#include <QList>
#include <QMainWindow>
#include <QProgressBar>
#include <QTranslator>
#include <QApplication>

class AutoUpdaterUI : public QMainWindow
{
    Q_OBJECT

public:
    static AutoUpdaterUI *getInstance(bool bCh)
    {
        static AutoUpdaterUI instance(bCh);
        return &instance;
    }
    ~AutoUpdaterUI();

    /**
     * @brief updater
     * @param isFirst from parent process.
     * Process entrance, wether update or not.
     */
    void updater(bool isFromParentMain, QString parentPid);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    AutoUpdaterUI(bool bCh, QWidget *parent = 0);

    /**
     * @brief checkForUpdate
     * Check update situation
     */
    void checkForUpdate();

    /**
     * @brief update
     * Update situation
     */
    void update();

    /**
     * @brief updating
     * Updating situation
     */
    void updating();

    /**
     * @brief finishUpdate
     * Finish update situation
     */
    void finishUpdate();

    /**
     * @brief notUpdate
     * Don't neet to update situation
     */
    void notUpdate();

    /**
     * @brief checkForUpdateError
     */
    void checkForUpdateError();

    //It is about UI init
    void initUI();
    void updateUI();
    void updatingUI();
    void finishUpdateUI();
    void notUpdateUI();
    void downloadErrorUI();

    /**
     * @brief language
     * @param ch
     * The default is english
     */
    void language(bool ch);

    /**
     * @brief checkFtpDownloadError
     * @return
     */
    bool isFtpDownloadError();

    /**
     * @brief updatingError
     */
    void updatingError();

    /**
     * @brief showWhichUI
     * @param widgets
     * @param visible
     */
    void showWhichUI(const QList<QWidget *> &widgets, bool visible);

    /**
     * @brief parseXmlError
     * @param xml
     */
    void parseXmlError(AutoUpdater::UPDATER_ERROR_CODE code);

    /**
     * @brief showVersionInfo
     */
    void showVersionInfo();

protected slots:
    /**
     * @brief on_btn_update_clicked
     * Start update operation.
     * Download new version path to Up-level directory of QApplication path.
     */
    void on_btn_update_clicked();

    /**
     * @brief on_btn_restart_clicked
     * It is restart application to the newest version.
     */
    void on_btn_restart_clicked();

    /**
     * @brief on_timer_updating
     * m_updateProsessTimer time out is called
     * It is show the process of current update.
     */
    void on_timer_updating();

    /**
     * @brief on_updater_initFileDownloadFinish
     * Init file are updater.xml and versionInfo.txt.
     * The two files be download at start update process.
     */
    void on_updater_initFileDownloadFinish();

    /**
     * @brief on_timer_checkForUpdate
     * m_updatingTimer time out is called
     * Download init files time out.
     */
    void on_timer_checkForUpdate();

    /**
     * @brief on_btn_exit_clicked
     * exit process.
     */
    void on_btn_exit_clicked();

    /**
     * @brief on_updater_startDownloadPerFile
     * @param fileName
     */
    void on_updater_startDownloadPerFile(QString fileName);

    /**
     * @brief on_updater_finishDownloadPerFile
     * @param fileName
     */
    void on_updater_finishDownloadPerFile(QString fileName);


private:
    //Mouse drag event
    bool m_bDrag;
    QPoint m_pointMouseStart;
    QPoint m_pointWindowTopLeft;

    //Translation
    QTranslator m_translator;

    //widgets
    QLabel          *m_labelTitle;
    QPushButton     *m_btnClose;
    QLabel          *m_labelLogTitle;
    QTextEdit       *m_teOutputVersionInfo;
    QString         m_strVersionServerInfo;
    QPushButton     *m_btnUpdate;
    QLabel          *m_labelHaveNewVersionNotise;
    QLabel          *m_labelCurrentVersion;
    QProgressBar    *m_pbUpdating;
    QPushButton     *m_btnRestart;
    QPushButton     *m_btnExit;

    //Timer
    QTimer *m_timerUpdating;
    QTimer *m_timerCheckForUpdate;

    //Celect control widgets
    QList<QWidget*> m_listCheckUpdateWidgets; //check update widgets
    QList<QWidget*> m_listUpdateWidgets;//update widgets
    QList<QWidget*> m_listUpdatingWidgets;//updating widgets
    QList<QWidget*> m_listFinishWidgets;//finish update widgets
    QList<QWidget*> m_listNotUpdateWidgets;//don't need update widgets
    QList<QWidget*> m_listDownloadErrorWidgets;//Time out widgets

    AutoUpdater *m_updater;//update manager model
    bool m_bFromParentMain;//first check, true from parent call main function, other is false.

    //Updating failure or not
    bool m_bUpdatingError;

    //auto resize for defference destop.
    AutoResize *m_autoResizeHandler;

};

#endif // AUTOUPDATERUI_H
