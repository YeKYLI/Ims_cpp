#ifndef PROCEDURE
#define PROCEDURE

#include <string>
#include <iostream>
#include <sipMessage.h>

using std::string;
using std::cout;
using std::endl;

static int udpSockFd = -1;
static int tcpSockFd = -1; // 后期会改成在消息里、、多个UE肯定会出现问题的

void sendUdpMessage(char* buf, int len, char* ip)
{
    if(udpSockFd == -1)
    {
        cout << "Error: the udp socket fd is wrong !!!" << endl;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5060);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    if(connect(udpSockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("Error: cannot connect the udp port \n");
        return ;
    }

    int length = int(write(udpSockFd, buf, len));
    if(length < 0)
    {
        printf("send udp message  failed !!!");
    }
}

void invite(SipMessage msg)
{
    /*
    // 200 OK
    string firstLine = "200 OK \r\n";
    memcpy(p, firstLine.c_str(), firstLine.size());

    // TO
    string To = "To: " + msg.headers[string("To")] + "\r\n";
    memcpy(p, firstLine.c_str(), firstLine.size());

    */

    // 向 UE1 回应



    // 给UE 1 回复 100 TRYING

cout << "Xxxxxxx" << endl;
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

    string firstLine = "Status-Line: SIP/2.0 100 Trying\r\n";
    memcpy(p, firstLine.c_str(), firstLine.size());
    p += firstLine.size();
    len += firstLine.size();

    string from = "XXX \r\n";

    int n = write(tcpSockFd, sendBuf, 10);
    if(n < 0)
    {
        cout << "response tcp message failed !!!" << endl;
    }
}

/*
void register(SipMessage msg)
{
    // to do
}

*/

void procedure(SipMessage msg)
{
    // 四大流程
    if(msg.firstLine.at(0) == 'I')
    {
        invite(msg);
    }
    else if(msg.firstLine.at(0) == 'R')
    {
        //register(msg);
    }
    else if(msg.firstLine.at(0) == 'S')
    {
        //subscribe(msg);
    }
    else if(msg.firstLine.at(0) == 'B')
    {
        //bye(msg);
    }
    else
    {
        cout << "Error: bad message format : " << msg.firstLine << endl;
    }
}

#endif
