# Add to this list all files related to imgsoa library
add_library(imgsoa
        imagesoa.cpp
        imagesoa.hpp
)
target_include_directories(imgsoa PRIVATE ${CMAKE_SOURCE_DIR}/libraries/common)
# Use this line only if you have dependencies from this library to GSL
target_link_libraries (imgsoa PRIVATE common Microsoft.GSL::GSL)