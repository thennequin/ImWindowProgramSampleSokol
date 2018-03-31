
#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "ImwWindowManagerSokol.h"

class ShortKeyManager;

class Program
{
	Program();
	~Program();
public:

	static Program*							CreateInstance();
	static void								DestroyInstance();
	static Program*							GetInstance() { return s_pInstance; }

	bool									Run();
	void									AskExit();

	ImWindow::ImwWindowManager*				GetWindowManager() { return &m_oMgr; }

protected:
	static Program*							s_pInstance;

	bool									m_bRun;

	ImWindow::ImwWindowManagerSokol			m_oMgr;

	ShortKeyManager*						m_pShortKeyManager;
};

#endif // __PROGRAM_H__