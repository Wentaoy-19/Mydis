// 网络通讯的客户端程序。
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

ssize_t tcpsend(int fd,void *data,size_t size)
{
    char tmpbuf[1024];                    // 临时的buffer，报文头部+报文内容。
    memset(tmpbuf,0,sizeof(tmpbuf));
    memcpy(tmpbuf,&size,4);         // 拼接报文头部。
    memcpy(tmpbuf+4,data,size);  // 拼接报文内容。

    return send(fd,tmpbuf,size+4,0);        // 把请求报文发送给服务端。
}

ssize_t tcprecv(int fd,void *data)
{
    int len;
    recv(fd,&len,4,0);            // 先读取4字节的报文头部。
    return recv(fd,data,len,0);           // 读取报文内容。
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage:./client ip port\n"); 
        printf("example:./DBclient 127.0.0.1 5085\n\n"); 
        return -1;
    }

    int sockfd;
    struct sockaddr_in servaddr;
    char buf[1024];
 
    if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0) { printf("socket() failed.\n"); return -1; }
    
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)
    {
        printf("connect(%s:%s) failed.\n",argv[1],argv[2]); close(sockfd);  return -1;
    }

    printf("connect ok.\n");



    for(int i=0;i<10;i++){
        std::string input_buf = "p "+std::to_string(i)+" 23333";
        if (tcpsend(sockfd,input_buf.data(),strlen((input_buf.data()))) <=0) { printf("tcpsend() failed.\n"); return -1; }
        printf("SEND: %s\n",input_buf.data());
    }    

    // while(1){
    //     std::string input_buf;
    //     std::getline(std::cin, input_buf);
    //     if(input_buf == "q") return 0;


    //     if (tcpsend(sockfd,input_buf.data(),strlen((input_buf.data()))) <=0) { printf("tcpsend() failed.\n"); return -1; }
    //     printf("SEND: %s\n",input_buf.data());
        
    //     memset(buf,0,sizeof(buf));
    //     if (tcprecv(sockfd,buf) <=0) { printf("tcprecv() failed.\n"); return -1; }
    //     printf("RECV: %s\n",buf);
    // }


    // //////////////////////////////////////////
    // // 登录业务。
    // memset(buf,0,sizeof(buf));
    // sprintf(buf,"<bizcode>00101</bizcode><username>wucz</username><password>123465</password>");
    // if (tcpsend(sockfd,buf,strlen((buf))) <=0) { printf("tcpsend() failed.\n"); return -1; }
    // printf("发送：%s\n",buf);

    // memset(buf,0,sizeof(buf));
    // if (tcprecv(sockfd,buf) <=0) { printf("tcprecv() failed.\n"); return -1; }
    // printf("接收：%s\n",buf);
    // //////////////////////////////////////////

    // //////////////////////////////////////////
    // // 查询余额业务。
    // memset(buf,0,sizeof(buf));
    // sprintf(buf,"<bizcode>00201</bizcode><cardid>1234567890</cardid>");
    // if (tcpsend(sockfd,buf,strlen((buf))) <=0) { printf("tcpsend() failed.\n"); return -1; }
    // printf("发送：%s\n",buf);

    // memset(buf,0,sizeof(buf));
    // if (tcprecv(sockfd,buf) <=0) { printf("tcprecv() failed.\n"); return -1; }
    // printf("接收：%s\n",buf);
    // //////////////////////////////////////////

    // //////////////////////////////////////////
    // // 心跳。
    // while (true)
    // {
    //     memset(buf,0,sizeof(buf));
    //     sprintf(buf,"<bizcode>00001</bizcode>");
    //     if (tcpsend(sockfd,buf,strlen((buf))) <=0) { printf("tcpsend() failed.\n"); return -1; }
    //     printf("发送：%s\n",buf);

    //     memset(buf,0,sizeof(buf));
    //     if (tcprecv(sockfd,buf) <=0) { printf("tcprecv() failed.\n"); return -1; }
    //     printf("接收：%s\n",buf);

    //     //sleep(10);
    // }
    // //////////////////////////////////////////

    // //////////////////////////////////////////
    // // 注销业务。
    // memset(buf,0,sizeof(buf));
    // sprintf(buf,"<bizcode>00901</bizcode>");
    // if (tcpsend(sockfd,buf,strlen((buf))) <=0) { printf("tcpsend() failed.\n"); return -1; }
    // printf("发送：%s\n",buf);

    // memset(buf,0,sizeof(buf));
    // if (tcprecv(sockfd,buf) <=0) { printf("tcprecv() failed.\n"); return -1; }
    // printf("接收：%s\n",buf);
    // //////////////////////////////////////////
    return 0;

} 
