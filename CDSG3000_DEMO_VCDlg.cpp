#include "CDSG3000_DEMO_VCDlg.h"

// Encapsulate the read and write operations of VISA.
// 1) Encapsulate the write operation of VISA for easier operation.

#include <iostream>
#include "CDSG3000_DEMO_VCDlg.h"
#include <WinUser.h>
#include <format>


bool CDSG3000_DEMO_VCDlg::InstrWrite(CString strAddr, CString strContent) //write function
{
	ViSession defaultRM, instr;
	ViStatus status;
	ViUInt32 retCount;
	char* SendBuf = NULL;
	char* SendAddr = NULL;
	bool bWriteOK = false;
	CString str;
	//Change the address's data style from CString to char*
	SendAddr = strAddr.GetBuffer(strAddr.GetLength());
	strcpy(SendAddr, strAddr);
	strAddr.ReleaseBuffer();
	//Change the command's data style from CString to char*
	SendBuf = strContent.GetBuffer(strContent.GetLength());
	strcpy(SendBuf, strContent);
	strContent.ReleaseBuffer();
	//open the VISA instrument
	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		//AtlMessageBox("No VISA instrument was opened !");
		return false;
	}
	status = viOpen(defaultRM, SendAddr, VI_NULL, VI_NULL, &instr);
	//write command to the instrument


	status = viWrite(instr, (unsigned char*)SendBuf, strlen(SendBuf), &retCount);
	//close the instrument
	status = viClose(instr);
	status = viClose(defaultRM);
	return bWriteOK;
}
// 2) Encapsulate the read operation of VISA for easier operation.
bool CDSG3000_DEMO_VCDlg::InstrRead(CString strAddr, CString* pstrResult)
//Read from the instrument
{
	ViSession defaultRM, instr;
	ViStatus status;
	ViUInt32 retCount;
	char* SendAddr = NULL;
	unsigned char RecBuf[MAX_REC_SIZE];
	bool bReadOK = false;
	CString str;
	//Change the address's data style from CString to char*
	SendAddr = strAddr.GetBuffer(strAddr.GetLength());
	strcpy(SendAddr, strAddr);
	strAddr.ReleaseBuffer();
	memset(RecBuf, 0, MAX_REC_SIZE);
	//open the VISA instrument
	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		// Error Initializing VISA...exiting 
		//AfxMessageBox("No VISA instrument was opened !");
		return false;
	}
	//open the instrument
	status = viOpen(defaultRM, SendAddr, VI_NULL, VI_NULL, &instr);
	//read from the instrument
	status = viRead(instr, RecBuf, MAX_REC_SIZE, &retCount);
	//close the instrument
	status = viClose(instr);
	status = viClose(defaultRM);
	(*pstrResult).Format("%s", RecBuf);
	return bReadOK;
}

// Add the control message response codes.
// 1) Connect the instrument
void CDSG3000_DEMO_VCDlg::OnConnect()
{
	// TODO: Add your control notification handler code here
	ViStatus status;
	ViSession defaultRM;
	ViString expr = "?*";
	ViPFindList findList = new unsigned long;
	ViPUInt32 retcnt = new unsigned long;
	ViChar instrDesc[1000];
	CString strSrc = "";
	CString strInstr = "";
	unsigned long i = 0;
	bool bFindDSG = false;
	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		// Error Initializing VISA...exiting 
		//MessageBox("No VISA instrument was opened ! ");
		return;
	}
	memset(instrDesc, 0, 1000);
	// Find resource
	status = viFindRsrc(defaultRM, expr, findList, retcnt, instrDesc);
	for (i = 0; i < (*retcnt); i++)
	{
		// Get instrument name
		strSrc.Format("%s", instrDesc);
		InstrWrite(strSrc, "*IDN?");
		::Sleep(200);
		InstrRead(strSrc, &strInstr);

		// If the instrument(resource) belongs to the DSG series then jump out from the loop
		strInstr.MakeUpper();
		if (strInstr.Find("DSG") >= 0)
		{
			bFindDSG = true;
			m_strInstrAddr = strSrc;
			break;
		}
		//Find next instrument
		status = viFindNext(*findList, instrDesc);
	}
	if (bFindDSG == false)
	{
		MessageBox("Didn't find any DSG!");
	}
	UpdateData(false);
}
// 2) Write Operation
void CDSG3000_DEMO_VCDlg::OnSend()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if (m_strInstrAddr.IsEmpty())
	{
		MessageBox("Please connect to the instrument first!");
	}
	InstrWrite(m_strInstrAddr, m_strCommand);
	m_strResult.Empty();
	UpdateData(false);
}
// 3) Read Operation
void CDSG3000_DEMO_VCDlg::OnRead()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	InstrRead(m_strInstrAddr, &m_strResult);
	UpdateData(false);
}