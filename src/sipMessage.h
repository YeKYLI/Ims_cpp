#ifndef SIP_MESSAGE
#define SIP_MESSAGE

#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;

class SipMessage
{
    public:

    vector<string> splitString(string str, char ch);

    string firstLine;

    map<string, string> headers;

    string content;        

    string buf;

} ;


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
