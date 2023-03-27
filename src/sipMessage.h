#ifndef SIP_MESSAGE
#define SIP_MESSAGE

#include <string.h>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>

using std::string;
using std::map;
using std::vector;
using std::unordered_map;

static int udpSockFd = -1;
string ip;

map<string, string> imsiPhone; // waiting for optimization !!!
map<string, string> phoneImsi;

class State
{
public:
	string fromTag;
	string toTag;
	string viaBranch;
	string callId;
	string ip;
	string imsi;
	string domain;
	string pairPhone;
	int fd;
};

map<string, State> phoneState;

class SipMessage
{
    public:

    //vector<string> splitString(string str, char ch);

    string imsi;

	string phone;
	
    string domain;

	string ip; 

    string firstLine;

    map<string, string> headers;

	map<string, string> contactMap;

    string content;

	int fd;

    string buf;


} ;

void updateState(string phone, char* type, string value, int fd = 1)
{
	State state;
	if(phoneState.find(phone) != phoneState.end())
	{
		state = phoneState[phone];
	}
	if(strcmp(type, "fromTag") == 0) state.fromTag = value;
	if(strcmp(type, "toTag") == 0) state.toTag = value;
	if(strcmp(type, "viaBranch") == 0) state.viaBranch = value;
	if(strcmp(type, "callId") == 0) state.callId = value;
	if(strcmp(type, "ip") == 0) state.ip = value;
	if(strcmp(type, "imsi") == 0) state.imsi = value;
	if(strcmp(type, "domain") == 0) state.domain = value;
	if(strcmp(type, "pairPhone") == 0) state.pairPhone = value;
	if(strcmp(type, "fd") == 0) state.fd = fd;

	phoneState[phone] = state;
	
}

string cutString(string str, char left, char right)
{
   	return str.substr(str.find(left) + 1, str.find(right) - str.find(left) - 1);
}


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

#endif
