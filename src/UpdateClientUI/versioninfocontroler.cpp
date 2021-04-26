#include "versioninfocontroler.h"
#include "updateclientfilecontroler.h"
#include <QCoreApplication>

VersionInfoControler::VersionInfoControler()
{
    init();
}

VersionInfoControler::~VersionInfoControler()
{

}

void VersionInfoControler::init()
{
    QString versionClientFileName = QString::asprintf("%1/versionInfo.txt")
                                                        .arg(QCoreApplication::applicationDirPath());
    UpdateClientFileControler versionClientFileInfo(versionClientFileName);
    curVersionInfo = versionClientFileInfo.readFile();
    curVersion = curVersionInfo[0];
}

QString VersionInfoControler::getCurrentVersion()
{
    return curVersion;
}

QStringList VersionInfoControler::getCurrenVersionInfo()
{
    return curVersionInfo;
}

bool VersionInfoControler::compareServerAndClientVersion(QString versionServerInfo)
{
    //server and client version;

    QStringList strSplitSeverInfos = versionServerInfo.split('.');
    QStringList curversionList = curVersion.split('.');

    for(int i = 0; i < strSplitSeverInfos.size(); ++i)
    {
        if(strSplitSeverInfos[i] > curversionList[i])
        {
            //server version new than client
            //setCurrentversion(versionServerInfo);
            return true;
        }
    }
    return false;
}
