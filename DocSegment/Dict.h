//
// Created by zwk on 18-7-26.
//

#ifndef DOCSEGMENT_DICT_H
#define DOCSEGMENT_DICT_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>




using namespace std;



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

#endif //DOCSEGMENT_DICT_H
