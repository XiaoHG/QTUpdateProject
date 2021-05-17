#ifndef SERVERREQUEST_H
#define SERVERREQUEST_H

#include <QByteArray>

class ServerRequest
{
public:
    ServerRequest();
    virtual ~ServerRequest();

public:
    void getRequest();
};

#endif // SERVERREQUEST_H
