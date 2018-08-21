//
// Created by zwk on 18-7-27.
//

#ifndef SEARCH_DICT_H
#define SEARCH_DICT_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>




using namespace std;

const string DICTFILENAME("words.dict");


//该类主要是一种词汇频率的集合
class Dict
{
public:
    Dict();
    ~Dict();

    int getFreq(string s) ;
    bool isWord(string s) const;
    void addFreq(string s,int n);

private:
    map<string ,int> mapDict;
    void openDict();
};



#endif //SEARCH_DICT_H
