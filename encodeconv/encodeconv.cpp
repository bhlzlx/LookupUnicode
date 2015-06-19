// encodeconv.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "encodeconv.h"
#include "resource.h"
#include <cstdio>
#include <commctrl.h>
#include <Windows.h>
#include <iconv.h>
#include <string>

using namespace std;

#define MAX_LOADSTRING 100

iconv_t iconv_handle;

wstring ANSIToUnicode( const string& str )
{
 int  len = 0;
 len = str.length();
 int  unicodeLen = ::MultiByteToWideChar( CP_ACP,
            0,
            str.c_str(),
            -1,
            NULL,
            0 );  
 wchar_t *  pUnicode;  
 pUnicode = new  wchar_t[unicodeLen+1];  
 memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  
 ::MultiByteToWideChar( CP_ACP,
         0,
         str.c_str(),
         -1,
         (LPWSTR)pUnicode,
         unicodeLen );  
 wstring  rt;  
 rt = ( wchar_t* )pUnicode;
 delete  pUnicode; 
 
 return  rt;  
}
string UnicodeToANSI( const wstring& str )
{
 char*     pElementText;
 int    iTextLen;
 // wide char to multi char
 iTextLen = WideCharToMultiByte( CP_ACP,
         0,
         str.c_str(),
         -1,
         NULL,
         0,
NULL,
         NULL );
 pElementText = new char[iTextLen + 1];
 memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
 ::WideCharToMultiByte( CP_ACP,
         0,
         str.c_str(),
         -1,
         pElementText,
         iTextLen,
         NULL,
         NULL );
 string strText;
 strText = pElementText;
 delete[] pElementText;
 return strText;
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
		case WM_INITDIALOG:
		{
			::SetWindowPos(hwndDlg,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
		return TRUE;

		case WM_CLOSE:
		{
			EndDialog(hwndDlg, 0);
		}
		return TRUE;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_CHAR:
				{
					switch(HIWORD(wParam))
					{
					case EN_UPDATE:
						{
							unsigned short word[6] = {0};
							GetDlgItemText(hwndDlg,IDC_CHAR,(LPWSTR)&word,8);
							unsigned len = lstrlenW((LPWSTR)&word[0]);
							if(len != 0)
							{
								unsigned short buff[64];
								wsprintf((LPWSTR)&buff[0],L"%d\n",word[len-1]);
								
								OutputDebugString((LPWSTR)&buff[0]);
								if(len > 1)
								{
									SetDlgItemText(hwndDlg,IDC_CHAR,(LPWSTR)&word[len-1]);
									SendMessage(GetDlgItem(hwndDlg,IDC_CHAR),EM_SETSEL,1,1);
								}
								SetDlgItemText(hwndDlg,IDC_UNICODE,(LPWSTR)&buff[0]);
							}
						}
						break;
					}

					break;
				}
			case IDC_UNICODE_IN:
				{
					switch(HIWORD(wParam))
					{
					case EN_UPDATE:
						{
							unsigned short word[36] = {0};
							GetDlgItemText(hwndDlg,IDC_UNICODE_IN,(LPWSTR)&word,72);
							string str = UnicodeToANSI(wstring((wchar_t*)&word[0]));
							int value = atoi(str.c_str());
							wchar_t errmsg[] = L"Invalid Value...";
							if(value > 1<<17-1 || value<0)
							{
								memcpy(&word[0],&errmsg[0],sizeof(errmsg));
							}
							else
							{
								word[0] = value;
								word[1] = 0;
							}
							SetDlgItemText(hwndDlg,IDC_CHAR_OUT,(LPWSTR)&word[0]);
						}
						break;
					}

					break;
				}
			}
		}
		return TRUE;
    }
    return FALSE;
}


HINSTANCE hInst;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)DlgMain);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
