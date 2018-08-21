//
// Created by zwk on 18-7-26.
//

#include "HzSeg.h"
#include <assert.h>
const unsigned int MAX_WORD_LENGTH = 8;

const string SEPARATOR("/  ");

HzSeg::HzSeg()
{
}

HzSeg::~HzSeg()
{
}



//string分词
//正向减字最大匹配法MM
string HzSeg::SegmentHzStrMM(Dict &dict, string s)const
{
    string ret = "";
    while (!s.empty())
    {
        unsigned int len = s.size();
        if (len > MAX_WORD_LENGTH)
            len = MAX_WORD_LENGTH;//最大词长


        string word = s.substr(0, len);
        bool isword = dict.isWord(word);
        while (len > 2 && isword == false)	//为啥是>2
        {
            len = len - 2;
            word = word.substr(0, len);
            isword = dict.isWord(word);
        }
        ret += word + SEPARATOR;

        s = s.substr(word.size());
    }

    return ret;

}


//句子，就要处理标点什么的吗
string HzSeg::SegmentSentenceMM (Dict &dict, string s1) const {
    string s2 = "";
    unsigned int i, len;

    while (!s1.empty()) {
        unsigned char ch = (unsigned char) s1[0];
        if (ch < 128) { // deal with ASCII
            i = 1;
            len = s1.size();
            while (i < len && ((unsigned char) s1[i] < 128)
                   && (s1[i] != 10) && (s1[i] != 13)) { // LF, CR
                i++;
            }

            if ((ch != 32) && (ch != 10) && (ch != 13)) {// SP, LF, CR
                s2 += s1.substr(0, i) + SEPARATOR;
            } else {
                if (ch == 10 || ch == 13) {
                    s2 += s1.substr(0, i);
                }
            }

            if (i <= s1.size())    // added by yhf
                s1 = s1.substr(i);
            else break;        // yhf

            continue;

        } else {
            if (ch < 176) { // 中文标点等非汉字字符
                i = 0;
                len = s1.length();

                while (i < len && ((unsigned char) s1[i] < 176) && ((unsigned char) s1[i] >= 161)
                       && (!((unsigned char) s1[i] == 161 &&
                             ((unsigned char) s1[i + 1] >= 162 && (unsigned char) s1[i + 1] <= 168)))
                       && (!((unsigned char) s1[i] == 161 &&
                             ((unsigned char) s1[i + 1] >= 171 && (unsigned char) s1[i + 1] <= 191)))
                       && (!((unsigned char) s1[i] == 163 &&
                             ((unsigned char) s1[i + 1] == 172 || (unsigned char) s1[i + 1] == 161)
                             || (unsigned char) s1[i + 1] == 168 || (unsigned char) s1[i + 1] == 169 ||
                             (unsigned char) s1[i + 1] == 186
                             || (unsigned char) s1[i + 1] == 187 || (unsigned char) s1[i + 1] == 191))) {
                    i = i + 2; // 假定没有半个汉字
                }

                if (i == 0) i = i + 2;

                // 不处理中文空格
                if (!(ch == 161 && (unsigned char) s1[1] == 161)) {
                    if (i <= s1.size())    // yhf
                        // 其他的非汉字双字节字符可能连续输出
                        s2 += s1.substr(0, i) + SEPARATOR;
                    else break; // yhf
                }

                if (i <= s1.size())    // yhf
                    s1 = s1.substr(i);
                else break;        //yhf

                continue;
            }
        }


        // 以下处理汉字串

        i = 2;
        len = s1.length();

        while (i < len && (unsigned char) s1[i] >= 176)
//    while(i<len && (unsigned char)s1[i]>=128 && (unsigned char)s1[i]!=161)
            i += 2;

        s2 += SegmentHzStrMM(dict, s1.substr(0, i));

        if (i <= len)    // yhf
            s1 = s1.substr(i);
        else break;    // yhf
    }

    return s2;
}

void HzSeg::Translate(char* SourceStr) const
{
    int i=0;
    int j=0;
    char *tempstr,tempchar1,tempchar2;

    tempstr = (char*)malloc(strlen(SourceStr) + 1);
    if(tempstr == NULL){
        return;
    }

    while (SourceStr[j])
    {
        if ((tempstr[i]=SourceStr[j])=='%'){
            if (SourceStr[j+1]>='A')
                tempchar1=((SourceStr[j+1]&0xdf)-'A')+10;
            else
                tempchar1=(SourceStr[j+1]-'0');
            if (SourceStr[j+2]>='A')
                tempchar2=((SourceStr[j+2]&0xdf)-'A')+10;
            else
                tempchar2=(SourceStr[j+2]-'0');
            tempstr[i]=tempchar1*16+tempchar2;
            j=j+2;
        }
        i++;
        j++;
    }
    tempstr[i]='\0';
    strcpy(SourceStr,tempstr);

    if(tempstr) free(tempstr);
}

/*
 * segment the image URL by '/'
 * omit the domain name
 */


string HzSeg::SegmentURL(Dict &dict, string url) const
{
    string::size_type idx, nidx;
    char *curl = (char *)url.c_str();
    this->Translate(curl);
    url = curl;
    if((idx = url.find("http://", 0)) != string::npos)
    {
        if((nidx = url.find("/", 7)) != string::npos)
        {
            url = url.substr(nidx + 1);	// cut the part of sitename
        }
    }
    idx = 0;
    while((idx = url.find("/", idx)) != string::npos)
    {
        url.replace(idx, 1, SEPARATOR);	// replace "/" with SEPARATOR "/  "
        idx += 3;
    }
    if((idx = url.rfind(".")) != string::npos)
    {
        url = url.erase(idx);	// erase the file extension
    }

    url += "/  ";

    // segment the string whose length is greater than 8 (4 HZ_chars)
    idx = 0; nidx = 0;
    bool isover = false;
    string stmp;
    while(!isover)
    {
        if((nidx = url.find(SEPARATOR, idx)) == string::npos)
            isover = true;
        if(nidx - idx > 0)
        {
            stmp = url.substr(idx, nidx-idx);
            stmp = SegmentSentenceMM(dict, stmp);
            if ( stmp.size() >= 3)
                stmp.erase(stmp.length() - 3);	// erase the tail "/  "
            url = url.replace(idx, nidx-idx, stmp);
            idx += stmp.length() + 3;
        }
        else if(nidx == string::npos && idx < url.length())
        {
            stmp = url.substr(idx);
            stmp = SegmentSentenceMM(dict, stmp);
            stmp.erase(stmp.length() - 3);
            url = url.substr(0, idx) + stmp;
        }
        else
            idx = nidx + 3;
    }

    return url;

}

