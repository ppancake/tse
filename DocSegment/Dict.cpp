//
// Created by zwk on 18-7-26.
//

#include "Dict.h"

Dict::Dict()
{
    openDict();
}

Dict::~Dict()
{
    mapDict.clear();
}

void Dict::openDict()
{
    ifstream ifsdit("words.dict");
    if (!ifsdit)
    {
        cerr << "Cannot open " << "DICTFILENAME" << " for input\n";
    }
    string strLine;
    int id, freq;
    char word[16];
    while (getline(ifsdit, strLine))
    {
        sscanf(strLine.c_str(), "%d %s %d", &id, word, &freq);//C++有啥代替的吗，，sscanf是真的好用

        mapDict.insert(map<string, int>::value_type(word, freq));
    }

    ifsdit.close();

}


bool Dict::isWord(string s) const
{
    if (mapDict.find(s) != mapDict.end())
        return true;
    return false;
}

int Dict::getFreq(string s)
{
    map<string, int>::iterator it = mapDict.find(s);
    if (it == mapDict.end())
        return -1;
    else
        return it->second;
}

void Dict::addFreq(string s, int n)
{
    if (mapDict.find(s) != mapDict.end())
        return;
    mapDict.insert(map<string, int>::value_type(s, n));
}