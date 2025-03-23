cmake_minimum_required (VERSION 3.23)

add_library(WindowResizerUtils
	processdata.cpp
	processutils.cpp
	windowresizer.cpp
)

if(WIN32)
	target_sources(WindowResizerUtils
		PRIVATE
			windowsinterface.cpp
	)
endif()

target_sources(WindowResizerUtils
	PUBLIC
	FILE_SET HEADERS
	BASE_DIRS
		include
		"${CMAKE_CURRENT_BINARY_DIR}/include"
)
add_subdirectory(include/windowresizerutils)

target_compile_features(WindowResizerUtils PRIVATE cxx_std_17)
target_compile_definitions(WindowResizerUtils PRIVATE UNICODE _UNICODE)

install(TARGETS WindowResizerUtils
	LIBRARY
		COMPONENT Runtime
	FILE_SET HEADERS
		COMPONENT Development
	#INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
