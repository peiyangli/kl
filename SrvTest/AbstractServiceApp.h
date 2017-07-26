#pragma once


class AbstractServiceApp
{
public:
	AbstractServiceApp();

	enum RunType{
		RT_SERVICE,
		RT_CONSOLE
	};
	bool run(RunType t = RT_SERVICE);
	inline RunType runType()const{
		return _runType;
	}

private:
	virtual bool beforeRunning();
	virtual bool cml(int argc, const char* const* argv) = 0;
	virtual int execute() = 0;
private:
	RunType _runType;
	friend class ServiceHandler;
};
