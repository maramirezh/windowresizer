cmake_minimum_required (VERSION 3.23)

generate_export_header(WindowResizerUtils)

target_sources(WindowResizerUtils
	PUBLIC
		FILE_SET HEADERS
		FILES
			constants.h
			processdata.h
			processutils.h
			windowresizer.h
			${CMAKE_CURRENT_BINARY_DIR}/windowresizerutils_export.h
)
