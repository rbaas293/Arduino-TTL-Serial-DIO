#ifndef TPGCommandManager_h
#define TPGCommandManager_h
#include "SerialCommand.h"

class TPGCommandManager
{
public:
	TPGCommandManager(unsigned long iBaud);
	~TPGCommandManager(void);

	void Setup(void);
	void Refresh(void);
	static void On_Command1(void);

protected:
	unsigned long mBaudRate;
	SerialCommand mSerCmd;


};

void CmdMgrQueryMode(void);
void CmdMgrUnknownCommand(const char *iCommand);


#endif // TPGCommandManager_h
