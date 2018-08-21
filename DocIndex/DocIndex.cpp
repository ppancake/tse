//
// Created by zwk on 18-7-26.
//

#include "MD5.h"
#include "Url.h"
#include "Document.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;



int main()
{
    //读取---天网数据打开
    ifstream ifs("Tianwang.raw.1");

    if (!ifs)
    {
        cerr << "Cannot open " << "Tianwang.raw.1" << " for input\n";
        return -1;
    }
    //存储输出---Url.idx，Doc.idx，DocId2Url.idx
    ofstream ofsUrl("Url.idx", ios::in | ios::out | ios::trunc | ios::binary);
    if (!ofsUrl)
    {
        cout << "error open file " << endl;
    }

    ofstream ofsDoc("Doc.idx", ios::in | ios::out | ios::trunc | ios::binary);
    if (!ofsDoc)
    {
        cout << "error open file " << endl;
    }

    ofstream ofsDocId2Url("DocId2Url.idx", ios::in | ios::out | ios::trunc | ios::binary);
    if (!ofsDocId2Url)
    {
        cout << "error open file " << endl;
    }


    int cnt = 0;
    string strLine;
    Url iUrl;
    Document iDocument;
    MD5 iMD5;

    int offset = ifs.tellg();	//代表当前get 流指针的位置(用tellg)
    while (getline(ifs, strLine))
    {
        if (strLine[0] == '\0' || strLine[0] == '#' || strLine[0] == '\n')
        {
            offset = ifs.tellg();
            continue;
        }

        //每一个version为一组
        if (!strncmp(strLine.c_str(), "version: 1.0", 12))
        {
            //下一行,没有内容就跳出
            //url行有内容就复制到md5和url的成员变量中
            //length行得到文件长度
            if (!getline(ifs, strLine)) break;

            if (!strncmp(strLine.c_str(), "url: ", 4))
            {
                iUrl.m_Url = strLine.substr(5);
                iMD5.GenerateMD5((unsigned char*)iUrl.m_Url.c_str(), iUrl.m_Url.size());
                iUrl.m_Checksum = iMD5.ToString();
            }
            else
                continue;


            while (getline(ifs, strLine))
            {
                if (!strncmp(strLine.c_str(), "length: ", 8))
                {
                    sscanf(strLine.substr(8).c_str(), "%d", &(iDocument.m_Length));
                    break;
                }
            }

            getline(ifs, strLine);

            iDocument.m_DocId = cnt;
            iDocument.m_Pos = offset;

            char *content = new char[iDocument.m_Length + 1];
            memset(content, 0, iDocument.m_Length + 1);
            //读取？？？现在document有内容？？
            ifs.read(content, iDocument.m_Length);
            iMD5.GenerateMD5((unsigned char*)content, iDocument.m_Length);
            iDocument.m_Checksum = iMD5.ToString();
            delete content;
            content = NULL;



            //刚才得到的url  length  md5码全部输出
            ofsUrl << iUrl.m_Checksum;
            ofsUrl << "\t" << iDocument.m_DocId << endl;

            ofsDoc << iDocument.m_DocId;
            ofsDoc << "\t" << iDocument.m_Pos;
            ofsDoc << "\t" << iDocument.m_Checksum << endl;

            ofsDocId2Url << iDocument.m_DocId;
            ofsDocId2Url << "\t" << iUrl.m_Url << endl;

            cnt++;

        }

        offset = ifs.tellg();
    }

    //输出文档中doc的总数和偏移
    ofsDoc << cnt;
    ofsDoc << "\t" << offset << endl;

    return 0;
}
