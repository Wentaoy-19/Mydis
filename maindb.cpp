#include <signal.h>
#include "dbServer.h"

// 1、设置2和15的信号。
// 2、在信号处理函数中停止主从事件循环和工作线程。
// 3、服务程序主动退出。

dbServer *DbServer;

void Stop(int sig)    // 信号2和15的处理函数，功能是停止服务程序。
{
    printf("sig=%d\n",sig);
    // 调用EchoServer::Stop()停止服务。
    DbServer->Stop();
    printf("DbServer\n");
    delete DbServer;
    printf("delete DbServer\n");
    exit(0); 
}

int main(int argc,char *argv[])
{
    if (argc != 3) 
    { 
        printf("usage: ./DbServer ip port\n"); 
        printf("example: ./DbServer 192.168.150.128 5085\n\n"); 
        return -1; 
    }

    signal(SIGTERM,Stop);    // 信号15，系统kill或killall命令默认发送的信号。
    signal(SIGINT,Stop);        // 信号2，按Ctrl+C发送的信号。

    DbServer=new dbServer(argv[1],atoi(argv[2]),3,5);
    DbServer->Start();

    return 0;
}
