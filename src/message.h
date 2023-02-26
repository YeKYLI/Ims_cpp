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



void processMessage(SipMessage message)
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

        message.headers[headerParts[0]] = headers[i].substr(headerParts[0].size() + 2, headers[i].size() - 2 - headerParts[0].size());

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
    
    procedure(message);

}
 
void processBuf(char* buf, int len)
{
    
    //假设我们这里已经收到了完整的数据包
    //
    // 这是一个很重要的假设，后边所有的处理都依赖于这个假设
    
    SipMessage msg;
    msg.buf = string(buf, 0, len);
    
    processMessage(msg);
    
        
    // 检查是否是一个完整的SIP包


}

#endif

