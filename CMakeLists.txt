cmake_minimum_required(VERSION 3.15)
project(DownloaderLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/DownloadLib/resources)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../LogLib/resources)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

find_package(CURL REQUIRED)

add_library(DownloaderLib STATIC src/Downloader.cpp include/Downloader.h)
target_link_libraries(DownloaderLib stdc++fs)
target_link_libraries(DownloaderLib ${CURL_LIBRARIES})

