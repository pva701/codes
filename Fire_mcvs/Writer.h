#ifndef WRITER_H
#define WRITER_H
#include <vector>
#include <algorithm>
using namespace std;

class Writer {
public:
	typedef unsigned char UByte;
	void confirm();
	char* toCharPtr();
	int size();
	Writer& operator << (const wstring& str);
	Writer& operator << (int x);
private:
	vector <UByte> bytes;
};

#endif