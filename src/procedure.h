#ifndef PROCEDURE
#define PROCEDURE

#include <string.h>
#include <string>
#include <iostream>
#include <sipMessage.h>

using std::string;
using std::cout;
using std::endl;

static int count = 1;

string magicToTag = string("");

void sendUdpMessage(char* buf, int len, const char* ip)
{
	
    if(udpSockFd == -1)
    { 
        cout << "Error: the udp socket fd is wrong !!!" << endl;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5060);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    int size = sendto(udpSockFd, buf, len,0,(struct sockaddr *)&serverAddr, sizeof(serverAddr));

    //cout << "INFO: SEND : UDP : IP = " << ip << " size = " << size << "endl";
	
    //if(connect(udpSockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    //{
    //    printf("Error: cannot connect the udp port \n");
    //    return ;
    //}

    //int length = int(write(udpSockFd, buf, len));
    //if(length < 0)
    //{
    //    printf("send udp message  failed !!!");
    //}
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

string newCallId()
{
	string output = string("28344237-58da32bd-195782-7fa7f5c942b0-2278580a-13c4-") + std::to_string(count ++);

	return output;
}

string newBranch()
{
	string output = string("z9hG4bK-65502eb3-195782-62fdd60c-7fa7f6a4") + std::to_string(count ++);

	return output;
}


void invite(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

	cout <<"INFO : begin invite procedure " << endl; 

	//From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
	string fromPhone = msg.headers[string("From")].substr( msg.headers[string("From")].find(':') + 1,
					msg.headers[string("From")].find('@') - msg.headers[string("From")].find(':') - 1);
	State state = phoneState[fromPhone];
	state.fromTag = msg.headers[string("From")].substr( msg.headers[string("From")].find('=') + 1,
					msg.headers[string("From")].size() - msg.headers[string("From")].find('=') - 1);
	state.callId = msg.headers[string("Call-ID")];
	//Via: SIP/2.0/TCP 182.0.3.113:5060;branch=z9hG4bKrvecbbPKcWj1EbefaynY;rport
	state.viaBranch = splitString(msg.headers[string("Via")], ';')[1]; // with branch=
	state.fd = msg.fd;

	phoneState[fromPhone] = state;

	//State state2; 
	//To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>
	string toPhone = msg.headers[string("To")].substr( msg.headers[string("To")].find(':') + 1,
			msg.headers[string("To")].find(';') - msg.headers[string("To")].find(':') - 1);
	State state2 = phoneState[toPhone];
	state2.fromTag = msg.headers[string("From")].substr( msg.headers[string("From")].find('=') + 1,
					 msg.headers[string("From")].size() - msg.headers[string("From")].find('=') - 1);
	//string toDomain = msg.firstLine.substr(msg.firstLine.find('=') + 1,
                     //msg.firstLine.size() - msg.firstLine.find('=') - 6); // magic
	state2.callId = newCallId();
	state2.viaBranch = newBranch();
	phoneState[toPhone] = state2;

//	cout << "Debug: INVITE:  toPhone = " << toPhone <<  "|ip|" << phoneState[toPhone].ip << "||" << endl;

	
   // 向 UE2 发送 INVITE

   // INVITE sip:1012@182.0.3.112:5060 SIP/2.0  
    string curr = string("INVITE sip:") +  toPhone + string("@") + phoneState[toPhone].ip + string(" SIP/2.0") + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	//To: <sip:1012@182.0.3.112:5060>
	curr = string("To: <sip:") + toPhone + string("@") + phoneState[toPhone].ip + string(":5060>")+ string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Call-ID: ") + phoneState[toPhone].callId + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	// Via: SIP/2.0/UDP 10.88.120.34:5060;branch=z9hG4bK-65502eb3-195782-62fdd60c-7fa7f6a474f0  // 自己构造，不知道以后会不会用到
	curr = string("Via: SIP/2.0/UDP ") + ip + string(":5060;branch=") + phoneState[toPhone].viaBranch + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	//P-Preferred-Identity: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>

	 curr = string("P-Preferred-Identity: ") + msg.headers[string("P-Preferred-Identity")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);


// Accept-Contact: *;+g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"
 curr = string("Accept-Contact: ") + msg.headers[string("Accept-Contact")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);


	curr = string("P-Preferred-Service: ") + msg.headers[string("P-Preferred-Service")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Accept: ") + msg.headers[string("Accept")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Require: ") + msg.headers[string("Require")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Proxy-Require: ") + msg.headers[string("Proxy-Require")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Max-Forwards: ") + msg.headers[string("Max-Forwards")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	 curr = string("User-Agent: ") + msg.headers[string("User-Agent")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("P-Access-Network-Info: ") + msg.headers[string("P-Access-Network-Info")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("P-Early-Media: ") + msg.headers[string("P-Early-Media")] + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Allow: ") + msg.headers[string("Allow")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Session-Expires: ") + msg.headers[string("Session-Expires")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Min-SE: ") + msg.headers[string("Min-SE")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Contact: <sip:") + ip + string(";transport=UDP>") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Content-Type: ") + msg.headers[string("Content-Type")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("\r\n");
	copyStringToBuff(curr, &p, &len);

    copyStringToBuff(msg.content, &p, &len);

	
	sendUdpMessage(sendBuf, len, phoneState[toPhone].ip.c_str());
	    
  
// 给UE 1 回复 100 TRYING

    p = sendBuf;
    len = 0;

	curr = "SIP/2.0 100 Trying\r\n";
    copyStringToBuff(curr, &p, &len);

	curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	//curr = string("To: ") + msg.headers[string("To")] + string("\r\n");
	curr = string("To: \"") + toPhone + string("\"<tel:") + toPhone + string(";phone-context=") + msg.domain + string(">\r\n");
	 //curr = string("To: <sip:") + toPhone + string("@") + phoneState[toPhone].ip + string(":5060>")+ string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Call-ID: ") + msg.headers[string("Call-ID")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Via: SIP/2.0/TCP ") +  phoneState[fromPhone].ip + string(":5060;rport=5060;") + 
		   splitString(msg.headers[string("Via")], ';')[1] + 
		   string("\r\n");
	copyStringToBuff(curr, &p, &len);

	 curr = string("Contact: <sip:") + ip + string(":5060;transport=UDP>") + string("\r\n");
        copyStringToBuff(curr, &p, &len);
 //	curr = string("User-Agent: ") + msg.headers[string("User-Agent")] + string("\r\n");
  //  copyStringToBuff(curr, &p, &len);

	curr = string("Content-Length:0\r\n\r\n");
    copyStringToBuff(curr, &p, &len);
	
	int n = write(msg.fd, sendBuf, len);
    if(n < 0)
    {
        cout << "response invite tcp message failed !!!" << endl;
    }
    

    free(sendBuf);
}

void ack(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;
sleep(1);	
	//To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217
	string toPhone = msg.headers[string("To")].substr( msg.headers[string("To")].find(':') + 1,
				msg.headers[string("To")].find(';') - msg.headers[string("To")].find(':') - 1);

   //ACK sip:1012@182.0.3.112:5060 SIP/2.0
	string curr = string("ACK sip:") + toPhone + string("@") + phoneState[toPhone].ip + string(":5060 SIP/2.0\r\n");
   // string curr = msg.firstLine;
    copyStringToBuff(curr, &p, &len);

    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	//To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3    // 自己构造
    //curr = string("To: <sip:") + toPhone + string("@") + phoneState[toPhone].ip + string(":5060>;tag=") + phoneState[toPhone].toTag + string("\r\n");
    curr = string("To: <sip:") + toPhone + string("@") + phoneState[toPhone].ip + string(":5060>;") + magicToTag + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Call-ID: ") + phoneState[toPhone].callId +  string("\r\n");
	copyStringToBuff(curr, &p, &len);

    curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    //curr = string("Via: ") + msg.headers[string("Via")] + string("\r\n");  // 这里创建新的ViaBRanch即可，不必记录
    curr = string("Via: SIP/2.0/UDP ") + ip + string(":5060;branch=") + phoneState[toPhone].viaBranch + string("\r\n");
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

    curr = string("Contact: <sip:") + ip + string(":5060;transport=UDP>") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

    curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("\r\n");
    copyStringToBuff(curr, &p, &len);

	sendUdpMessage(sendBuf, len, phoneState[toPhone].ip.c_str());
	
    free(sendBuf);

    return ;
}

void bye(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

	string ipCurrent = msg.headers[string("Via")].substr( msg.headers[string("Via")].find(' ') + 1,
					   msg.headers[string("Via")].find(':') - msg.headers[string("Via")].find(' ') - 1);

	string fromPhone = msg.headers[string("From")].substr( msg.headers[string("From")].find(':') + 1,
					   msg.headers[string("From")].find('@') - msg.headers[string("From")].find(':') - 1);
	string fromPhoneIp = phoneState[fromPhone].ip;
	string toPhone = msg.headers[string("To")].substr( msg.headers[string("To")].find(';') + 1,
					   msg.headers[string("To")].find(';') - msg.headers[string("To")].find(':') - 1);
	string toPhoneIp = phoneState[toPhone].ip;

	string phone = toPhone;
	if(ipCurrent == toPhoneIp) phone = fromPhone;

	

    //string curr = string("ACK sip:1012@182.0.3.112:5060 SIP/2.0");
	string curr = string("BYE sip:") + phone + string("@") + phoneState[phone].ip + string(":5060 SIP/2.0\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    //curr = string("To: ") + msg.headers[string("To")] + string("\r\n");
	curr = string("To: <sip:") + toPhone + string("@") + phoneState[toPhone].ip + string(":5060>;tag=") + phoneState[phone].toTag + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Call-ID: ") + phoneState[toPhone].callId + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    //curr = string("Via: ") + msg.headers[string("Via")] + string("\r\n");
	curr = string("Via: SIP/2.0/UDP ") + ip + string(":5060;branch=") + newBranch() + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Max-Forwards: ") + msg.headers[string("Max-Forwards")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("\r\n");
    copyStringToBuff(curr, &p, &len);

	sendUdpMessage(sendBuf, len, phoneState[phone].ip.c_str());

    free(sendBuf);

    return ;
}

void subscribe(SipMessage msg)
{
// 200 OK

	cout << "INFO: enter SUBSCRIBE procudure " << endl;
    char* sendBuf = (char*)(malloc(10000));
	string curr;
    char* p = sendBuf;
    int len = 0;

     string fromImsi = msg.headers[string("From")].substr( msg.headers[string("From")].find(':') + 1,
                   msg.headers[string("From")].find('@') - msg.headers[string("From")].find(':') - 1);

    //cout << "Debug : Regist : fomeImsi = " << fromImsi << endl;


    copyStringToBuff(string("SIP/2.0 200 OK\r\n"), &p, &len);

	curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	string tag = newTag();
	curr = string("To: ") + msg.headers[string("To")] + string(";tag=") +  tag +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Call-ID: ") + msg.headers[string("Call-ID")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("CSeq: ") + msg.headers[string("CSeq")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Expires: ") + msg.headers[string("Expires")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	//Via: SIP/2.0/UDP 182.0.3.112:5060;branch=z9hG4bKaaecbT3QIWzL-OaaaW2Y;rport
	//Via: SIP/2.0/UDP 182.0.3.112:5060;rport=5060;branch=z9hG4bKaaecbT3QIWzL-OaaaW2Y 
	string temp = splitString(msg.headers[string("Via")], ';')[1];
	//cout << "Debug : Register : Via branch : " << temp << endl;
	curr = string("Via:SIP/2.0/TCP ") + msg.ip + string(":5060;rport=5060;") + temp + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Contact: ") + msg.headers[string("To")] + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Content-Length: ") + msg.headers[string("Content-Length")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);	


    curr = string("\r\n");
    copyStringToBuff(curr, &p, &len);


	sendUdpMessage(sendBuf, len, msg.ip.c_str());


   // int n = write(msg.fd, sendBuf, len);
   // if(n < 0)
   // {
   //     cout << "response tcp message failed !!!" << endl;
   // }

//  NOTIFY
 

    p = sendBuf;
    len = 0;


	curr = string("NOTIFY sip:") + imsiPhone[fromImsi] + string("@") + msg.ip + string(":5060 SIP/2.0\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("From: ") + msg.headers[string("To")] + string(";tag=") +  tag +  string("\r\n"); // TRICK
    copyStringToBuff(curr, &p, &len);

	curr = string("To: ") + msg.headers[string("From")] + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Call-ID: ") + msg.headers[string("Call-ID")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);	

	curr = string("CSeq:1 NOTIFY") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	//Via: SIP/2.0/UDP 10.88.120.34:5060;branch=z9hG4bK-418a3fd9-19577b-62fdb882-7fa7f6a469d8
	curr = string("Via:SIP/2.0/UDP ") + ip + string(":5060;branch=z9hG4bK-41") + tag +   string("\r\n");
	copyStringToBuff(curr, &p, &len);


	curr = string("Subscription-State:active;expires=599999") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Event:reg") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Max-Forwards: ") + msg.headers[string("Max-Forwards")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Contact: ") + msg.headers[string("To")] + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Content-Length: ") + msg.headers[string("Content-Length")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);	


    curr = string("\r\n");
    copyStringToBuff(curr, &p, &len);

	//sendUdpMessage(char * buf, int len, char * ip)

	sendUdpMessage(sendBuf, len, msg.ip.c_str());

	free(sendBuf);

    return ;

	
/*
	curr = string("Via:SIP/2.0/TCP ") + msg.ip + string(":5060;branch=z9hG4bKNOT000001PX00003300000300000;received=") + msg.ip + string(";rport=5060")+ string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Record-Route:<sip:PX000000330300000000@1") + ip + string(":5060;lr>\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Max-Forward: ") + std::to_string(std::atoi(msg.headers[string("Max-Forward")].c_str()) - 1) + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("To: ") + msg.headers[string("To")] + string(";tag=") +  tag +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Content-Type:application/reginfo+xml") + string("\r\n");
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

*/

	
}

void regist(SipMessage msg)
{

    cout << "INFO: begin regist procesure " << endl;

    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0; 

    string fromImsi = msg.headers[string("From")].substr( msg.headers[string("From")].find(':') + 1,
                   msg.headers[string("From")].find('@') - msg.headers[string("From")].find(':') - 1);
	string fromPhone = imsiPhone[fromImsi];
	string fromIp = msg.headers[string("Via")].substr( msg.headers[string("Via")].find(' ') + 1,
					msg.headers[string("Via")].find(':') - msg.headers[string("Via")].find(' ') - 1);
	State state;
	state.imsi = fromImsi;
	state.ip = fromIp;
	phoneState[fromPhone] = state;

    copyStringToBuff(string("SIP/2.0 200 OK\r\n"), &p, &len);

	string curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("To: ") + msg.headers[string("To")] + string(";tag=") +  newTag() +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Call-ID: ") + msg.headers[string("Call-ID")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("CSeq: ") + msg.headers[string("CSeq")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	// Via: SIP/2.0/TCP 182.0.3.112:5060;branch=z9hG4bKX9dcbT3QIWzL-OaaaW2Y;rport
	// Via: SIP/2.0/TCP 182.0.3.112:5060;rport=5060;branch=z9hG4bKX9dcbT3QIWzL-OaaaW2Y
	string temp = splitString(msg.headers[string("Via")], ';')[1];
	//cout << "Debug : Register : Via branch : " << temp << endl;
	curr = string("Via: SIP/2.0/TCP ") + msg.ip + string(":5060;rport=5060;") + temp + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Contact: <sip:") + fromImsi + string("@") + msg.ip + string(":5060>;expires=") + msg.headers[string("Expires")] + 
				  string(";+sip.instance=") + msg.contactMap[string("+sip.instance")] +  
				  string(";+g.3gpp.accesstype=") + msg.contactMap[string("+g.3gpp.accesstype")] + 
				  string(";+g.3gpp.icsi-ref=") + msg.contactMap[string("+g.3gpp.icsi-ref")] +
				  string("\r\n");
	copyStringToBuff(curr, &p, &len);

	//P-Associated-URI: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>,<tel:1012> // 新
	curr = string("P-Associated-URI: <sip:")+ msg.phone + string("@") +  msg.domain + string(">,<tel:") + imsiPhone[fromImsi] + string(">") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Content-Length: ") + msg.headers[string("Content-Length")] +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("\r\n");
    copyStringToBuff(curr, &p, &len); 

    	//cout << "len = " << len << endl;

    int n = write(msg.fd, sendBuf, len);

    free(sendBuf);

    return ;
}

void ring183(SipMessage msg)
{
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

cout << "INFO: enter ring 183 procesure " << endl;

	// To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3
    string toPhone = msg.headers[string("To")].substr(msg.headers[string("To")].find(':') + 1 ,
        msg.headers[string("To")].find('@') - msg.headers[string("To")].find(':') - 1);
    //cout << "Debug ring183 toPhone " << toPhone << endl;
    //
    // test
   //To: <sip:1012@182.0.3.112:5060>;tag=GGicbT8
   magicToTag = splitString(msg.headers[string("To")], ';')[1];

    string toImsi = phoneImsi[toPhone];
    //cout << "Debug: toImsi = " << toImsi << endl;

	//From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
	string fromPhone = msg.headers[string("From")].substr(msg.headers[string("From")].find(':') + 1 ,
        msg.headers[string("From")].find('@') - msg.headers[string("From")].find(':') - 1);
	cout << "Debug: ring183 fromPhone = " << fromPhone;

	State state = phoneState[toPhone];
	//To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3
	state.toTag = newTag();
	//state.toTag = msg.headers[string("To")].substr(msg.headers[string("To")].find('=') + 1,
	//			  msg.headers[string("To")].size() - msg.headers[string("To")].find('=') - 1);
	phoneState[toPhone] = state;

	state = phoneState[fromPhone];
	state.toTag = newTag();
	phoneState[fromPhone] = state;
	
//  begin
/*
SIP/2.0 183 Session Progress
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217  // 修改to的tag变成自己、、而且这个内容等同于自己再构造一遍
Call-ID: ttecbbPKc@182.0.3.113                               // 直接替换掉call-ID
CSeq: 1 INVITE
P-Preferred-Identity: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Via: SIP/2.0/TCP 182.0.3.113:5060;rport=5060;branch=z9hG4bKrvecbbPKcWj1EbefaynY                  // 修改via，，更换branch参数值，貌似这一个值气态地方都用不到
Contact: <sip:10.88.120.34:5060;transport=UDP>  // 自己构造
Content-Type: application/sdp
Content-Length: 335

*/

    string curr = "SIP/2.0 183 Session Progress\r\n";
    copyStringToBuff(curr, &p, &len);

	curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	// To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217  // 修改to的tag变成自己、、而且这个内容等同于自己再构造一遍
	//curr = string("To: ") + msg.headers[string("To")] + string("\r\n");
    //copyStringToBuff(curr, &p, &len);
	curr = string("To: \"") + toPhone + string("\"<tel:") + toPhone + string(";phone-context=")
           + msg.domain + string(";tag=") + phoneState[toPhone].toTag + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Call-ID: ") + phoneState[fromPhone].callId +  string("\r\n");
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

	//Via: SIP/2.0/TCP 182.0.3.113:5060;rport=5060;branch=z9hG4bKrvecbbPKcWj1EbefaynY
	curr = string("Via: SIP/2.0/TCP ") + phoneState[fromPhone].ip + string(":5060;rport=5060;") + phoneState[fromPhone].viaBranch +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Contact: <sip:") + ip + string(":5060;transport=UDP>") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

	curr = string("Content-Type: ") + msg.headers[string("Content-Type")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("\r\n");
	copyStringToBuff(curr, &p, &len);

    copyStringToBuff(msg.content, &p, &len);

    curr = string("\r\n");
	copyStringToBuff(curr, &p, &len);



	int n = write(phoneState[fromPhone].fd, sendBuf, len);
    if(n < 0)
    {
        cout << "Error: regist response tcp message failed !!!" << endl;
    }

    free(sendBuf);

	return ;
	
// end

	//---------------------------------
/*
    curr = string("Via: ") + msg.headers[string("Via")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Record-Route: ") + msg.headers[string("Record-Route")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Contact: ") + msg.headers[string("Contact")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    

    curr = string("Call-ID: ") + msg.headers[string("Call-ID")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("To: ") + msg.headers[string("To")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Allow: ") + msg.headers[string("Allow")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Require: ") + msg.headers[string("Require")] + string("\r\n");
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

*/
}

void ring180(SipMessage msg)
{
/*

SIP/2.0 180 Ringing
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
																	     6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217 // 修改to的tag变成自己、、而且这个内容等同于自己再构造一遍
Call-ID: ttecbbPKc@182.0.3.113         // 修改call-ID
CSeq: 1 INVITE             
P-Preferred-Identity: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Via: SIP/2.0/TCP 182.0.3.113:5060;rport=5060;branch=z9hG4bKrvecbbPKcWj1EbefaynY  // 自己构造、这个值可能是唯一的
Contact: <sip:10.88.120.34:5060;transport=UDP>                                // 自己构造
Content-Length: 0

*/
	cout << "INFO: enter 180 ring procedure" << endl;
    char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;

	string fromPhone = msg.headers[string("From")].substr(msg.headers[string("From")].find(':') + 1 ,
    				   msg.headers[string("From")].find('@') - msg.headers[string("From")].find(':') - 1);
	

    // To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3
    string toPhone = msg.headers[string("To")].substr(msg.headers[string("To")].find(':') + 1 ,
        msg.headers[string("To")].find('@') - msg.headers[string("To")].find(':') - 1);
    //cout << "Debug ring180 toPhone " << toPhone << endl;

    string toIp = msg.headers[string("To")].substr(msg.headers[string("To")].find('@') + 1 ,
        msg.headers[string("To")].find('>') - msg.headers[string("To")].find('@') - 1);
    //cout << "Debug ring180 toIp = " << toIp << endl;

    string toImsi = phoneImsi[toPhone];
    //cout << "Debug ring180 toImsi = " << toImsi << endl;

    string curr = "SIP/2.0 180 Ringing\r\n";
    copyStringToBuff(curr, &p, &len);

    curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    // To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217  // 修改to的tag变成自己、、而且这个内容等同于自己再构造一遍
	//curr = string("To: ") + msg.headers[string("To")] + string("\r\n");
    //copyStringToBuff(curr, &p, &len);
	curr = string("To: \"") + toPhone + string("\"<tel:") + toPhone + string(";phone-context=")
           + msg.domain + string(";tag=") +  phoneState[toPhone].toTag + string("\r\n");
    copyStringToBuff(curr, &p, &len);

    //curr = string("Call-ID: ") + phoneCallId[msg.phone] + string("\r\n");
	curr = string("Call-ID: ") + phoneState[fromPhone].callId +  string("\r\n");
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

	//curr = string("Via: SIP/2.0/TCP : ") + msg.ip + string(":5060;rport=5060;branch=") + phoneBranch[msg.phone] +  string("\r\n");
	curr = string("Via: SIP/2.0/TCP ") + phoneState[fromPhone].ip + string(":5060;rport=5060;") + phoneState[fromPhone].viaBranch +  string("\r\n");
    copyStringToBuff(curr, &p, &len);

    curr = string("Contact: <sip:") + ip + string(":5060;transport=UDP>") + string("\r\n");
	copyStringToBuff(curr, &p, &len);

    curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    copyStringToBuff(curr, &p, &len);

	
	curr = string("\r\n");
	copyStringToBuff(curr, &p, &len);
	
	int n = write(phoneState[fromPhone].fd, sendBuf, len);
    if(n < 0)
    {
        cout << "Error: regist response tcp message failed !!!" << endl;
    }


    free(sendBuf);



    return ;
}

void ok200(SipMessage msg)
{
/*
SIP/2.0 200 OK
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217  // 修改to的tag
Call-ID: ttecbbPKc@182.0.3.113     // 修改call-ID
CSeq: 1 INVITE
P-Preferred-Identity: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Supported: 100rel,histinfo,join,norefersub,replaces,timer
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
Via: SIP/2.0/TCP 182.0.3.113:5060;rport=5060;branch=z9hG4bKrvecbbPKcWj1EbefaynY   // 自己构造、这个值可能是随机的
Contact: <sip:10.88.120.34:5060;transport=UDP>    // 随机构造
Content-Type: application/sdp
Content-Length: 335

*/

	cout << "INFO: Enter 200 OK procedure " << endl;
	char* sendBuf = (char*)(malloc(10000));
    char* p = sendBuf;
    int len = 0;
	
	// NOTIFY
	if(msg.headers[string("CSeq")].find("NOTIFY") < msg.headers[string("CSeq")].size())
	{
		cout << "INFO: Enter NOTIFY 200 OK procedure " << endl;
		return;
	}
	
	if(msg.headers[string("CSeq")].find("INVITE") < msg.headers[string("CSeq")].size())
	{
		cout << "INFO: Enter INVITE 200 OK procedure " << endl;

		string fromPhone = msg.headers[string("From")].substr(msg.headers[string("From")].find(':') + 1 ,
    				   msg.headers[string("From")].find('@') - msg.headers[string("From")].find(':') - 1);
		
		string toPhone = msg.headers[string("To")].substr(msg.headers[string("To")].find(':') + 1 ,
        msg.headers[string("To")].find('@') - msg.headers[string("To")].find(':') - 1);
    	//cout << "Debug 200OK toPhone " << toPhone << endl;
		
		string curr = string("SIP/2.0 200 OK\r\n");
    	copyStringToBuff(curr, &p, &len);
	
		curr = string("From: ") + msg.headers[string("From")] + string("\r\n");
    	copyStringToBuff(curr, &p, &len);

		// To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217  // 修改to的tag变成自己、、而且这个内容等同于自己再构造一遍
		curr = string("To: \"") + toPhone + string("\"<tel:") + toPhone + string(";phone-context=")
	           + msg.domain + string(">;tag=") + phoneState[toPhone].toTag + string("\r\n");
	    copyStringToBuff(curr, &p, &len);

		curr = string("Call-ID: ") + phoneState[fromPhone].callId + string("\r\n");
    	copyStringToBuff(curr, &p, &len);

		curr = string("CSeq: ") + msg.headers[string("CSeq")] + string("\r\n");
    	copyStringToBuff(curr, &p, &len);
	
		curr = string("P-Preferred-Identity: ") + msg.headers[string("P-Preferred-Identity")] + string("\r\n");
		copyStringToBuff(curr, &p, &len);
	
		curr = string("P-Access-Network-Info: ") + msg.headers[string("P-Access-Network-Info")] + string("\r\n");
		copyStringToBuff(curr, &p, &len);

		curr = string("Supported: ") + msg.headers[string("Supported")] + string("\r\n");
    	copyStringToBuff(curr, &p, &len);
		
		curr = string("User-Agent: ") + msg.headers[string("User-Agent")] + string("\r\n");
		copyStringToBuff(curr, &p, &len);

		 curr = string("Allow: ") + msg.headers[string("Allow")] + string("\r\n");
    	copyStringToBuff(curr, &p, &len);

		//curr = string("Via: SIP/2.0/TCP : ") + msg.ip + string(":5060;rport=5060;branch=") + phoneBranch[msg.phone] +  string("\r\n");
		curr = string("Via: SIP/2.0/TCP ") + phoneState[fromPhone].ip + string(":5060;rport=5060;") + phoneState[fromPhone].viaBranch +  string("\r\n");
    	copyStringToBuff(curr, &p, &len);

		 curr = string("Contact: <sip:") + ip + string(":5060;transport=UDP>") + string("\r\n");
		copyStringToBuff(curr, &p, &len);

		curr = string("Content-Type: ") + msg.headers[string("Content-Type")] + string("\r\n");
    	copyStringToBuff(curr, &p, &len);

    	curr = string("Content-Length: ") + msg.headers[string("Content-Length")] + string("\r\n");
    	copyStringToBuff(curr, &p, &len);

	
		curr = string("\r\n");
		copyStringToBuff(curr, &p, &len);

		copyStringToBuff(msg.content, &p, &len);

		int n = write(phoneState[fromPhone].fd, sendBuf, len);
	    if(n < 0)
	    {
	        cout << "Error: 200 OK TO UE  failed !!!" << endl;
	    }

		
		
	}
	
	free(sendBuf);
	
    return;	
    
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

        if(msg.firstLine.at(1) == 'I' && msg.firstLine.at(8) == '1' 
                        && msg.firstLine.at(9) == '8' && msg.firstLine.at(10) == '3') // ring 183
            ring183(msg);

        if(msg.firstLine.at(1) == 'I' && msg.firstLine.at(8) == '1' 
                        && msg.firstLine.at(9) == '8' && msg.firstLine.at(10) == '0')  // ring 180
            ring180(msg);
        if(msg.firstLine.at(1) == 'I' && msg.firstLine.at(8) == '2' 
                        && msg.firstLine.at(9) == '0' && msg.firstLine.at(10) == '0') // ok 200
            ok200(msg);
    }
    else if(msg.firstLine.at(0) == 'B')
    {
        bye(msg); // BYE
    }
    else
    {
        cout << "Error: receive bad sip message, message is : " << msg.firstLine << endl;
    }
}

#endif
