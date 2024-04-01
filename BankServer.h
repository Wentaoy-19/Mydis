#pragma once
#include "TcpServer.h"
#include "EventLoop.h"
#include "Connection.h"
#include "ThreadPool.h"

/*
    BankServer类：网上银行服务器
*/

bool getxmlbuffer(const std::string &xmlbuffer,const std::string &fieldname,std::string &value,const int ilen=0);    // 视频中没有第三个参数，加上第三个参数更好。
//bool getxmlbuffer(const std::string &xmlbuffer,const std::string &fieldname,char *value,const int ilen=0);
//bool getxmlbuffer(const std::string &xmlbuffer,const std::string &fieldname,bool &value);
//bool getxmlbuffer(const std::string &xmlbuffer,const std::string &fieldname,int  &value);
//bool getxmlbuffer(const std::string &xmlbuffer,const std::string &fieldname,unsigned int &value);
//bool getxmlbuffer(const std::string &xmlbuffer,const std::string &fieldname,long &value);
//bool getxmlbuffer(const std::string &xmlbuffer,const std::string &fieldname,unsigned long &value);
//bool getxmlbuffer(const std::string &xmlbuffer,const std::string &fieldname,double &value);
//bool getxmlbuffer(const std::string &xmlbuffer,const std::string &fieldname,float &value);
///////////////////////////////////// /////////////////////////////////////


class UserInfo  // 用户信息（状态机）。
{
private:
    int fd_;                      // 客户端的fd。
    std::string ip_;           // 客户端的ip地址。
    bool login_=false;    // 客户端登录的状态：true-已登录；false-未登录。
public:
    UserInfo(int fd,const std::string &ip):fd_(fd),ip_(ip) {}
    void setLogin(bool login) { login_=login; }
    bool Login() { return login_; }
};

class BankServer 
{
private:
    using spUserInfo=std::shared_ptr<UserInfo>;
    TcpServer tcpserver_;
    ThreadPool threadpool_;               // 工作线程池。
    std::map<int,spUserInfo> usermap_;

public:
    BankServer(const std::string &ip,const uint16_t port,int subthreadnum=3,int workthreadnum=5);
    ~BankServer();

    void Start();                // 启动服务。
    void Stop();                // 停止服务。

    void HandleNewConnection(spConnection conn);     // 处理新客户端连接请求，在TcpServer类中回调此函数。
    void HandleClose(spConnection conn);                      // 关闭客户端的连接，在TcpServer类中回调此函数。 
    void HandleError(spConnection conn);                       // 客户端的连接错误，在TcpServer类中回调此函数。
    void HandleMessage(spConnection conn,std::string& message);     // 处理客户端的请求报文，在TcpServer类中回调此函数。

    void OnMessage(spConnection conn,std::string& message);     // 处理客户端的请求报文，用于添加给线程池。
    void HandleRemove(int fd);                       // 客户端的连接超时，在TcpServer类中回调此函数。
};