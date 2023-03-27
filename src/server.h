#ifndef SERVER
#define SERVER

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netinet/tcp.h>
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

int tcpWorker(int connfd)
{
     char* buf = (char*)(malloc(10000));
    //struct sockaddr_in clientAddr;
    //int clientAddrLen = sizeof(clientAddr);
    string rawString = string("");
	while(1)
    {
        
        //int len = recvfrom(connfd, buf, 10000, 0,  (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
        int len = read(connfd, buf, 10000); 
        if (len <= 0) 
        { 
            continue;
        }
        
     //   printf("INFO: recving tcp message\n");

        processBuf(buf, len, connfd, rawString);

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

        printf("INFO: waiting tcp accept \n");
        int connfd = accept(sockFd, &clientAddr, (socklen_t*)&clientAddrLen);//使用accept从消息队列中获取请求 
        if (connfd < 0) 
        { 
            printf("Error: accept tcp failed !!!\n");
            return -1; 
        }
		else
		{
			int flag = 1;
			setsockopt(connfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));

			cout << "INFO: tcp connfd = " << connfd << endl;
		}

		
        thread tcpWorkerThread(tcpWorker, connfd);
        tcpWorkerThread.detach();

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

    printf("INFO: listening udp message \n");

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    char* buf = (char*)(malloc(10000));
	string rawString = string("");
    while(1)
    {
        int len = recvfrom(sockFd, buf, 10000, 0,  (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
        if(len < 0)
        {
            printf("Error: Recieve bad udp message");
            continue;
        }
		//printf("INFO: recving udp message ip = %s port = %i \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    
        processBuf(buf, len, sockFd, rawString);
    }
}


int server()
{
    FILE* cfgFile = fopen("cscf.cfg", "r");
    if(cfgFile != NULL)
    {
		char str[1000];
        while(fgets(str, 999, cfgFile) != NULL)
        {
 	    
 	    	string curr(str);
    	    auto output = splitString(curr, ':');
            if(output.size() == 2)
	   		{
  	       
				curr += string("xxxxxx");
				std::replace(curr.begin(), curr.end(),'\n', ':');
				ip = splitString(curr, ':')[1];

	    	}
		    if(output.size() > 2)
		    {
		    	curr += string("xxxxxx");
				std::replace(curr.begin(), curr.end(),'\n', ':');
				auto output = splitString(curr, ':');
				
		    	
				imsiPhone[output[1]] = output[2];
	            //phoneImsi[output[2]] = output[1];
		      //  cout << output[1] << " || " << output[2] << endl;
			}
	   }
        

       fclose(cfgFile);
    }
    else
    {
        cout << "Error: cannot fine the cfg file !!!" << endl;
        return 1;
    }


    thread tcpThread(tcpServer);
    thread udpThread(udpServer);

    tcpThread.detach();
    udpThread.join();
    
    return 0;
}

#endif
