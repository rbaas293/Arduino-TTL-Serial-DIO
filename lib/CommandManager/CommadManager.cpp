#include "CommadManager.h"
#include "RunMode.h"


TPGCommandManager::TPGCommandManager(unsigned long iBaud)
{
	mSerCmd.addCommand("MOD?", CmdMgrQueryMode);
	mSerCmd.addCommand("XXX", On_Command1);
	mSerCmd.setDefaultHandler(CmdMgrUnknownCommand);
	mBaudRate = iBaud;
}


TPGCommandManager::~TPGCommandManager(void)
{
	
}


void TPGCommandManager::Setup()
{
	Serial.begin(mBaudRate);
}

void TPGCommandManager::Refresh(void)
{
	mSerCmd.readSerial();
}

void TPGCommandManager::On_Command1()
{
	Serial.println("xxx");
}

void CmdMgrQueryMode(void)
{
	Serial.println("zzz");
}

void CmdMgrUnknownCommand(const char *iCommand)
{
	Serial.println("UNK");
}
