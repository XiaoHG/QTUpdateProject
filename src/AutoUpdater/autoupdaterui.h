#ifndef AUTOUPDATERUI_H
#define AUTOUPDATERUI_H

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
     * @brief CheckUpdater
     * @param isFirst from parent process.
     * Process entrance, wether update or not.
     */
    void Updater(bool isFirst);

private:
    AutoUpdaterUI(bool bCh, QWidget *parent = 0);

    /**
     * @brief CheckUpdate
     * Check update situation
     */
    void CheckForUpdate();

    /**
     * @brief Update
     * Update situation
     */
    void Update();

    /**
     * @brief Updating
     * Updating situation
     */
    void Updating();

    /**
     * @brief FinishUpdate
     * Finish update situation
     */
    void FinishUpdate();

    /**
     * @brief NotUpdate
     * Don't neet to update situation
     */
    void NotUpdate();

    /**
     * @brief UpdateFailure
     */
    void CheckForUpdateError();

    void InitUI();
    void UpdateUI();
    void UpdatingUI();
    void FinishUpdateUI();
    void NotUpdateUI();
    void DownloadTimeoutUI();
    void UpdateFailureUI();

    /**
     * @brief Language
     * @param ch
     * The default is english
     */
    void Language(bool ch);

    /**
     * @brief CheckFtpDownloadError
     * @return
     */
    bool CheckFtpDownloadError();

    /**
     * @brief UpdatingError
     */
    void UpdatingError();

protected slots:
    /**
     * @brief slotBtnUpdateClicked
     * Start update operation.
     * Download new version path to Up-level directory of QApplication path.
     */
    void slotBtnUpdateClicked();

    /**
     * @brief slotBtnRestartClicked
     * It is restart application to the newest version.
     */
    void slotBtnRestartClicked();

    /**
     * @brief slotUpdateProcess
     * m_updateProsessTimer time out is called
     * It is show the process of current update.
     */
    void slotUpdatingTimerout();

    /**
     * @brief slotDownloadInitFileOver
     * Init file are updater.xml and versionInfo.txt.
     * The two files be download at start update process.
     */
    void slotDownloadInitFileOver();

    /**
     * @brief slotCheckUpdateTimeOut
     * m_updatingTimer time out is called
     * Download init files time out.
     */
    void slotCheckForUpdateTimeout();

    /**
     * @brief slotClickTimeoutOk
     * exit process.
     */
    void slotClickOkExit();

    /**
     * @brief slotDownloadStartPerFile
     * @param fileName
     */
    void slotDownloadStartPerFile(QString fileName);

    /**
     * @brief slotDownloadFinishPerFile
     * @param fileName
     */
    void slotDownloadFinishPerFile(QString fileName);

    /**
     * @brief ShowWhichUI
     * @param widgets
     * @param visible
     */
    void ShowWhichUI(const QList<QWidget *> &widgets, bool visible);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    //Mouse drag event
    bool m_bDrag;
    QPoint m_mouseStartPoint;
    QPoint m_windowTopLeftPoint;

    //Translation
    QTranslator m_qtTranslator;
    bool m_bCh; //and choose version information from chinese or english.

    //widgets
    QLabel *m_titleLabel;
    QPushButton *m_btnClose;
    QLabel *m_logTitleLabel;
    QTextEdit *m_outputVersionInfoEdit;
    QString m_versionServerInfo;
    QPushButton *m_btnUpdate;
    QLabel *m_newHaveVersionLabel;
    QLabel *m_curVersionLabel;
    QProgressBar *m_updateProgressBar;
    QPushButton *m_btnRestart;
    QPushButton *m_btnOKExit;

    //Timer
    QTimer *m_updatingTimer;
    QTimer *m_checkForUpdateTimer;

    //Celect control widgets
    QList<QWidget*> m_checkUpdateWidgets; //check update widgets
    QList<QWidget*> m_updateWidgets;//update widgets
    QList<QWidget*> m_updatingWidgets;//updating widgets
    QList<QWidget*> m_finishWidgets;//finish update widgets
    QList<QWidget*> m_notUpdateWidgets;//don't need update widgets
    QList<QWidget*> m_downloadTimeoutWidgets;//Time out widgets
    QList<QWidget*> m_updateFailureWidgets;//Time out widgets
    QList<FtpManager*> m_ftpList;//ftp object list for download

    AutoUpdater *m_updater;//update manager model
    bool m_first;//first check, true from parent call main function, other is false.

    //Updating failure or not
    bool m_isUpdatingError;

};

#endif // AUTOUPDATERUI_H
