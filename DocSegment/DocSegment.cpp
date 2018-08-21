#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "MD5.h"
#include "Url.h"
#include "Document.h"
#include "Dict.h"
#include "HzSeg.h"
#include "StrFun.h"



using namespace std;


//这个是对天网文件进行分词


Dict iDict;//里面有个map要一直使用。所以全局
const unsigned int HEADER_BUF_SIZE = 1024;
const unsigned int MAX_DOC_ID = 21312;




int main()
{
    string strLine, strFileName = "Tianwang.raw.1";
    Url iUrl;
    vector<Url> vecUrl;
    Document iDocument;
    vector<Document> vecDocument;
    unsigned int docId = 0;


    ifstream ifs(strFileName.c_str());
    if (!ifs)
    {
        cerr << "Cannot open tianwang.raw.1 for input\n";
        return -1;
    }

    ifstream ifsUrl("Url.sort_uniq");
    if (!ifsUrl)
    {
        cerr << "Cannot open Url.idx.sort_uniq for input\n";
        return -1;
    }

    ifstream ifsDoc("Doc.idx");
    if (!ifsDoc)
    {
        cerr << "Cannot open Doc.idx for input\n";
        return -1;
    }



    //将url.idx内容赋值iUrl再放入vecUrl中
    while (getline(ifsUrl, strLine))
    {
        char chksum[33];
        int  docid;

        memset(chksum, 0, 33);
        sscanf(strLine.c_str(), "%s%d", chksum, &docid);
        iUrl.m_Checksum = chksum;
        iUrl.m_DocId = docid;
        vecUrl.push_back(iUrl);
    }



    //将doc.idx赋值iDocument再放入vecDocument中?
    while (getline(ifsDoc, strLine))
    {
        int docid, pos;
        char chksum[33];

        memset(chksum, 0, 33);
        sscanf(strLine.c_str(), "%d%d%s", &docid, &pos, chksum);
        iDocument.m_DocId = docid;
        iDocument.m_Pos = pos;
        iDocument.m_Checksum = chksum;
        vecDocument.push_back(iDocument);
    }


    //输出到分词文件，通过docid对每个文档分词，每个内容都是他的pos到下一个pos之间
    strFileName += ".seg";
    ofstream fout(strFileName.c_str(), ios::in | ios::out | ios::trunc | ios::binary);


    //通过文档id顺序，对每个网页进行分词
    for (docId = 0; docId < MAX_DOC_ID; docId++)
    {
        int length = vecDocument[docId + 1].m_Pos - vecDocument[docId].m_Pos - 1;//偏移差
        char *pContent = new char[length + 1];
        memset(pContent, 0, length + 1);
        ifs.seekg(vecDocument[docId].m_Pos);	//移到开始的地方
        ifs.read(pContent, length);


        char *s;
        s = pContent;
        //对内容进行解析
        //首先跳过版本头,知道出现连续两个换行
        int bytesRead = 0, newlines = 0;
        while (newlines != 2 && bytesRead != HEADER_BUF_SIZE - 1)
        {
            if (*s == '\n')
                newlines++;
            else
                newlines = 0;
            s++;
            bytesRead++;
        }
        if (bytesRead == HEADER_BUF_SIZE - 1) continue;

        //再跳过html返回头
        bytesRead = 0, newlines = 0;
        while (newlines != 2 && bytesRead != HEADER_BUF_SIZE - 1)
        {
            if (*s == '\n')
                newlines++;
            else
                newlines = 0;
            s++;
            bytesRead++;
        }
        if (bytesRead == HEADER_BUF_SIZE - 1) continue;


        iDocument.RemoveTags(s);
        iDocument.m_BodyNoTags = s;
        delete[] pContent;
        pContent = NULL;


        string strLine = iDocument.m_BodyNoTags;
        CStrFun::ReplaceStr(strLine, "&nbsp;", " ");//将html中的%nbsp换成空格，因为这个就是空格
        CStrFun::EmptyStr(strLine); // 将ttml中的 " \t\r\n" 设置为 " "

        HzSeg iHzSeg;
        strLine = iHzSeg.SegmentSentenceMM(iDict, strLine);
        fout << docId << endl << strLine;
        fout << endl;
    }

    return 0;
}



