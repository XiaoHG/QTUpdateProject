#include "serverrequest.h"

#include <QFile>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QEventLoop>
#include <QNetworkReply>

ServerRequest::ServerRequest()
{

}

ServerRequest::~ServerRequest()
{

}

void ServerRequest::getRequest()
{
    QNetworkAccessManager *m_pHttpMgr = new QNetworkAccessManager();
    //设置url
    QString url = "http://10.0.0.20:8005/vending_machine/items/ac83f39ea268";//fae4b325e52c
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));

    //添加事件循环机制，返回后再运行后面的
    QEventLoop eventLoop;
    QNetworkReply *reply = m_pHttpMgr->get(requestInfo);
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    //错误处理
    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug() << "request protobufHttp NoError";
    }
    else
    {
        qDebug()<<"request protobufHttp handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "request protobufHttp found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug(qPrintable(reply->errorString()));
    }
    //请求返回的结果
    QByteArray responseByte = reply->readAll();
    qDebug() << responseByte;
}


