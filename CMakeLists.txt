cmake_minimum_required(VERSION 3.15)
project(DownloaderLib VERSION "0.9" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../LogLib/resources)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/DownloadLib/resources)

find_package(CURL REQUIRED)

include_directories(include/DownloaderLib)

add_library(DownloaderLib STATIC src/Downloader.cpp include/DownloaderLib/Downloader.h src/DownloaderRAM.cpp include/DownloaderLib/DownloaderRAM.h)
target_link_libraries(DownloaderLib stdc++fs)
target_link_libraries(DownloaderLib ${CURL_LIBRARIES})

add_library(DownloaderLibShared SHARED src/Downloader.cpp include/DownloaderLib/Downloader.h src/DownloaderRAM.cpp include/DownloaderLib/DownloaderRAM.h)
target_link_libraries(DownloaderLibShared stdc++fs)
target_link_libraries(DownloaderLibShared ${CURL_LIBRARIES})

set_target_properties(DownloaderLibShared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/DownloadLib/resources)
set_target_properties(DownloaderLibShared PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/DownloadLib/resources)

