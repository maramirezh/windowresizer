#pragma once

#include "windowresizerutils/windowresizerutils_export.h"
#include "windowresizerutils/osinterface.h"

#include <cstdint>

namespace winresizer {

	/**
	 * @class WindowsInterface
	 * @brief Thin wrapper around Win32 APIs.
	 * The class methods intentionally map 1:1 to the Win32 APIs inputs and outputs
	 * to facilitate mocking for testing purposes. These methods should not be used
	 * directly, but instead be wrapped around a nicer interface via other classes,
	 * such as ProcessUtils
	 */
	class WindowsInterface : public OSInterface
	{
	public:

		WINDOWRESIZERUTILS_EXPORT void* OpenProcessWrapper(
			uint32_t desiredAccess,
			bool inheritHandle,
			uint32_t processId) const override;

		/**
		 * @brief Gets the window thread process ID.
		 * @param hwnd Window handle
		 * @param processId Process Id
		 * @return Non-zero if successful, zero otherwse.
		 */
		WINDOWRESIZERUTILS_EXPORT uint32_t GetWindowThreadProcessIdWrapper(
			void* hwnd,
			uint32_t* processId) const override;

		/**
		 * @brief Gets the window wrapper.
		 * @param hwnd Window handle
		 * @param cmd Command
		 * @return The window wrapper.
		 */
		WINDOWRESIZERUTILS_EXPORT void* GetWindowWrapper(
			void* hwnd,
			uint32_t cmd) const override;

		/**
		 * @brief Checks if the window is visible.
		 * @param hwnd Window handle
		 * @return True if the window is visible, false otherwise.
		 */
		WINDOWRESIZERUTILS_EXPORT bool IsWindowVisibleWrapper(void* hwnd) const override;

		WINDOWRESIZERUTILS_EXPORT bool EnumWindowsWrapper(
			std::function<bool(void*, void*)> callback,
			void* customParameter
		) const override;

		/**
		 * @brief Sets the position and size of the specified window.
		 *
		 * @param hwnd Handle to the window.
		 * @param hwndInsertAfter Handle to the window to precede the positioned window in the Z order.
		 * @param xPos New position of the left side of the window.
		 * @param yPos New position of the top of the window.
		 * @param width New width of the window.
		 * @param height New height of the window.
		 * @param flags Window sizing and positioning flags.
		 * @return True if the function succeeds, false otherwise.
		 */
		WINDOWRESIZERUTILS_EXPORT bool SetWindowPosWrapper(
			void* hwnd,
			void* hwndInsertAfter,
			int32_t xPos,
			int32_t yPos,
			int32_t width,
			int32_t height,
			uint32_t flags
		) override;

		/**
		 * @brief Enumerates the processes in the OS
		 * @param[in/out] processesList A pointer to an array of PIDs.
		 * The array will be filled on return
		 * @param[in] processesListInSize The size of processesList in memory
		 * @param[out] processesListOutSize Pointer to the size of processesList filled back, in memory
		 * @return true if process enumeration was successful, false otherwise
		 */
		WINDOWRESIZERUTILS_EXPORT virtual bool EnumProcessesWrapper(
			uint32_t* processesList,
			uint32_t processesListInSize,
			uint32_t* processesListOutSize
		) const override;

		/**
		 * @brief Destroys the WindowsInterface object.
		 */
		WINDOWRESIZERUTILS_EXPORT ~WindowsInterface() = default;
	};
}
