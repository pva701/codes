#include "Writer.h"

void Writer::confirm() {
	std::reverse(bytes.begin(), bytes.end());
	int x = bytes.size();
	bytes.push_back(x & 255);
	bytes.push_back((x >> 8) & 255);
	bytes.push_back((x >> 16) & 255);
	bytes.push_back((x >> 24) & 255);
	std::reverse(bytes.begin(), bytes.end());
}

char* Writer::toCharPtr() {
	char *c = new char[bytes.size()];
	for (int i = 0; i < bytes.size(); ++i)
		c[i] = bytes[i];
	return c;
}

int Writer::size() {
	return bytes.size();
}

Writer& Writer::operator << (const wstring& str)  {
    int len = str.size();
	*this << len;
	for (int i = 0; i < len; ++i) {
		bytes.push_back(str[i]&255);
		bytes.push_back(str[i]>>8);
	}
	return *this;
}

Writer& Writer::operator << (int x) {
	bytes.push_back((x >> 24) & 255);
	bytes.push_back((x >> 16) & 255);
	bytes.push_back((x >> 8) & 255);
	bytes.push_back(x & 255);
	return *this;
}