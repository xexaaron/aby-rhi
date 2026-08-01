cmake_minimum_required(VERSION 3.28.3)
project(aby-rhi-test-win32 LANGUAGES CXX)

set(ABY_RHI_WIN32_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ABY_RHI_WIN32_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${ABY_RHI_WIN32_SRC_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE aby-rhi::aby-rhi)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include ${ABY_RHI_WIN32_INCLUDE_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /WX
        /Zc:preprocessor
    >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/test_frag.frag
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_frag.frag

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/test_vertex.vert
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_vertex.vert

    COMMENT "[test:win32] Copying test shaders"
)