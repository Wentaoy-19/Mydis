#include "dbServer.h"
#include <signal.h>

dbServer::dbServer(const std::string &ip,const uint16_t port,int subthreadnum,int workthreadnum)
                   :tcpserver_(ip,port,subthreadnum),threadpool_(workthreadnum,"WORKS"),lsm_(LSM<int, int>(800,20,1.0,0.00100,1024,20))
{
    dblock_.unlock();
    tcpserver_.setnewconnectioncb(std::bind(&dbServer::HandleNewConnection, this, std::placeholders::_1));
    tcpserver_.setcloseconnectioncb(std::bind(&dbServer::HandleClose, this, std::placeholders::_1));
    tcpserver_.seterrorconnectioncb(std::bind(&dbServer::HandleError, this, std::placeholders::_1));
    tcpserver_.setonmessagecb(std::bind(&dbServer::HandleMessage, this, std::placeholders::_1, std::placeholders::_2));
    tcpserver_.setsendcompletecb(std::bind(&dbServer::HandleSendComplete, this, std::placeholders::_1));
    // tcpserver_.settimeoutcb(std::bind(&dbServer::HandleTimeOut, this, std::placeholders::_1));
}

dbServer::~dbServer()
{

}

void dbServer::queryLine(LSM<int, int> &lsm, const string &line, vector<string> &strings){
    unsigned long pos = line.find(' ');
    unsigned long ip = 0;
    strings.clear();
    
    // Decompose statement
    while( pos != string::npos ) {
        strings.push_back( line.substr( ip, pos - ip + 1 ) );
        ip = pos + 1;
        
        pos = line.find( ' ', ip );
    }
    
    // Add the last one
    strings.push_back( line.substr( ip, (pos < line.size() ? pos : line.size()) - ip + 1 ) );
    
    switch ((char) strings[0].c_str()[0]){
        case 'p':{
            int pk = stoi(strings[1]);
            int v = stoi(strings[2]);
            lsm.insert_key(pk, v);
        }
            break;
        case 'g': {
            int lk = stoi(strings[1]);
            int v;
            bool found = lsm.lookup(lk, v);
            if (found) {
                cout << v;
            }
            
            cout << endl;
        }
            break;
        case 'r':{
            int lk1 = stoi(strings[1]);
            int lk2 = stoi(strings[2]);
            auto res = lsm.range(lk1, lk2);
            if (!res.empty()){
                for (int i = 0; i < res.size(); ++i){
                    cout << res[i].key << ":" << res[i].value << " ";
                }
            }
            cout << endl;

        }
            break;
        case 'd': {
            int dk = stoi(strings[1]);
            lsm.delete_key(dk);
        }
            break;
        case 's': {
            lsm.printStats();
        }

        default:{
            cout<<"Invalid Query"<<endl;
        }
    }

}


// 启动服务。
void dbServer::Start()                
{
    tcpserver_.start();
}

 // 停止服务。
 void dbServer::Stop()
 {
    // 停止工作线程。
    threadpool_.stop();
    printf("工作线程已停止。\n");

    // 停止IO线程（事件循环）。
    tcpserver_.stop();
 }

// 处理新客户端连接请求，在TcpServer类中回调此函数。
void dbServer::HandleNewConnection(spConnection conn)    
{
    // std::cout << "New Connection Come in." << std::endl;
    // printf("dbServer::HandleNewConnection() thread is %d.\n",syscall(SYS_gettid));
    printf ("%s new connection(fd=%d,ip=%s,port=%d) ok.\n",Timestamp::now().tostring().c_str(),conn->fd(),conn->ip().c_str(),conn->port());
}

// 关闭客户端的连接，在TcpServer类中回调此函数。 
void dbServer::HandleClose(spConnection conn)  
{
    printf ("%s connection closed(fd=%d,ip=%s,port=%d) ok.\n",Timestamp::now().tostring().c_str(),conn->fd(),conn->ip().c_str(),conn->port());
    // std::cout << "dbServer conn closed." << std::endl;
}

// 客户端的连接错误，在TcpServer类中回调此函数。
void dbServer::HandleError(spConnection conn)  
{
    // std::cout << "dbServer conn error." << std::endl;
}

// 处理客户端的请求报文，在TcpServer类中回调此函数。
void dbServer::HandleMessage(spConnection conn,std::string& message)     
{
    // printf("dbServer::HandleMessage() thread is %d.\n",syscall(SYS_gettid)); 

    if (threadpool_.size()==0)
    {
        // 如果没有工作线程，表示在IO线程中计算。
        OnMessage(conn,message);       
    }
    else
    {
        // 把业务添加到线程池的任务队列中，交给工作线程去处理业务。
        threadpool_.addtask(std::bind(&dbServer::OnMessage,this,conn,message));
    }
}

 // 处理客户端的请求报文，用于添加给线程池。
 void dbServer::OnMessage(spConnection conn,std::string& message)     
 {
    // printf("%s message (fd=%d):%s\n",Timestamp::now().tostring().c_str(),conn->fd(),message.c_str());

    // message="reply:"+message;          // 回显业务。
    
    // conn->send(message.data(),message.size());   // 把数据发送出去。 

    // dblock_.lock();
    //     exec_ret_pkg retpkg = dbcore_.parse_and_execute(message);
    // dblock_.unlock();

    // std::string retdata = retpkg._buffer;
    // auto strings = vector<string>(3);
    // dblock_.lock();
    //     queryLine(lsm_,message, strings);
    // dblock_.unlock();
    
    std::string retdata = "This is mock return data";
    conn->send(retdata.data(),retdata.size());

 }

// 数据发送完成后，在TcpServer类中回调此函数。
void dbServer::HandleSendComplete(spConnection conn)     
{
    // std::cout << "Message send complete." << std::endl;

    // 根据业务的需求，在这里可以增加其它的代码。
}


// epoll_wait()超时，在TcpServer类中回调此函数。
// void dbServer::HandleTimeOut(EventLoop *loop)         
// {
//     std::cout << "dbServer timeout." << std::endl;

//     // 根据业务的需求，在这里可以增加其它的代码。
// }


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
