#ifndef SERVER
#define SERVER

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <message.h>
//#include "../lib/ims/ims_getters.h"

using std::endl;
using std::cout;
using std::thread;
extern int udpSockFd;
extern int tcpSockFd;

int tcpServer()
{
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);//创建套接字 
    if (sockFd < 0) 
    { 
        printf("Error: create tcp listen socket failed !!!\n"); 
        return -1; 
    } 

    struct sockaddr_in serverAddr; 
    memset(&serverAddr, 0, sizeof(serverAddr));  
    serverAddr.sin_family       = AF_INET; 
    serverAddr.sin_port         = htons(5060); 
    serverAddr.sin_addr.s_addr  = inet_addr("192.168.0.1");  

    if (bind(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)//绑定套接字 
    {
        printf("Error: bind tcp socket failed !!!\n"); 
        return -1; 
    }

    if(listen(sockFd, 5))
    {
        printf("Errot: listen tcp socket failed");
        return -1;
    }

    struct sockaddr clientAddr;
    int clientAddrLen = sizeof(struct sockaddr);
    while(1)
    {
        memset(&clientAddr, 0, sizeof(clientAddr));

        printf("Info: waiting for tcp accept \n");
        int connfd = accept(sockFd, &clientAddr, (socklen_t*)&clientAddrLen);//使用accept从消息队列中获取请求 
        if (connfd < 0) 
        { 
            printf("Error: accept tcp failed !!!\n");
            return -1; 
        }
        else
        {
            tcpSockFd = connfd;
        }
        char* buf = (char*)(malloc(10000));
        //struct sockaddr_in clientAddr;
        //int clientAddrLen = sizeof(clientAddr);
	    while(1)
        {
            
	    //int len = recvfrom(connfd, buf, 10000, 0,  (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
            int len = read(connfd, buf, 10000); 
            if (len <= 0) 
            { 
                //这里有个重大bug，阻塞在一起
                continue;
                //printf("Error: recv bad tcp message \n");
            }
            
            printf("recv new tcp message\n");

            processBuf(buf, len);

        }
    }
}

int udpServer()
{
    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockFd < 0)
    {
        printf("Error: create udp listen socket failed !!!\n");
        return -1;
    }

    udpSockFd = sockFd;
    //printf("TEST: udp sockFd = %d \n", udpSockFd);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5060);
    serverAddr.sin_addr.s_addr = inet_addr("192.168.0.1");

    if(bind(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)))
    {
        printf("Error: cannot bind the udp port\n");
        return -1;
    }

    printf("Info: listening udp message \n");

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    char* buf = (char*)(malloc(2000));
    while(1)
    {
        int len = recvfrom(sockFd, buf, 2000, 0,  (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
        if(len < 0)
        {
            printf("Error: Recieve bad udp message");
            continue;
        }
        printf("Received message from IP: %s and port: %i\n",
                inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    
        processBuf(buf, len);
    }
}


int server()
{
    thread tcpThread(tcpServer);
    thread udpThread(udpServer);

    tcpThread.join();
    udpThread.join();
    
    return 0;
}

#endif
