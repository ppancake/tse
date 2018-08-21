//
// Created by zwk on 18-7-27.
//

#ifndef SEARCH_HZSEG_H
#define SEARCH_HZSEG_H
#include <iostream>
#include <string>
#include <fstream>
#include "Dict.h"


using namespace std;
class HzSeg
{
public:
    HzSeg();
    ~HzSeg();

    string SegmentSentenceMM(Dict&, string) const;
    string SegmentHzStrMM(Dict&, string) const;
};


#endif //SEARCH_HZSEG_H
