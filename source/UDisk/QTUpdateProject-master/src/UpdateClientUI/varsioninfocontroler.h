#ifndef VARSIONINFOCONTROLER_H
#define VARSIONINFOCONTROLER_H

#include <QStringList>

class VarsionInfoControler
{
public:
    VarsionInfoControler();
//    static VarsionInfoControler *getInstall()
//    {
//        static VarsionInfoControler install;
//        return &install;
//    }
    virtual ~VarsionInfoControler();

private:

    void init();

public:
    void setCurrentVarsion(QString curVarsion);
    QString getCurrentVarsion();
    QStringList getCurrenVarsionInfo();

    bool compareServerAndClientVarsion(QString varsionServerInfo);

private:
    QString curVarsion;
    QStringList curVarsionInfo;
};

#endif // VARSIONINFOCONTROLER_H
