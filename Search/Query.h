//
// Created by zwk on 18-7-27.
//

#ifndef SEARCH_QUERY_H
#define SEARCH_QUERY_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unistd.h>
#include "Document.h"

using namespace std;

const int MaxNameLength = 255;
const int MaxValueLength = 2000;
const int MAXFORMITEM = 40;
const string INF_INFO_NAME("./Data/sun.iidx");
const string DOC_IDX_NAME("./Data/Doc.idx");
const string RAWPAGE_FILE_NAME("./Data/Tianwang.raw.2559638448");

//Name=value
typedef struct {
    char Name[MaxNameLength];
    char Value[MaxValueLength];
}HtmlInput_Struct;


//查询类
class Query
{
public:
    string m_Query;
    string m_SegQuery;
    unsigned int  m_Start;
private:
    int HtmlInputCount;
    HtmlInput_Struct HtmlInputs[MAXFORMITEM];
public:
    Query();
    ~Query();

    void ParseQuery(vector<string> &);

    bool GetInvLists(map<string, string> &) const;
    //bool GetDocidUrl(map<string, string> &) const;
    bool GetDocIdx(vector<DocIdx> &) const;

    int GetInputs();


    // p1: &vecTerm, p2:&mapBuckets, p3: &setRelvantRst
    bool GetRelevantRst(vector<string> &, map<string, string> &, set<string> &) const;

    bool RecordQueryLog() const;

    void SetQuery();
    void SetStart();

};

#endif //SEARCH_QUERY_H
