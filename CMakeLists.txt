cmake_minimum_required(VERSION 3.4)

project(image_interpolation)

## Compile as C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Compile with the highest warning level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

## System dependencies
find_package(OpenCV REQUIRED)

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(load_display_image src/load_display_image.cpp)
add_executable(play_videos src/play_videos.cpp)
add_executable(save_image src/save_image.cpp)
add_executable(save_videos src/save_videos.cpp)
add_executable(change_brightness_image src/change_brightness_image.cpp)
add_executable(change_contrast_image src/change_contrast_image.cpp)
add_executable(change_contrast_video src/change_contrast_video.cpp)
add_executable(the_oxford src/the_oxford.cpp)
add_executable(histogram_equalization_image src/histogram_equalization_image.cpp)
add_executable(histogram_calculation src/histogram_calculation.cpp)
add_executable(draw_line src/draw_line.cpp)
add_executable(draw_line_bresenham src/draw_line_bresenham.cpp)



## Specify libraries to link a library or executable target against
target_link_libraries(load_display_image ${OpenCV_LIBS})
target_link_libraries(play_videos ${OpenCV_LIBS})
target_link_libraries(save_image ${OpenCV_LIBS})
target_link_libraries(save_videos ${OpenCV_LIBS})
target_link_libraries(change_brightness_image ${OpenCV_LIBS})
target_link_libraries(change_contrast_image ${OpenCV_LIBS})
target_link_libraries(change_contrast_video ${OpenCV_LIBS})
target_link_libraries(histogram_equalization_image ${OpenCV_LIBS})
target_link_libraries(histogram_calculation ${OpenCV_LIBS})
target_link_libraries(draw_line ${OpenCV_LIBS})
target_link_libraries(draw_line_bresenham ${OpenCV_LIBS})
