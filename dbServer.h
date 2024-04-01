#pragma once
#ifndef dbServer_h
#define dbServer_h
#include "TcpServer.h"
#include "EventLoop.h"
#include "Connection.h"
#include "ThreadPool.h"

#include "skipList.hpp"
#include "bloom.hpp"
#include "hashMap.hpp"
#include "lsm.hpp"
/*
    dbServer: Main Class for Database Server based on LSM Tree
*/

class dbServer 
{
private:
    TcpServer tcpserver_;
    ThreadPool threadpool_;               // Working Threads
    std::mutex dblock_;
    LSM<int, int> lsm_;

public:
    dbServer(const std::string &ip,const uint16_t port,int subthreadnum=3,int workthreadnum=5);
    ~dbServer();

    void Start();                // 启动服务。
    void Stop();                // 停止服务。

    void HandleNewConnection(spConnection conn);     // 处理新客户端连接请求，在TcpServer类中回调此函数。
    void HandleClose(spConnection conn);                      // 关闭客户端的连接，在TcpServer类中回调此函数。 
    void HandleError(spConnection conn);                       // 客户端的连接错误，在TcpServer类中回调此函数。
    void HandleMessage(spConnection conn,std::string& message);     // 处理客户端的请求报文，在TcpServer类中回调此函数。
    void HandleSendComplete(spConnection conn);        // 数据发送完成后，在TcpServer类中回调此函数。
    // void HandleTimeOut(EventLoop *loop);                   // epoll_wait()超时，在TcpServer类中回调此函数。

    void OnMessage(spConnection conn,std::string& message);     // 处理客户端的请求报文，用于添加给线程池。

    void queryLine(LSM<int, int> &lsm, const string &line, vector<string> &strings); 
};
#endif /* dbServer_h */

