cmake_minimum_required (VERSION 3.18)

find_package(GTest CONFIG REQUIRED)

include(FetchContent)

if(WIN32)
	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_executable(WindowResizerTests
	main.cpp
	processdata_test.cpp
	processutils_test.cpp
	windowresizer_test.cpp
)

target_sources(WindowResizerTests
	PUBLIC
	FILE_SET HEADERS
	BASE_DIRS
		include
		"${CMAKE_CURRENT_BINARY_DIR}/include"
)

target_compile_features(WindowResizerTests PRIVATE cxx_std_17)
target_compile_definitions(WindowResizerTests PRIVATE UNICODE _UNICODE)

target_link_libraries(WindowResizerTests
	PRIVATE
		GTest::gtest
		GTest::gmock
		WindowResizerUtils
)

add_test(WindowResizerUnitTests WindowResizerTests)
