#ifndef SIP_MESSAGE
#define SIP_MESSAGE

#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;

static int udpSockFd = -1;
static int tcpSockFd = -1; // 后期会改成在消息里、、多个UE肯定会出现问题的
string udpStringBuf("");
string tcpStringBuf("");
string ip;
map<string, string> imsiPhone;
static int count = 1;


class SipMessage
{
    public:

    vector<string> splitString(string str, char ch);

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

map<string, int> imsiFdMap; // temport design


/*

class state
{
	string imsi;

	SipMessage register;
	SipMessage subscribe;
	SipMessage invite;
	SipMessage bye;
} ;

map<string, state> stateMachine;
*/




vector<string> SipMessage::splitString(string str, char ch)
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
