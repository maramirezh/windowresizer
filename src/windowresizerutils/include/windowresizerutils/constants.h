#pragma once

#include <cstdint>
#include <string>

namespace winresizer {
	namespace constants {
		namespace ui {
			/**
			 * @brief Minimum width for the target window.
			 */
			constexpr int32_t minWidth = 100; 
			
			/**
			 * @brief Minimum height for the target window.
			 */
			constexpr int32_t minHeight = 100;
			
			/**
			 * @brief Maximum width for the target window.
			 */
			constexpr int32_t maxWidth = 20000;
			
			/**
			 * @brief Maximum height for the target window.
			 */
			constexpr int32_t maxHeight = 20000;
			
			/**
			 * @brief Default width for the target window.
			 */
			constexpr int32_t defaultWidth = 800;
			
			/**
			 * @brief Default height for the target window.
			 */
			constexpr int32_t defaultHeight = 600;
			
			/**
			 * @brief Default X position for the target window.
			 */
			constexpr int32_t defaultPosX = 100;
			
			/**
			 * @brief Default Y position for the target window.
			 */
			constexpr int32_t defaultPosY = 100;

			/**
			 * @brief Default label for when no program is selected.
			 */
			constexpr char noProgramSelected[] = "<none selected>";
		}
		namespace logger {
			/**
			 * @brief Logger file name.
			 */
			constexpr char fileName[] = "window_resizer.log";
			constexpr char loggerName[] = "window_resizer";
			
			/**
			 * @brief Maximum log file size (in bytes).
			 */
			constexpr int fileSize = 1 * 1024 * 1024; // 1MiB
			
			/**
			 * @brief Maximum number of log files.
			 */
			constexpr int maxFiles = 1;
			
			/**
			 * @brief Whether it rotates log file when the program is opened.
			 */
			constexpr bool rotateOnOpen = false;
		}
	}
}
