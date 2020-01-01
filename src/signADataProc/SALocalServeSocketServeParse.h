#ifndef SALOCALSERVESOCKETSERVEPARSE_H
#define SALOCALSERVESOCKETSERVEPARSE_H


class SALocalServeSocketServeParse : public SALocalServeSocketOpt
{
    Q_OBJECT
public:
    SALocalServeSocketServeParse(QLocalSocket* localSocket,QObject* parent = nullptr);
    SALocalServeSocketServeParse(QObject* parent = nullptr);
    //设置token - 仅用于服务端
    void setToken(uint token);
};

#endif // SALOCALSERVESOCKETSERVEPARSE_H
