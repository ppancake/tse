//./ForwardDocIdx Tianwang.raw.2559638448.seg > moon.fdx
#include <iostream>
#include <fstream>

using namespace std;

const string SEPARATOR("/  ");          // delimiter between words

int main(int argc, char* argv[])
{
	ifstream ifsImgInfo(argv[1]);
	if (!ifsImgInfo) {
		cerr << "Cannot open " << argv[1] << " for input\n";
		return -1;
	}

	string strLine,strDocNum;
	int cnt = 0;
	while (getline(ifsImgInfo, strLine)) {
		string::size_type idx;

		cnt++;
		//单字节都是符号，不需要放入索引
		if (cnt%2 == 1){
			strDocNum = strLine.substr(0,strLine.size());
			continue;
		}
		if (strLine[0]=='\0' || strLine[0]=='#' || strLine[0]=='\n'){
			continue;
		}

		while ( (idx = strLine.find(SEPARATOR)) != string::npos ) {
			string tmp1 = strLine.substr(0,idx);
			// 单词+文档id
			cout << tmp1 << "\t" << strDocNum << endl;
			strLine = strLine.substr(idx + SEPARATOR.size());
		}

		//if (cnt==100) break;
	}

	return 0;
}
