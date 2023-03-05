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
#include <map>
#include <algorithm>
#include <message.h>

using std::endl;
using std::cout;
using std::thread;
using std::map;
extern int udpSockFd;
extern int tcpSockFd;

vector<string> splitString(string str, char ch)
{
    vector<string> res;
    str += ch;
    int start = 0;
    int last = str.find(ch);
    while(last < int(str.size()))
    {
        if(start != last)
        {
            res.push_back(str.substr(start, last - start));
        }
        start = last + 1;
        last = str.find(ch, start);
        if((-1 == start) || (-1 == last))
        {
            return res;
        }
    }
    return res;
}

int tcpWorker(int connfd)
{
     char* buf = (char*)(malloc(10000));
    //struct sockaddr_in clientAddr;
    //int clientAddrLen = sizeof(clientAddr);
	while(1)
    {
        
        //int len = recvfrom(connfd, buf, 10000, 0,  (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
        int len = read(connfd, buf, 10000); 
        if (len <= 0) 
        { 
            continue;
            //printf("Error: recv bad tcp message \n");
        }
        
        printf("recv new tcp message\n");

        processBuf(buf, len, 2, connfd);

    }

    return 1;
}

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
    serverAddr.sin_addr.s_addr  = inet_addr(ip.c_str());  

    if (bind(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)//绑定套接字 
    {
        printf("Error: bind tcp socket failed !!!\n"); 
        return -1; 
    }

    if(listen(sockFd, 100))
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

        thread tcpWorkerThread(tcpWorker, connfd);
        tcpWorkerThread.join();

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
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());

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
    
        processBuf(buf, len, 1, sockFd);
    }
}


int server()
{
    FILE* cfgFile = fopen("cscfcfg.yaml", "r");
    if(cfgFile != NULL)
    {
	char str[1000];
        while(fgets(str, 999, cfgFile) != NULL)
        {
            //cout << str << endl;
 	    
 	    string curr(str);

    	    auto output = splitString(curr, ':');
            if(output.size() == 2)
	    {
  	       
	//	cout << output[1] <<  "||" << endl;
		std::replace(output[1].begin(), output[1].end(),'\n', '\0');
		ip = output[1];
	//	cout << ip << endl;
	//	cout << output[1] <<  "||" << endl;

	    }
	    if(output.size() == 3)
	    {
		std::replace(output[2].begin(), output[2].end(),'\n', '\0');
		imsiPhone[output[1]] = output[2];
	//	cout << imsiPhone[output[1]] << endl;
		 
            }
        }
        

        fclose(cfgFile);
    }
    else
    {
        cout << "Bad Cfg File !!!" << endl;
        return 1;
    }

    thread tcpThread(tcpServer);
    thread udpThread(udpServer);

    tcpThread.join();
    udpThread.join();
    
    return 0;
}

#endif
