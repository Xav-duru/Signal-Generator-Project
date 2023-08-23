#pragma once
#include <cstring>
#include <atlstr.h>
#include <atlwin.h>
#include <string>
#include <atltypes.h>
#include <iostream>
#include <C:\Program Files\IVI Foundation\VISA\Win64\Include\visatype.h>
#include <C:\Program Files\IVI Foundation\VISA\Win64\Include\vpptype.h>
#include <C:\Program Files\IVI Foundation\VISA\Win64\Include\visa.h>
#define MAX_REC_SIZE 2048

class CDSG3000_DEMO_VCDlg
{
	bool InstrWrite(CString, CString);
	bool InstrRead(CString, CString*);
	void OnConnect();
	void OnSend();
	void OnRead();
};





//write function
	