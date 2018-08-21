//
// Created by zwk on 18-7-27.
//

#ifndef SEARCH_DISPLAYRST_H
#define SEARCH_DISPLAYRST_H

#include <string>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include "Document.h"

using namespace std;
const unsigned  int RstPerPage=20;
const string INF_INFO_NAME("./Data/sun.iidx");
const string DOC_IDX_NAME("./Data/Doc.idx");
const string RAWPAGE_FILE_NAME("./Data/Tianwang.raw.1");

//展示搜索结果
class DisplayRst
{
public:
    DisplayRst();
    ~DisplayRst();

    bool ShowTop();
    bool ShowMiddle(string, float, unsigned, unsigned);
    bool ShowBelow(vector<string>&, set<string> &, vector<DocIdx> &, unsigned);
private:


};

#endif //SEARCH_DISPLAYRST_H
