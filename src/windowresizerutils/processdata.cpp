#include "windowresizerutils/processdata.h"

using namespace winresizer;

ProcessData::ProcessData(std::string processName, uint32_t processId) :
	processName(processName), processId(processId)
{
}

std::string ProcessData::getProcessName() const
{
	return this->processName;
}

uint32_t ProcessData::getProcessId() const
{
	return this->processId;
}
