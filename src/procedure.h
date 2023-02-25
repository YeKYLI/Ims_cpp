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
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

   // 向 UE2 发送 INVITE
    p = sendBuf;
    len = 0;

    string curr = string("INVITE ") + string("sip:13800011103@182.0.123.58:5060") + string(" SIP/2.0");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("To: ") + string("<sip:13800011103@182.0.123.58:5060>") + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("Call-ID: 2eefbb81-fc08a76-2bd31-7fc60491c140-6e78580a-13c4-7217\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("CSeq: 1 INVITE\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("Via: SIP/2.0/UDP 10.88.120.110:5060;branch=z9hG4bK-65738974-2bd31-ab308a7-7fc605696d50");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("Max-Forwards: 70");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr  = string("P-Preferred-Service: ") + msg.headers[string("P-Preferred-Service")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr  = string("Accept-Contact: ") + msg.headers[string("Accept-Contact")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr  = string("Allow: ") + msg.headers[string("Allow")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr  = string("P-Access-Network-Info: ") + msg.headers[string("P-Access-Network-Info")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr  = string("P-Early-Media: ") + msg.headers[string("P-Early-Media")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr  = string("P-Preferred-Identity: ") + msg.headers[string("P-Preferred-Identity")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("Contact: <sip:10.88.120.110:5060;transport=UDP>");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr  = string("Content-Type: ") + msg.headers[string("Content-Type")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr  = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    memcpy(p, msg.content.c_str(), msg.content.size());
    p += msg.content.size();
    len += msg.content.size();

    // hardcode
    sendUdpMessage(sendBuf, len, "182.0.0.58");

    // 给UE 1 回复 100 TRYING

    p = sendBuf;
    len = 0;

    string firstLine = "SIP/2.0 100 Trying\r\n";
    memcpy(p, firstLine.c_str(), firstLine.size());
    p += firstLine.size();
    len += firstLine.size();

    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("To: ") + msg.headers[string("To")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("Call-ID: ") + msg.headers[string("Call-ID")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size(); 

    curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size(); 

    curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size(); 

    curr = string("Contact: <sip:10.88.120.110:5060;transport=UDP>");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr  = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();



    int n = write(tcpSockFd, sendBuf, 10);
    if(n < 0)
    {
        cout << "response tcp message failed !!!" << endl;
    }

    free(sendBuf);
}

void ack(SipMessage msg)
{
    return ;
}

void bye(SipMessage msg)
{
    return ;
}

void subscribe(SipMessage msg)
{
    return ;
}

void regist(SipMessage msg)
{
    return ;
}
void copyStringToBuff(string curr, char** p, int* len)
{
    memcpy(*p, curr.c_str(), curr.size());
    *p += curr.size();
    *len += curr.size();
}

void ring(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

    copyStringToBuff(string("SIP/2.0 100 Trying\r\n"), &p, &len);

    string curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("To: ") + msg.headers[string("To")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("Call-ID: ") + msg.headers[string("Call-ID")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size(); 

    curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("Allow: ") + msg.headers[string("Allow")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();  

    curr = string("P-Access-Network-Info: ") + msg.headers[string("P-Access-Network-Info")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

     curr = string("Via: ") + msg.headers[string("Via")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("Contact: ") + msg.headers[string("Contach")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    memcpy(p, curr.c_str(), curr.size());
    p += curr.size();
    len += curr.size();

    free(sendBuf);
}

void procedure(SipMessage msg)
{
    
    if(msg.firstLine.at(0) == 'I')
    {
        invite(msg);
    }
    else if(msg.firstLine.at(0) == '1' && msg.firstLine.at(1) == '8' && msg.firstLine.at(2) == '0')
    {
        ring(msg);
    }
    else if(msg.firstLine.at(0) == '2' && msg.firstLine.at(1) == '0' && msg.firstLine.at(2) == '0')
    {
        ring(msg);
    }
    if(msg.firstLine.at(0) == 'A')
    {
        ack(msg);
    }
    else if(msg.firstLine.at(0) == 'R')
    {
        regist(msg);
    }
    else if(msg.firstLine.at(0) == 'S')
    {
        subscribe(msg);
    }
    else if(msg.firstLine.at(0) == 'B')
    {
        bye(msg);
    }
    else
    {
        cout << "Error: bad message format : " << msg.firstLine << endl;
    }
}

#endif
