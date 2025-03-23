#pragma once

#include "windowresizerutils/windowresizerutils_export.h"

#include <string>
#include <cstdint>

namespace winresizer {
	/**
	 * @class ProcessData
	 * @brief A class to store and retrieve data about an OS process.
	 */
	class ProcessData {
	public:
		/**
		 * @brief Constructs a ProcessData object with the given process name and ID.
		 * @param processName The name of the process.
		 * @param processId The ID of the process.
		 */
		WINDOWRESIZERUTILS_EXPORT ProcessData(std::string processName, uint32_t processId);

		/**
		 * @brief Gets the name of the process.
		 * @return The name of the process.
		 */
		WINDOWRESIZERUTILS_EXPORT auto getProcessName() const -> std::string;

		/**
		 * @brief Gets the PID of the process.
		 * @return The PID of the process.
		 */
		WINDOWRESIZERUTILS_EXPORT auto getProcessId() const -> uint32_t;
	private:
		const std::string processName; ///< The name of the process.
		const uint32_t processId{}; ///< The ID of the process.
	};
}
