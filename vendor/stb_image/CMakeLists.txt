project(stb_image)
cmake_minimum_required(VERSION 3.28.3)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)

add_library(${PROJECT_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image/stb_image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image/stb_image_resize2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image/stb_image_write.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image.cpp
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)