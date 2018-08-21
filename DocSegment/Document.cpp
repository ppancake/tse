//
// Created by zwk on 18-7-26.
//
#include "Document.h"

Document::Document()
{
    m_DocId = -1;
    m_Pos = -1;
    m_Length = 0;
    m_Checksum = "";

    m_Url = "";
}

Document::~Document()
{

}


//意义何在
bool Document::ParseRecord(string &content) const
{
    return true;
}

bool Document::CleanBody(string &body) const
{
    return true;
}

//即去掉所有<>内的内容，包括<>号
//remove tag and content of scripts, css, java, embeddedobjects, comments, etc
void Document::RemoveTags(char *s)
{
    char *d = s;
    while (*s) {
        if (*s == '<') {
            while (*s && *s!='>') s++;
            if( *s == '\0') break;
            s++;
            continue;
        }

        *d++ = *s++;
    }
    *d = 0;
}
