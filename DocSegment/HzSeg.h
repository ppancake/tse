//
// Created by zwk on 18-7-26.
//

#ifndef DOCSEGMENT_HZSEG_H
#define DOCSEGMENT_HZSEG_H
#include <iostream>
#include <string>
#include <fstream>
#include "Dict.h"
#include <cstring>


using namespace std;

class HzSeg {
public:
    HzSeg();

    ~HzSeg();

    string SegmentSentenceMM(Dict &, string) const;

    string SegmentHzStrMM(Dict &, string) const;

    string SegmentURL(Dict &, string) const;

    void Translate(char *SourceStr) const;
};

#endif //DOCSEGMENT_HZSEG_H
