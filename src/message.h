#ifndef MESSAGE
#define MESSAGE

#include <vector>
#include <string>
#include <iostream>
#include <sipMessage.h>
#include <procedure.h>

using std::vector;
using std::string;
using std::cout;
using std::endl;

void processMessage(SipMessage& message)
{
    vector<string> headers =  message.splitString(message.buf, '\n');

    if(headers.size() < 1)
    {
        cout << "Error: bad message, fatal error !!!" << endl;
        return;
    }
    message.firstLine = headers[0];
    //cout << message.firstLine << endl;

    for(int i = 1; i < headers.size(); i ++)
    {
        vector<string> headerParts = message.splitString(headers[i], ':');

        if(headerParts.size() < 2)
        {
            cout << "Error: bad message  format, fatal error !!!" << endl;
            continue;
        }

	    if(headers[i].size() <  headerParts[0].size() + 2)
        {
            cout << "Error: bad header body format,  fatal error !!!" << endl;
            continue;
        }

        message.headers[headerParts[0]] = headers[i].substr(headerParts[0].size() + 2, headers[i].size() - 3 - headerParts[0].size());

        //cout << "Info: key " << headerParts[0] << " val " << message.headers[headerParts[0]] << endl;

        if(strcmp(headerParts[0].c_str(), "Content-Length") == 0) 
        {
	    int contentLen = std::stoi(message.headers[headerParts[0]]);
            message.content = message.buf.substr(message.buf.size() - contentLen, contentLen); 
            //cout << contentLen << endl;
	    //cout << message.content << endl;
            
	    break;
            
        }

    }
}
 
void processBuf(char* buf, int len, int type)
{
    //cout << buf << " | | | | " << endl;

    string& stringBuf = udpStringBuf;
    if(type == 2)
    {
        stringBuf = tcpStringBuf;
    }

    SipMessage msg;
    msg.buf = stringBuf + string(buf, 0, len);

    // 检查是否是一个完整的SIP包
    processMessage(msg);
    if(msg.headers.find(string("Content-Length")) != msg.headers.end())
    {
        int contentLen = std::stoi(msg.headers[string("Content-Length")]);
        cout << contentLen << endl;

        // 找到Content-Length在字符串的位置
        string ContentLength = string("Content-Length: ") + 
                               string(msg.headers[string("Content-Length")]) +
                               string("\r\n");
        int position = msg.buf.find(ContentLength);
        // check one
        if(position == -1)
        {
            stringBuf = msg.buf;
            return ;
        }
        // check two
        if(position + contentLen + ContentLength.size() < msg.buf.size())
        {
            stringBuf = msg.buf;
            return ;
        }
        else
        {
            stringBuf = string("");
            procedure(msg);
        }

    }
    else
    {
        stringBuf = msg.buf;
        return ;
    }
}

#endif

