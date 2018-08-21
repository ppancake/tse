#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "Query.h"
#include "StrFun.h"


using namespace std;
Query::Query()
{
    m_Query = "";
    m_SegQuery = "";
    m_Start = 0;
    HtmlInputCount = 0;
}

Query::~Query()
{

}


//对分词后的查询语句以/  分开放入vecTerm里
void Query::ParseQuery(vector<string> &vecTerm)
{
    string::size_type idx;
    while ((idx = m_SegQuery.find("/  ")) != string::npos)
    {
        vecTerm.push_back(m_SegQuery.substr(0, idx));
        m_SegQuery = m_SegQuery.substr(idx + 3);
    }
}


//查询词只有HtmlInputs[0]？？
void Query::SetQuery()
{
    string q = HtmlInputs[0].Value;
    CStrFun::Str2Lower(q, q.size());
    m_Query = q;
}


//start到底是什么玩意
void Query::SetStart()
{
    m_Start = atoi(HtmlInputs[1].Value);
}



//一个是word，一个是DocNum,放入mapBuckets里
bool Query::GetInvLists(map<string, string> &mapBuckets) const
{
    ifstream ifsInvInfo(INF_INFO_NAME.c_str(), ios::binary);
    if (!ifsInvInfo)
    {
        cerr << "Cannot open " << INF_INFO_NAME << " for input\n";
        return false;
    }

    string strLine, strWord, strDocNum;
    while (getline(ifsInvInfo, strLine))
    {
        string::size_type idx;
        string tmp;
        idx = strLine.find("\t");
        strWord = strLine.substr(0, idx);
        strDocNum = strLine.substr(idx + 1);
        mapBuckets.insert(map<string, string>::value_type(strWord, strDocNum));
    }

    return true;
}


//将docIdx放入vecDocIdx中
bool Query::GetDocIdx(vector<DocIdx> &vecDocIdx) const
{
    ifstream ifs(DOC_IDX_NAME.c_str(), ios::binary);
    if (!ifs)
    {
        cerr << "Cannot open " << DOC_IDX_NAME << " for input\n";
        return false;
    }

    string strLine;

    while (getline(ifs, strLine))
    {
        DocIdx di;

        sscanf(strLine.c_str(), "%d%d", &di.docid, &di.offset);
        vecDocIdx.push_back(di);
    }

}



//读取参数到HtmlInputs中
int Query::GetInputs()
{
    char *mode = getenv("REQUEST_METHOD");


    cout << "Content-type: text/html\n\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "</head>\n";
    cout.flush();

    if (mode == NULL) return 1;



    int length;
    char *in_line;
    if (strcmp(mode, "POST") == 0)
    {
        length = atoi(getenv("CONTENT_LENGTH"));
        if (length == 0 || length >= 256)
            return 1;
        in_line = (char*)malloc(length + 1);
        read(STDIN_FILENO, in_line, length);
        in_line[length] = '\0';
    }
    else if (strcmp(mode, "GET") == 0)
    {
        char* inputstr = getenv("QUERY_STRING");
        length = strlen(inputstr);
        if (inputstr == 0 || length >= 256)
            return 1;
        in_line = (char*)malloc(length + 1);
        strcpy(in_line, inputstr);
    }



    char *tempstr = (char*)malloc(length + 1);
    if (tempstr == NULL)
    {
        printf("<title>Error Occurred</title>\n");
        printf("</head><body>\n");
        printf("<p>Major failure #1;please notify the webmaster\n");
        printf("</p></body></html>\n");
        fflush(stdout);
        exit(2);
    }

    int i, j = 0;
    for (i = 0; i < length; i++)
    {
        if (in_line[i] == '=')
        {
            tempstr[j] = '\0';
            CStrFun::Translate(tempstr);
            strcpy(HtmlInputs[HtmlInputCount].Name, tempstr);
            if (i == length - 1)
            {
                strcpy(HtmlInputs[HtmlInputCount].Value, "");
                HtmlInputCount++;
            }
            j = 0;
        }

        else if ((in_line[i] == '&') || (i == length - 1))
        {
            if (i == length - 1)
            {
                if (in_line[i] == '+')
                    tempstr[j] = ' ';
                else
                    tempstr[j] = in_line[i];
                j++;
            }
            tempstr[j] = '\0';
            CStrFun::Translate(tempstr);
            strcpy(HtmlInputs[HtmlInputCount].Value, tempstr);
            HtmlInputCount++;
            j = 0;
        }

        else if (in_line[i] == '+')
        {
            tempstr[j] = ' ';
            j++;
        }

        else
        {
            tempstr[j] = in_line[i];
            j++;
        }
    }
    if (in_line) free(in_line);
    if (tempstr) free(tempstr);

    return 0;
}



//记载日志：
//时间和查询词
bool Query::RecordQueryLog() const
{
    FILE *fp;
    time_t current_time;
    char tmpbuf[20];
    unsigned int j;

    fp = fopen("/home/webg/public_html/data/imgsearch_submit.list", "a");
    if (fp == NULL)
    {
        printf("Content-type:text/html\n\nopen error.\n");
        return false;
    }

    current_time = time(NULL);
    memset(tmpbuf, 0, 20);
    sprintf(tmpbuf, "%s", ctime(&current_time));
    for (j = 0; j<strlen(tmpbuf); j++)
    {
        if (tmpbuf[j] == ' ') tmpbuf[j] = '-';
    }
    tmpbuf[j - 1] = 0;

    fprintf(fp, "%s ", tmpbuf);
    fprintf(fp, "%s ", m_Query.c_str());
    fprintf(fp, "%s", ";;;\n");
    fclose(fp);

    return 0;

}


//获得RelevantRst
//vecTerm分词后的中文查询词
bool Query::GetRelevantRst(vector<string> &vecTerm,map<string, string> &mapBuckets,
                           set<string> &setRelevantRst) const
{
    set<string> setSRst;
    bool bFirst = true;
    vector<string>::iterator itTerm = vecTerm.begin();


    for (; itTerm != vecTerm.end(); ++itTerm)
    {
        setSRst.clear();
        //如果要把一个序列（sequence）拷贝到一个容器（container）中去，通常用std::copy算法，代码如下：
        //std::copy(start, end, std::back_inserter(container));
        copy(setRelevantRst.begin(), setRelevantRst.end(),inserter(setSRst, setSRst.begin()));

        map<string, int> mapRstDoc;
        string docid;
        int doccnt;


        // map:  word，一个是DocNum
        map<string, string>::iterator itBuckets = mapBuckets.find(*itTerm);//*itTerm词汇
        if (itBuckets != mapBuckets.end())
        {
            string strBucket = (*itBuckets).second;
            string::size_type idx;
            idx = strBucket.find_first_not_of(" ");
            strBucket = strBucket.substr(idx);

            while ((idx = strBucket.find(" ")) != string::npos)
            {
                docid = strBucket.substr(0, idx);
                doccnt = 0;

                if (docid.empty())
                    continue;
                map<string, int>::iterator it = mapRstDoc.find(docid);
                if (it != mapRstDoc.end())
                {
                    doccnt = (*it).second + 1;	//doc次数+1？所以mapRstDoc是啥 docid 和doccnt
                    mapRstDoc.erase(it);
                }
                mapRstDoc.insert(pair<string, int>(docid, doccnt));
                strBucket = strBucket.substr(idx + 1);
            }

            docid = strBucket;
            doccnt = 0;
            map<string, int>::iterator it = mapRstDoc.find(docid);
            if (it != mapRstDoc.end())
            {
                doccnt = (*it).second + 1;
                mapRstDoc.erase(it);
            }
            mapRstDoc.insert(pair<string, int>(docid, doccnt));

        }

        //按mapRstDoc词频排序
        multimap<int, string, greater<int> > newRstDoc;
        map<string, int>::iterator it0 = mapRstDoc.begin();
        for (; it0 != mapRstDoc.end(); ++it0)
        {
            newRstDoc.insert(pair<int, string>((*it0).second, (*it0).first));
        }


        multimap<int, string>::iterator itNewRstDoc = newRstDoc.begin();
        setRelevantRst.clear();
        for (; itNewRstDoc != newRstDoc.end(); ++itNewRstDoc)
        {
            string docid = (*itNewRstDoc).second;

            if (bFirst == true)
            {
                setRelevantRst.insert(docid);
                continue;
            }

            if (setSRst.find(docid) != setSRst.end())
            {
                setRelevantRst.insert(docid);
            }
        }

        bFirst = false;
    }

    return true;
}