#include "windowresizerutils/windowsinterface.h"
#include <Windows.h>
#include <psapi.h>

using namespace winresizer;

void* WindowsInterface::OpenProcessWrapper(uint32_t desiredAccess, bool inheritHandle,
	uint32_t processId) const
{
	return ::OpenProcess(
		static_cast<DWORD>(desiredAccess),
		static_cast<BOOL>(inheritHandle),
		static_cast<DWORD>(processId)
	);
}

uint32_t WindowsInterface::GetWindowThreadProcessIdWrapper(void* hwnd, uint32_t* processId) const
{
	return ::GetWindowThreadProcessId(
		reinterpret_cast<HWND>(hwnd),
		reinterpret_cast<DWORD*>(processId)
	);
}

void* WindowsInterface::GetWindowWrapper(void* hwnd, uint32_t cmd) const
{
	return ::GetWindow(
		reinterpret_cast<HWND>(hwnd),
		static_cast<UINT>(cmd)
	);
}

bool WindowsInterface::IsWindowVisibleWrapper(void* hwnd) const
{
	return ::IsWindowVisible(
		reinterpret_cast<HWND>(hwnd)
	) != 0;
}


struct EnumData {
	std::function<bool(void*, void*)> callback;
	void* lparam{};
};

bool WindowsInterface::EnumWindowsWrapper(
	std::function<bool(void*, void*)> callback, void* customParameter) const
{
	EnumData data = { callback, customParameter };

	auto internalCallback = [](HWND hwnd, LPARAM lParam) -> BOOL {
		EnumData* pData = reinterpret_cast<EnumData*>(lParam);
		return pData->callback(reinterpret_cast<void*>(hwnd), pData->lparam);
		};

	return EnumWindows(internalCallback, reinterpret_cast<LPARAM>(&data));
}

bool WindowsInterface::SetWindowPosWrapper(
	void* hwnd,
	void* hwndInsertAfter,
	int32_t xPos,
	int32_t yPos,
	int32_t width,
	int32_t height,
	uint32_t flags)
{
	return ::SetWindowPos(
		reinterpret_cast<HWND>(hwnd),
		reinterpret_cast<HWND>(hwndInsertAfter),
		xPos,
		yPos,
		width,
		height,
		flags
	) != 0;
}

bool WindowsInterface::EnumProcessesWrapper(uint32_t* processesList, uint32_t processesListInSize,
	uint32_t* processesListOutSize) const
{
	//see: https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes
	return EnumProcesses(
		reinterpret_cast<DWORD*>(processesList),
		processesListInSize,
		reinterpret_cast<DWORD*>(processesListOutSize)
	) != 0;
}
