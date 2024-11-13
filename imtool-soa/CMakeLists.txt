add_executable(imtool-soa imtool-soa.cpp)
target_include_directories(imtool-soa PRIVATE ${CMAKE_SOURCE_DIR}/libraries/common ${CMAKE_SOURCE_DIR}/libraries/imgsoa)
target_link_libraries (imtool-soa PRIVATE imgsoa common stdc++)