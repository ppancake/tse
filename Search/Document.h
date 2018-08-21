//
// Created by zwk on 18-7-27.
//

#ifndef SEARCH_DOCUMENT_H
#define SEARCH_DOCUMENT_H

#include <string>

using std::string;

typedef struct {
    int docid;
    int offset;
}DocIdx;//文件索引格式

class Document
{

public:
    //文档一些参数
    int m_DocId;	//该文档是天网的第几个
    int m_Pos;		//该文档在天网的偏移位置
    int m_Length;	//该文档的长度
    string m_Checksum;	//文档内容的MD5  内容就前面3项

    //这些跟url网页有关的
    string m_Url;
    string m_Record;
    string m_Head;
    string m_Header;
    string m_Body;
    string m_BodyNoTags;

public:
    Document();
    ~Document();


    bool ParseRecord(string &content) const;	//任何不会修改数据成员的函数都应该声明为const类型，且不能调用非const函数
    bool CleanBody(string &body) const;

    void RemoveTags(char *s);

private:

};


#endif //SEARCH_DOCUMENT_H
