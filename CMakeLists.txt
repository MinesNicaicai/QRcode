cmake_minimum_required(VERSION 2.8)
project( QRcode )
find_package( OpenCV REQUIRED )
enable_testing()
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( QRparser main.cpp )
target_link_libraries( QRparser ${OpenCV_LIBS} )

add_test(NAME test COMMAND QRparser ../6_LI_Yanhao.bmp output.txt)