//
// Created by zwk on 18-7-26.
//

#ifndef DOCSEGMENT_MD5_H
#define DOCSEGMENT_MD5_H
#include <string>
using namespace std;
typedef unsigned char uint8;		//8字节	 1位
typedef unsigned long int uint32;	//32字节 4位
struct md5_context
{
    uint32 total[2];
    uint32 state[4];
    uint8 buffer[64];
};

class MD5
{
public:
    MD5();
    MD5(const char *md5src);
    MD5(unsigned long* md5src);

    void GenerateMD5(unsigned char* buffer, int bufferlen);

    MD5 operator +(MD5 adder);
    bool operator ==(MD5 cmper);
    unsigned long m_data[4];
    string ToString();
    ~MD5();

private:
    void md5_starts(struct md5_context *ctx);
    void md5_process(struct md5_context *ctx, uint8 data[64]);
    void md5_update(struct md5_context *ctx, uint8 *input, uint32 length);
    void md5_finish(struct md5_context *ctx, uint8 digest[16]);


};

#endif //DOCSEGMENT_MD5_H
