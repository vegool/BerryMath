#include <vector>
#include "stringpp.h"

std::string BerryMath::ltrim(const std::string& s)
{
    const string WHITESPACE = " \n\r\t\f\v";
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string BerryMath::rtrim(const std::string& s)
{
    const string WHITESPACE = " \n\r\t\f\v";
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string BerryMath::trim(const std::string& s)
{
    return rtrim(ltrim(s));
}

std::vector<string> BerryMath::spilt(const string &str, const string &delim)
{
    std::vector<string> spiltCollection;
    if(str.size()==0)
        return spiltCollection;
    int start = 0;
    int idx = str.find(delim, start);
    while( idx != string::npos )
    {
        spiltCollection.push_back(str.substr(start, idx-start));
        start = idx+delim.size();
        idx = str.find(delim, start);
    }
    spiltCollection.push_back(str.substr(start));
    return spiltCollection;
}
