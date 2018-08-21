#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <list>

#include "Query.h"
#include "Document.h"
#include "StrFun.h"
#include "Dict.h"
#include "HzSeg.h"
#include "DisplayRst.h"

using namespace std;


int main(int argc, char* argv[])
{
    struct timeval begin_tv, end_tv;
    struct timezone tz;

    Dict iDict;
    map<string, string> dictMap, mapBuckets;
    vector<DocIdx> vecDocIdx;

    Query iQuery;
    iQuery.GetInputs();
    // current query & result page number
    iQuery.SetQuery();
    iQuery.SetStart();

    // begin to search
    gettimeofday(&begin_tv,&tz);

    iQuery.GetInvLists(mapBuckets);
    iQuery.GetDocIdx(vecDocIdx);

    HzSeg iHzSeg;
    iQuery.m_SegQuery = iHzSeg.SegmentSentenceMM(iDict,iQuery.m_Query);

    vector<string> vecTerm;
    iQuery.ParseQuery(vecTerm);

    set<string> setRelevantRst;
    iQuery.GetRelevantRst(vecTerm, mapBuckets, setRelevantRst);

    gettimeofday(&end_tv,&tz);
    // search end

    DisplayRst iDisplayRst;
    iDisplayRst.ShowTop();

    float used_msec = (end_tv.tv_sec-begin_tv.tv_sec)*1000
                      +((float)(end_tv.tv_usec-begin_tv.tv_usec))/(float)1000;

    iDisplayRst.ShowMiddle(iQuery.m_Query,used_msec,
                           setRelevantRst.size(), iQuery.m_Start);

    iDisplayRst.ShowBelow(vecTerm,setRelevantRst,vecDocIdx,iQuery.m_Start);

    return 0;

}