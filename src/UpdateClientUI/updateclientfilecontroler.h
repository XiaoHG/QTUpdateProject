#ifndef UPDATECLIENTFILECONTROLER_H
#define UPDATECLIENTFILECONTROLER_H

#include <QFile>



class UpdateClientFileControler
{
public:
    UpdateClientFileControler(QString &filePath);

public:
    QStringList readFile();

private:
    QString versionFilePath;
    QStringList versionInfos;
};

#endif // UPDATECLIENTFILECONTROLER_H
