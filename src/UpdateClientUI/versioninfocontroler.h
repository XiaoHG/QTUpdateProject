#ifndef VERSIONINFOCONTROLER_H
#define VERSIONINFOCONTROLER_H

#include <QStringList>

class VersionInfoControler
{
public:
    VersionInfoControler();
//    static versionInfoControler *getInstall()
//    {
//        static versionInfoControler install;
//        return &install;
//    }
    virtual ~VersionInfoControler();

private:

    void init();

public:
    void setCurrentVersion(QString curVersion);
    QString getCurrentVersion();
    QStringList getCurrenVersionInfo();

    bool compareServerAndClientVersion(QString versionServerInfo);

private:
    QString curVersion;
    QStringList curVersionInfo;
};

#endif // VERSIONINFOCONTROLER_H
