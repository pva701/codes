#include "IGrabber.h"

const wchar_t* IGrabber::fromCharToWChar(const char *c) {
	const size_t cSize = strlen(c)+1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs (wc, c, cSize);
	return wc;
}

const char* IGrabber::decodeBlob(const char *blob, int size) {
	DATA_BLOB DataIn, DataOut;
	DataIn.pbData = (BYTE*) blob;
	DataIn.cbData = size;
	LPWSTR pDescrOut = NULL;
	CRYPTPROTECT_PROMPTSTRUCT PromptStruct;
	int flags = 1;
	bool success = CryptUnprotectData(&DataIn,
									 &pDescrOut,
									  NULL,
									  NULL,
									  NULL,
									  flags,
									  &DataOut);
	char *decr = new char[DataOut.cbData + 1];
	decr[DataOut.cbData] = 0;
	memcpy(decr, DataOut.pbData, DataOut.cbData);
	return decr;
}