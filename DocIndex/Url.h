//
// Created by zwk on 18-7-26.
//

#ifndef DOCINDEX_URL_H
#define DOCINDEX_URL_H
#include <string>

using std::string;

class Url
{
public:
    string m_Url;
    string m_Checksum;	//md5码，检查是不是一样的
    int m_DocId;
public:
    Url();
    ~Url();

};

#endif //DOCINDEX_URL_H
