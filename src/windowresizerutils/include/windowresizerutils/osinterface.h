#pragma once

#include "windowresizerutils/windowresizerutils_export.h"

#include <cstdint>
#include <functional>

namespace winresizer {

	/**
	 * @class OSInterface
	 * @brief Pure virtual interface for OS APIs
	 */
	class OSInterface
	{
	public:

		/**
		 * @brief Opens a process.
		 * @param desiredAccess Desired access
		 * @param inheritHandle Inherit handle
		 * @param processId Process Id
		 * @return The process handle.
		 */
		WINDOWRESIZERUTILS_EXPORT virtual void* OpenProcessWrapper(
			uint32_t desiredAccess,
			bool inheritHandle,
			uint32_t processId) const = 0;

		/**
		 * @brief Gets the window thread process ID.
		 * @param hwnd Window handle
		 * @param processId Process Id
		 * @return Non-zero if successful, zero otherwse.
		 */
		WINDOWRESIZERUTILS_EXPORT virtual uint32_t GetWindowThreadProcessIdWrapper(
			void* hwnd,
			uint32_t* processId) const = 0;

		/**
		 * @brief Gets the handle to a window based on the relationship/owner 
		 * to the specified window
		 * @param hwnd Window handle
		 * @param cmd Command
		 * @return The window handle.
		 */
		WINDOWRESIZERUTILS_EXPORT virtual void* GetWindowWrapper(void* hwnd, uint32_t cmd) const = 0;

		/**
		 * @brief Checks if the window is visible.
		 * @param hwnd Window handle
		 * @return True if the window is visible, false otherwise.
		 */
		WINDOWRESIZERUTILS_EXPORT virtual bool IsWindowVisibleWrapper(void* hwnd) const = 0;

		WINDOWRESIZERUTILS_EXPORT virtual bool EnumWindowsWrapper(
			std::function<bool(void*, void*)> callback,
			void* customParameter
		) const = 0;

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
		WINDOWRESIZERUTILS_EXPORT virtual bool SetWindowPosWrapper(
			void* hwnd,
			void* hwndInsertAfter,
			int32_t xPos,
			int32_t yPos,
			int32_t width,
			int32_t height,
			uint32_t flags
		) = 0;

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
		) const = 0;

		/**
		 * @brief Destroys the OSInterface object.
		 */
		WINDOWRESIZERUTILS_EXPORT virtual ~OSInterface() = default;
	};
}
