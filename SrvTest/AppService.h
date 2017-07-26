#pragma once


#include "abstractserviceapp.h"

class AppService : public AbstractServiceApp
{
public:
	AppService();

	virtual bool cml(int argc, const char* const* argv)override;
private:
	virtual int execute() override;
	virtual bool beforeRunning() override;
};
