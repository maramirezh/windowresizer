#include "windowresizerutils/processutils.h"
#include <vector>
#include <set>
#include <stdexcept>
#include <iostream>
#ifdef WIN32
#include "windowresizerutils/windowsinterface.h"
#include <Windows.h>
#include <psapi.h>
#endif

using namespace winresizer;

struct ParameterData
{
	uint32_t processId{};
	void* windowHandle{};
	OSInterface* osInterface{};
};

ProcessUtils::ProcessUtils(std::unique_ptr<OSInterface> osInterface) :
	osInterface(std::move(osInterface))
{
}

auto ProcessUtils::getProcessHandle(uint32_t processId) -> void*
{
	void* processIdHandle{};
#ifdef WIN32
	processIdHandle = this->osInterface->OpenProcessWrapper(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		false,
		static_cast<DWORD>(processId)
	);
#endif
	if (!processIdHandle) {
		throw std::runtime_error("Failed to get process handle");
	}
	return processIdHandle;
}

auto ProcessUtils::getTopWindowHandle(uint32_t processId) -> void*
{
	void* windowHandle{};
	ParameterData data = { processId, nullptr, osInterface.get() };
#ifdef WIN32
	auto enumWindowsCallback = [](void* hwnd, void* lParam) -> bool {
		ParameterData* parameterData = reinterpret_cast<ParameterData*>(lParam);
		uint32_t processId{};
		if (
			parameterData->osInterface->GetWindowThreadProcessIdWrapper(
				hwnd,
				&processId) != 0
			&& processId == parameterData->processId
			&& !parameterData->osInterface->GetWindowWrapper(hwnd, GW_OWNER)
			&& parameterData->osInterface->IsWindowVisibleWrapper(hwnd)
			)
		{
			// Stop enumerating
			parameterData->windowHandle = hwnd;
			return false;
		}
		// Continue enumerating
		return true;
	};
	std::ignore = this->osInterface->EnumWindowsWrapper(enumWindowsCallback, &data);
	windowHandle = data.windowHandle;
#endif
	if (!windowHandle) {
		throw std::runtime_error("Could not find Top Window for given process");
	}
	return windowHandle;
}

auto winresizer::ProcessUtils::getProcessNameFromId(uint32_t processId) -> std::string
{
	std::string processName;
#ifdef WIN32
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	//see: https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes
	HANDLE hProcess = this->getProcessHandle(processId);

	if (!hProcess) {
		throw std::runtime_error("Failed to open process from PID");
	}

	HMODULE hMod{};
	DWORD cbNeeded{};
	if (!EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
		CloseHandle(hProcess);
		throw std::runtime_error("Failed to get process name");
	}
	GetModuleBaseName(hProcess, hMod, szProcessName, std::size(szProcessName));
	CloseHandle(hProcess);
#ifdef UNICODE
	// TCHAR is wchar_t
	const int size = WideCharToMultiByte(CP_UTF8, 0, szProcessName, -1, nullptr, 0, nullptr, nullptr);
	std::string result(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, szProcessName, -1, &result[0], size, nullptr, nullptr);
	processName = result;
#else
	// TCHAR is char
	processName = szProcessName;
#endif
#endif
	return processName;
}

auto winresizer::ProcessUtils::getProcessIdList() noexcept -> std::vector<uint32_t>
{
	std::vector<uint32_t> processIdList;
	uint32_t aProcesses[1024];
	uint32_t cbNeeded{};
	if (this->osInterface->EnumProcessesWrapper(
		aProcesses, sizeof(aProcesses), &cbNeeded)) {
		const auto numProcesses = cbNeeded / sizeof(uint32_t);
		processIdList.reserve(numProcesses);
		for (uint32_t i = 0; i < numProcesses; ++i) {
			processIdList.push_back(static_cast<uint32_t>(aProcesses[i]));
		}
	}
	return processIdList;
}

auto ProcessUtils::getProcessIdListWithTopWindows() noexcept -> std::vector<uint32_t>
{
	const auto processIdList = this->getProcessIdList();
	std::set<uint32_t> processList;
	for (auto&& pid : processIdList) {
		try {
			std::ignore = this->getTopWindowHandle(pid);
			processList.insert(pid);
		}
		catch (const std::exception& e) {
			//has no top window
			continue;
		}
	}
	return std::vector(processList.begin(), processList.end());
}

auto ProcessUtils::getProcessDataList() noexcept -> std::vector<ProcessData>
{
	const auto pidList = getProcessIdListWithTopWindows();
	std::vector<ProcessData> processList;
	processList.reserve(pidList.size());
	for (auto&& pid : pidList) {
		try {
			ProcessData processData(getProcessNameFromId(pid), pid);
			processList.push_back(processData);
		}
		catch (const std::exception& e) {
			//may happen if user lacks privileges to open a process
			continue;
		}		
	}
    return processList;
}

auto ProcessUtils::getLastErrorAsString() noexcept -> std::string
{
	std::string message;
#ifdef WIN32
	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0) {
		return std::string(); // No error, return empty string
	}

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&messageBuffer,
		0,
		nullptr);

	message = std::string(messageBuffer, size);

	LocalFree(messageBuffer); // Free the buffer allocated by FormatMessage
#endif
	return message;
}
