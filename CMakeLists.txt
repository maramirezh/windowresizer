cmake_minimum_required (VERSION 3.23)

include(cmake/vcpkg_setup.cmake)

project("WindowResizer" VERSION 1.0.0)

option(WindowResizer_Generate_Docs "Generate docs for the WindowResizer project" ON)
option(WindowResizer_Generate_Graph "Generate dependency graph for the WindowResizer project" ON)
option(WindowResizer_Build_Unit_Tests "Build unit tests for the WindowResizer project" ON)

# Global CMake settings
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CTest)

# Include sub-projects.
add_subdirectory(src)

if(WindowResizer_Generate_Docs)
	include(cmake/doxygen_docs.cmake)
endif()

if(WindowResizer_Generate_Graph)
	include(cmake/graphviz_graph.cmake)
endif()

if(WindowResizer_Build_Unit_Tests)
	add_subdirectory(tests/windowresizer)
endif()
