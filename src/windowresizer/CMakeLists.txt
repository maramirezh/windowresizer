cmake_minimum_required (VERSION 3.23)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# need to explicitly include header files with Q_OBJECT macro in them
# so MOC can do its job
add_executable(WindowResizer 
	main.cpp
	mainwindow.cpp
	processlistmodel.cpp
	
	include/windowresizer/mainwindow.h
	include/windowresizer/processlistmodel.h

	mainwindow.ui
	"${CMAKE_SOURCE_DIR}/data/resources.qrc"
)

target_sources(WindowResizer
	PUBLIC
	FILE_SET HEADERS
	BASE_DIRS
		include
		"${CMAKE_CURRENT_BINARY_DIR}/include"
)

if(WIN32)
	target_sources(WindowResizer
		PRIVATE "${CMAKE_SOURCE_DIR}/data/windowresizericon.rc"
	)
	set_target_properties(WindowResizer
		PROPERTIES
			WIN32_EXECUTABLE TRUE
	)
endif()

target_compile_features(WindowResizer PRIVATE cxx_std_17)
target_compile_definitions(WindowResizer PRIVATE UNICODE _UNICODE)

# TODO: Swap Qt5 with Qt6. Define 'qtbase' in vcpkg.json file
target_link_libraries(WindowResizer
	PRIVATE
		WindowResizerUtils
		Qt5::Core
		Qt5::Gui
		Qt5::Widgets
		spdlog::spdlog
		fmt::fmt
)

install(TARGETS WindowResizer DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_SOURCE_DIR}/src/script/resize_window.ps1 DESTINATION ${CMAKE_INSTALL_BINDIR})

# package Qt DLLs with windows application
if(WIN32)
	install(FILES
	$<TARGET_FILE:Qt5::Widgets>
	$<TARGET_FILE:Qt5::Core>
	$<TARGET_FILE:Qt5::Gui>
	$<TARGET_FILE:spdlog::spdlog>
	$<TARGET_FILE:fmt::fmt>
	DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
	install(FILES
		$<TARGET_FILE:Qt5::QWindowsIntegrationPlugin>
		DESTINATION ${CMAKE_INSTALL_BINDIR}/platforms
	)
endif()
