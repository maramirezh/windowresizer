#pragma once

#include "windowresizerutils/windowresizerutils_export.h"
#include "windowresizerutils/processutils.h"

#include <cstdint>
#include <memory>

namespace winresizer {
	/**
	 * @class WindowResizer
	 * @brief A class to resize windows of a specific process.
	 */
	class WindowResizer
	{
	public:
		/**
		 * @brief Constructs a WindowResizer object for a given process ID.
		 * @param osInterface The OS interface object.
		 * @param processUtils The process utility object.
		 * @param processId The ID of the process whose window will be resized.
		 */
		WINDOWRESIZERUTILS_EXPORT WindowResizer(
			std::unique_ptr<OSInterface> osInterface,
			std::unique_ptr<ProcessUtils> processUtils,
			uint32_t processId);

		/**
		 * @brief Resizes the window to the specified width and height.
		 * The window is by default moved to position 100x100
		 * @param width The new width of the window.
		 * @param height The new height of the window.
		 */
		WINDOWRESIZERUTILS_EXPORT auto resizeWindow(int32_t width, int32_t height) -> void;

	private:
		const uint32_t processId{}; ///< The ID of the process whose window will be resized.
		std::unique_ptr<OSInterface> osInterface; ///< The OS interface object.
		std::unique_ptr<ProcessUtils> processUtils; ///< The process utility object.
		void* windowHandle{}; ///< Handle to the target window.
	};
}
