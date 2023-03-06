#ifndef PROCEDURE
#define PROCEDURE

#include <string.h>
#include <string>
#include <iostream>
#include <sipMessage.h>

using std::string;
using std::cout;
using std::endl;

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


void copyStringToBuff(string curr, char** p, int* len)
{
    memcpy(*p, curr.c_str(), curr.size());
    *p += curr.size();
    *len += curr.size();
}

string newTag()
{
    string output = string("hello435643646") + std::to_string(count ++);
    return output;
}

void invite(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

   // 向 UE2 发送 INVITE
    string toPhone  = msg.firstLine.substr(msg.firstLine.find(':') + 1,
                     msg.firstLine.find(';') - msg.firstLine.find(':') - 1);
    cout << "Debug: toPhone = " << toPhone << endl;

    string toImsi = phoneImsi[toPhone];
    cout << "Debug: toImsi = " << toImsi << endl;

    string toDomain = msg.firstLine.substr(msg.firstLine.find('=') + 1,
                     msg.firstLine.size() - msg.firstLine.find('=') - 6); // magic
    cout << "Debug: toDomain = " << toDomain << endl;

   // INVITE tel:13800011101;phone-context=ims.mnc001.mcc001.3gppnetwork.org SIP/2.0
    string curr = string("INVITE tel:") +  toPhone + string(";phone-context=") + 
                 toDomain + string(" SIP/2.0") + string("\r\n");
    copyStringToBuff(curr, &p, &len);
    
    //Via:SIP/2.0/TCP 172.16.1.201:11009;branch=z9hG4bKINV000001PX00003300000400100
    curr = string("Via:SIP/2.0/TCP ") + ip + 
           string(":11009;branch=z9hG4bKINV000001PX00003300000400100") + string("\r\n");
    copyStringToBuff(curr, &p, &len);
    
    //Record-Route:<sip:PX000000330400010000@172.16.1.201:11009;lr>
    curr = string("Record-Route:<sip:PX000000330400010000@") + ip + string(":11009;lr>\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Contact: ") + msg.headers[string("Contact")];
    copyStringToBuff(curr, &p, &len);
 
    curr = string("CSeq: ") + msg.headers[string("Cseq")];
    copyStringToBuff(curr, &p, &len);

    curr = string("Call-ID: ") + msg.headers[string("Call-ID")];
    copyStringToBuff(curr, &p, &len);

    curr = string("Max-Forward: ") + std::to_string(std::atoi(msg.headers[string("Max-Forward")].c_str()) - 1) + string("\r\n");
    copyStringToBuff(curr, &p, &len);
    
    curr = string("To: ") + msg.headers[string("To")];
    copyStringToBuff(curr, &p, &len);

    curr = string("Allow: ") + msg.headers[string("Allow")];
    copyStringToBuff(curr, &p, &len);

    curr = string("Session-Expires: ") + msg.headers[string("Session-Expires")];
    copyStringToBuff(curr, &p, &len);

    curr = string("Supported: ") + msg.headers[string("Supported")];
    copyStringToBuff(curr, &p, &len);

    curr = string("From: ") + msg.headers[string("From")];
    copyStringToBuff(curr, &p, &len);

    curr = string("Content-Type: ") + msg.headers[string("Content-Type")];
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Asserted-Identity:<sip:") + toImsi + string("@") + toDomain + string(">\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Asserted-Identity:<tel:") + toPhone + string(";phone-context=")
         + toDomain + string(">\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Called-Party-ID:<tel:") + msg.phone + string(";phone-context=")
         + msg.domain + string(">\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Accept: ") + msg.headers[string("Accept")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Accept-Contact: ") + msg.headers[string("Accept-Contact")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Min-SE: ") + msg.headers[string("Min-SE")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Early-Media: ") + msg.headers[string("P-Early-Media")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("User-Agent: ") + msg.headers[string("User-Agent")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("\r\n");
	copyStringToBuff(curr, &p, &len);

    copyStringToBuff(msg.content, &p, &len);

    int n = write(tcpSockFd, sendBuf, len);
    if(n < 0)
    {
        cout << "response tcp message failed !!!" << endl;
    }

// 给UE 1 回复 100 TRYING

    p = sendBuf;
    len = 0;

    curr = "SIP/2.0 100 Trying\r\n";
    copyStringToBuff(curr, &p, &len);

    curr = string("Via:SIP/2.0/TCP ") + msg.ip  + 
           string(":5060;branch=z9hG4bKHLecbPJdlaSIPpeaaypY;received=") + msg.ip + string(";rport=5060\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("CSeq:") + msg.headers[string("CSeq")];
    copyStringToBuff(curr, &p, &len);

    
    curr = string("Call-ID:") + msg.headers[string("Call-ID")];
    copyStringToBuff(curr, &p, &len);

    curr = string("To: ") + msg.headers[string("To")].substr(
        msg.headers[string("To")].find('<'), 
        msg.headers[string("To")].size() - msg.headers[string("To")].find('<')) + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("From: ") + msg.headers[string("From")];
    copyStringToBuff(curr, &p, &len);

    curr = string("Content-Length:0\r\n\r\n");
    copyStringToBuff(curr, &p, &len);

    n = write(tcpSockFd, sendBuf, len);
    if(n < 0)
    {
        cout << "response tcp message failed !!!" << endl;
    }

    free(sendBuf);
}

void ack(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

    string curr = msg.firstLine;
    copyStringToBuff(curr, &p, &len);

    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("To: ") + msg.headers[string("To")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Call-ID: ") + msg.headers[string("Call-ID")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Via: ") + msg.headers[string("Via")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Supported: ") + msg.headers[string("Supported")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Require: ") + msg.headers[string("Require")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Proxy-Require: ") + msg.headers[string("Proxy-Require")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Max-Forwards: ") + msg.headers[string("Max-Forwards")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("User-Agent: ") + msg.headers[string("User-Agent")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Contact: ") + msg.headers[string("Contact")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    free(sendBuf);

    return ;
}

void bye(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

    string curr = string("ACK sip:1012@182.0.3.112:5060 SIP/2.0");
    copyStringToBuff(curr, &p, &len);

    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("To: ") + msg.headers[string("To")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Call-ID: ") + msg.headers[string("Call-ID")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Via: ") + msg.headers[string("Via")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Supported: ") + msg.headers[string("Supported")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Require: ") + msg.headers[string("Require")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Proxy-Require: ") + msg.headers[string("Proxy-Require")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Max-Forwards: ") + msg.headers[string("Max-Forwards")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("User-Agent: ") + msg.headers[string("User-Agent")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("\r\n");
    copyStringToBuff(curr, &p, &len);

    free(sendBuf);

    return ;
}

void subscribe(SipMessage msg)
{
// 200 OK

    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

    copyStringToBuff(string("SIP/2.0 200 OK\r\n"), &p, &len);

	string curr = string("Via:SIP/2.0/TCP ") + msg.ip + string(":5060;branch=z9hG4bKIIecbPJdlaSIPpeaaOpY;received=") + msg.ip + string(";rport=5060")+ string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Contact: <sip:") + ip + string(":11004;transport=UDP>\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("CSeq: ") + msg.headers[string("CSeq")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Call-ID: ") + msg.headers[string("Call-ID")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	string tag = newTag();
	curr = string("To: ") + msg.headers[string("To")] + string(";tag=") +  tag +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Expires: ") + msg.headers[string("Expires")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Content-Length: ") + msg.headers[string("Content-Length")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);


    curr = string("\r\n");
    copyStringToBuff(curr, &p, &len);
	
    int n = write(tcpSockFd, sendBuf, len);
    if(n < 0)
    {
        cout << "response tcp message failed !!!" << endl;
    }

//  NOTIFY
 

    p = sendBuf;
    len = 0;


	curr = string("NOTIFY sip:") + msg.imsi + string("@") + msg.ip + string(":5060 SIP/2.0\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Via:SIP/2.0/TCP ") + msg.ip + string(":5060;branch=z9hG4bKNOT000001PX00003300000300000;received=") + msg.ip + string(";rport=5060")+ string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Record-Route:<sip:PX000000330300000000@1") + ip + string(":5060;lr>\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("CSeq:1 NOTIFY");
	copyStringToBuff(curr, &p, &len);

	curr = string("Call-ID: ") + msg.headers[string("Call-ID")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Max-Forward: ") + std::to_string(std::atoi(msg.headers[string("Max-Forward")].c_str()) - 1) + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("To: ") + msg.headers[string("To")] + string(";tag=") +  tag +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Content-Type:application/reginfo+xml") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Subscription-State:active") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Event:reg") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Content-Length:525") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n") + \
		string("<reginfo version=\"1\" state=\"full\"><registration state=\"active\" aor=\"sip:") +  \
		msg.imsi + string("@") + msg.domain + string("\" id=\"1\"><contact state=\"active\" id=\"1\" event=\"created\"><uri>sip:") + \
		msg.imsi + string("@") + msg.ip + \
		string(":5060</uri></contact></registration><registration state=\"active\" aor=\"tel:") + \
		msg.phone + string(";phone-context=") + msg.domain + \
		string("\" id=\"2\"><contact state=\"active\" id=\"1\" event=\"created\"><uri>sip:") + \
		msg.imsi + string("@") + msg.ip + \
		string("</uri></contact></registration></reginfo>");
	copyStringToBuff(curr, &p, &len);


free(sendBuf);

    return ;
}

void regist(SipMessage msg)
{
	imsiFdMap[msg.imsi] =msg.fd;

    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

    copyStringToBuff(string("SIP/2.0 200 OK\r\n"), &p, &len);

	string curr = string("Via:SIP/2.0/TCP ") + msg.ip + string(":5060;branch=z9hG4bKtFecbjeD1Gn8tbdaaypY;received=") + msg.ip + string(";rport=5060")+ string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Contact: <sip:") + msg.imsi + string("@") + msg.ip + string(":5060>;expires=") + msg.headers[string("Expires")] + 
				  string(";+sip.instance=") + msg.contactMap[string("+sip.instance=")] + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("CSeq: ") + msg.headers[string("CSeq")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Call-ID: ") + msg.headers[string("Call-ID")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("To: ") + msg.headers[string("To")] + string(";tag=") +  newTag() +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Expires: ") + msg.headers[string("Expires")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);
	
    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Service-Route:<sip:orig@") + ip + string(":11004;transport=udp;lr>\r\n"); //感觉这部分可以不用添加，待测试
    copyStringToBuff(curr, &p, &len);

	curr = string("Path:Path:<sip:term@")+ ip + string(":12004;lr>") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("P-Associated-URI:<sip:")+ msg.imsi + msg.domain + string(">") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("P-Associated-URI:<tel:")+ msg.phone + string(";phone-context=") +  msg.domain + string(">") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Content-Length: ") + msg.headers[string("Content-Length")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("\r\n");
    copyStringToBuff(curr, &p, &len);    

    int n = write(tcpSockFd, sendBuf, len);
    if(n < 0)
    {
        cout << "Error: regist response tcp message failed !!!" << endl;
    }

    free(sendBuf);

    return ;
}

void ring183(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

    string toPhone  = msg.firstLine.substr(msg.firstLine.find(':') + 1,
                     msg.firstLine.find(';') - msg.firstLine.find(':') - 1);
    cout << "Debug: toPhone = " << toPhone << endl;

    string toImsi = phoneImsi[toPhone];
    cout << "Debug: toImsi = " << toImsi << endl;

    string curr = "SIP/2.0 183 Session Progress\r\n";
    copyStringToBuff(curr, &p, &len);

    curr = string("Via: ") + msg.headers[string("Via")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Record-Route: ") + msg.headers[string("Record-Route")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Contact: ") + msg.headers[string("Contact")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Call-ID: ") + msg.headers[string("Call-ID")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("To: ") + msg.headers[string("To")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Allow: ") + msg.headers[string("Allow")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Require: ") + msg.headers[string("Require")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Content-Type: ") + msg.headers[string("Content-Type")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Access-Network-Info: ") + msg.headers[string("P-Access-Network-Info")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Asserted-Identity:<sip:") + toImsi + string("@") + msg.domain + string(">\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Asserted-Identity:<tel:") + toPhone + string(";phone-context=")
         + msg.domain + string(">\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("RSeq: ") + msg.headers[string("RSeq")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("User-Agent: ") + msg.headers[string("User-Agent")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("\r\n");
	copyStringToBuff(curr, &p, &len);

    copyStringToBuff(msg.content, &p, &len);

    free(sendBuf);
    return ;
}

void ring180(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

    // To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3
    string toPhone = msg.headers[string("To")].substr(msg.headers[string("To")].find(':') + 1 ,
        msg.headers[string("To")].find('@') - msg.headers[string("To")].find(':') - 1);
    cout << "Debug ring180 toPhone " << toPhone << endl;

    string toIp = msg.headers[string("To")].substr(msg.headers[string("To")].find('@') + 1 ,
        msg.headers[string("To")].find('>') - msg.headers[string("To")].find('@') - 1);
    cout << "Debug ring180 toIp = " << toIp << endl;

    string toImsi = phoneImsi[toPhone];
    cout << "Debug ring180 toImsi = " << toImsi << endl;

    string curr = "SIP/2.0 180 Ringing\r\n";
    copyStringToBuff(curr, &p, &len);

    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

//To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217

    curr = string("To: \"") + toPhone + string("\"<tel:") + toPhone + string(";phone-context=")
           + msg.domain + string(";tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217");
    copyStringToBuff(curr, &p, &len);

    curr = string("Call-ID: ") + msg.headers[string("Call-ID")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Preferred-Identity: ") + msg.headers[string("P-Preferred-Identity")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Access-Network-Info: ") + msg.headers[string("P-Access-Network-Info")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Allow: ") + msg.headers[string("Allow")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("User-Agent: ") + msg.headers[string("User-Agent")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Via: ") + msg.headers[string("Via")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Contact: ") + msg.headers[string("Contact")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);


    free(sendBuf);



    return ;
}

void ok200(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

    // To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3
    string toPhone = msg.headers[string("To")].substr(msg.headers[string("To")].find(':') + 1 ,
        msg.headers[string("To")].find('@') - msg.headers[string("To")].find(':') - 1);
    cout << "Debug ring180 toPhone " << toPhone << endl;

    string toIp = msg.headers[string("To")].substr(msg.headers[string("To")].find('@') + 1 ,
        msg.headers[string("To")].find('>') - msg.headers[string("To")].find('@') - 1);
    cout << "Debug ring180 toIp = " << toIp << endl;

    string toImsi = phoneImsi[toPhone];
    cout << "Debug ring180 toImsi = " << toImsi << endl;

    string curr = "SIP/2.0 200 OK\r\n";
    copyStringToBuff(curr, &p, &len);

    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

//To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217

    curr = string("To: \"") + toPhone + string("\"<tel:") + toPhone + string(";phone-context=")
           + msg.domain + string(";tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217");
    copyStringToBuff(curr, &p, &len);

    curr = string("Call-ID: ") + msg.headers[string("Call-ID")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Preferred-Identity: ") + msg.headers[string("P-Preferred-Identity")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("P-Access-Network-Info: ") + msg.headers[string("P-Access-Network-Info")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Allow: ") + msg.headers[string("Allow")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("User-Agent: ") + msg.headers[string("User-Agent")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Via: ") + msg.headers[string("Via")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Contact: ") + msg.headers[string("Contact")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);


    free(sendBuf);

    return ;
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
    if(msg.firstLine.at(0) == 'I')  // INVITE
    {
        invite(msg);
    }
    if(msg.firstLine.at(0) == 'A')  // ACK
    {
        ack(msg);
    }
    else if(msg.firstLine.at(0) == 'R')  // REGISTER
    {
        regist(msg);
    }
    else if(msg.firstLine.at(0) == 'S')
    {
        if(msg.firstLine.at(1) == 'U') subscribe(msg); // SUBSCRIBE

        if(msg.firstLine.at(2) == 'I' && msg.firstLine.at(8) == '1' 
                        && msg.firstLine.at(9) == '8' && msg.firstLine.at(10) == '3') // ring 183
            ring183(msg);

        if(msg.firstLine.at(2) == 'I' && msg.firstLine.at(8) == '1' 
                        && msg.firstLine.at(9) == '8' && msg.firstLine.at(10) == '0')  // ring 180
            ring180(msg);
        if(msg.firstLine.at(2) == 'I' && msg.firstLine.at(8) == '2' 
                        && msg.firstLine.at(9) == '0' && msg.firstLine.at(10) == '0') // ok 200
            ok200(msg);
    }
    else if(msg.firstLine.at(0) == 'B')
    {
        bye(msg); // BYE
    }
    else
    {
        cout << "Error: bad message format : " << msg.firstLine << endl;
    }
}

#endif
