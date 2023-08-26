cmake_minimum_required(VERSION 3.2)

project(test)

set(CMAKE_CXX_STANDARD 11)  # Set your desired C++ standard here

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/test/bin)

add_executable(test_main
	test_main.cpp
)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)
target_include_directories(test_main PRIVATE
    ${Boost_INCLUDE_DIRS}  # Include directories for Boost
    ${OPENSSL_INCLUDE_DIR}  # Include directories for OpenSSL
    ${SPDLOG_INCLUDE_DIRS}  # Include directories for spdlog
)

target_link_libraries(test_main PRIVATE
	Boost::system 
	OpenSSL::SSL 
	OpenSSL::Crypto
	ws2_32
	${GTEST_BOTH_LIBRARIES}
	spdlog::spdlog
)

add_custom_target(clean_test
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/test/bin
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/test/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/test/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/test/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/test/cmake_install.cmake
	COMMENT "Cleaning build files"
)
