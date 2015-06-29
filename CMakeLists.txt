cmake_minimum_required(VERSION 2.6 FATAL_ERROR)


project(depthmapfile)


file(GLOB_RECURSE CURRENT_HEADERS  *.h *.hpp)
file(GLOB CURRENT_SOURCES  *.cpp)
source_group("Include" FILES ${CURRENT_HEADERS}) 
source_group("Source" FILES ${CURRENT_SOURCES}) 



add_executable(depthmapfile ${CURRENT_HEADERS} ${CURRENT_SOURCES} )






