#include "varsioninfocontroler.h"
#include "updateclientfilecontroler.h"
#include <QCoreApplication>

VarsionInfoControler::VarsionInfoControler()
{
    init();
}

VarsionInfoControler::~VarsionInfoControler()
{

}

void VarsionInfoControler::init()
{
    QString varsionClientFileName = QString::asprintf("%1/varsionInfo.txt")
                                                        .arg(QCoreApplication::applicationDirPath());
    UpdateClientFileControler varsionClientFileInfo(varsionClientFileName);
    curVarsionInfo = varsionClientFileInfo.readFile();
    curVarsion = curVarsionInfo[0];
}

QString VarsionInfoControler::getCurrentVarsion()
{
    return curVarsion;
}

QStringList VarsionInfoControler::getCurrenVarsionInfo()
{
    return curVarsionInfo;
}

bool VarsionInfoControler::compareServerAndClientVarsion(QString varsionServerInfo)
{
    //server and client varsion;

    QStringList strSplitSeverInfos = varsionServerInfo.split('.');
    QStringList curVarsionList = curVarsion.split('.');

    for(int i = 0; i < strSplitSeverInfos.size(); ++i)
    {
        if(strSplitSeverInfos[i] > curVarsionList[i])
        {
            //server varsion new than client
            //setCurrentVarsion(varsionServerInfo);
            return true;
        }
    }
    return false;
}
