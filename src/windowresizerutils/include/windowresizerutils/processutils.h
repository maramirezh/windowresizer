#pragma once

#include "windowresizerutils/windowresizerutils_export.h"
#include "windowresizerutils/processdata.h"
#include "windowresizerutils/osinterface.h"

#include <cstdint>
#include <vector>
#include <memory>

namespace winresizer {

	/**
	 * @class ProcessUtils
	 * @brief Utility class for handling OS process-related operations.
	 */
	class ProcessUtils
	{
	public:
		ProcessUtils(std::unique_ptr<OSInterface> osInterface);
		/**
		 * @brief Gets the handle of a process by its PID.
		 * @param processId The PID of the process.
		 * @return The handle of the process.
		 */
		WINDOWRESIZERUTILS_EXPORT auto getProcessHandle(uint32_t processId) -> void*;

		/**
		 * @brief Gets the handle of the top window of a process by its PID.
		 * @param processId The PID of the process.
		 * @return The handle of the top window.
		 */
		WINDOWRESIZERUTILS_EXPORT auto getTopWindowHandle(uint32_t processId) -> void*;

		/**
		 * @brief Gets the name of a process by its PID.
		 * @param processId The PID of the process.
		 * @return The name of the process.
		 */
		WINDOWRESIZERUTILS_EXPORT auto getProcessNameFromId(uint32_t processId) -> std::string;

		/**
		 * @brief Gets a list of process IDs.
		 * @return A list of process IDs.
		 */
		WINDOWRESIZERUTILS_EXPORT auto getProcessIdList() noexcept -> std::vector<uint32_t>;

		/**
		 * @brief Gets a list of process IDs with top windows. Dialog windows are ignored.
		 * @return A list of process IDs with top windows.
		 */
		WINDOWRESIZERUTILS_EXPORT auto getProcessIdListWithTopWindows() noexcept
			-> std::vector<uint32_t>;

		/**
		 * @brief Gets a list of process data objects.
		 * @return A list of ProcessData objects.
		 * @see ProcessData
		 */
		WINDOWRESIZERUTILS_EXPORT auto getProcessDataList() noexcept -> std::vector<ProcessData>;

		/**
		 * @brief Gets the last error thrown by an OS call as a string.
		 * @return The last error as a string. Empty if last call was successful
		 */
		WINDOWRESIZERUTILS_EXPORT auto getLastErrorAsString() noexcept -> std::string;

	private:
		std::unique_ptr<OSInterface> osInterface; ///< The OS interface object.
	};
}
