cmake_minimum_required(VERSION 3.28.3)
cmake_policy(SET CMP0091 NEW) # Set the MSVC runtime library policy to NEW (required for CMake 3.15+)
project(aby-rhi LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)

set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(ABY_RHI_INCLUDE_DIR "include/aby-rhi")
set(ABY_RHI_SRC_DIR "src")

option(ABY_RHI_BUILD_TESTS "build aby-rhi tests for the specific platform" ON)
option(ABY_RHI_ENABLE_LOG_INFO "enable info logging" ON)
option(ABY_RHI_ENABLE_LOG_TRACE "enable trace logging" ON)
option(ABY_RHI_ENABLE_LOG_WARN "enable warning logging" ON)
option(ABY_RHI_ENABLE_ASSERT "enable assertions. if turned off then only errors will be logged." ON)
option(ABY_RHI_ENABLE_PROFILING "enable profiling of initialization scopes" OFF)

set(VK_BOOTSTRAP_VULKAN_HEADER_DIR ${VENDOR_DIR}/Vulkan-Headers/include CACHE FILEPATH "")
add_subdirectory(${VENDOR_DIR}/vk-bootstrap)
set_target_properties(vk-bootstrap PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
add_subdirectory(${VENDOR_DIR}/vma)
set(SHADERC_SKIP_EXAMPLES        ON CACHE BOOL "")
set(SHADERC_ENABLE_TESTS         OFF CACHE BOOL "")
set(SHADERC_SKIP_TESTS           ON CACHE BOOL "")
set(SHADERC_SKIP_EXECUTABLES     ON CACHE BOOL "")
set(SHADERC_SKIP_COPYRIGHT_CHECK ON CACHE BOOL "")
add_subdirectory(${VENDOR_DIR}/shaderc)

add_subdirectory(${VENDOR_DIR}/stb_image)
set_target_properties(stb_image PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

find_package(Vulkan REQUIRED)

add_library(${PROJECT_NAME} 
    # User API 
    ${ABY_RHI_INCLUDE_DIR}/aby-rhi.hpp
    ${ABY_RHI_INCLUDE_DIR}/buffer.hpp
    ${ABY_RHI_INCLUDE_DIR}/common.hpp
    ${ABY_RHI_INCLUDE_DIR}/common-enums.hpp
    ${ABY_RHI_INCLUDE_DIR}/common-types.hpp
    ${ABY_RHI_INCLUDE_DIR}/context.hpp
    ${ABY_RHI_INCLUDE_DIR}/draw-cmd.hpp
    ${ABY_RHI_INCLUDE_DIR}/renderer.hpp
    ${ABY_RHI_INCLUDE_DIR}/render-pass.hpp
    ${ABY_RHI_INCLUDE_DIR}/resource.hpp
    ${ABY_RHI_INCLUDE_DIR}/shader.hpp
    ${ABY_RHI_INCLUDE_DIR}/texture.hpp
    ${ABY_RHI_INCLUDE_DIR}/plugins/plugin.hpp
    ${ABY_RHI_INCLUDE_DIR}/utility/meta.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/interfaces.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/default_interfaces.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/default_allocator.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/default_fileio.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/default_job_system.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/default_logger.hpp

    ${ABY_RHI_SRC_DIR}/buffer.cpp
    ${ABY_RHI_SRC_DIR}/common.cpp
    ${ABY_RHI_SRC_DIR}/common-types.cpp
    ${ABY_RHI_SRC_DIR}/context.cpp
    ${ABY_RHI_SRC_DIR}/draw-cmd.cpp
    ${ABY_RHI_SRC_DIR}/renderer.cpp
    ${ABY_RHI_SRC_DIR}/render-pass.cpp
    ${ABY_RHI_SRC_DIR}/resource.cpp
    ${ABY_RHI_SRC_DIR}/shader.cpp
    ${ABY_RHI_SRC_DIR}/texture.cpp
    ${ABY_RHI_SRC_DIR}/plugins/plugin.cpp
    ${ABY_RHI_SRC_DIR}/interfaces/interfaces.cpp
    ${ABY_RHI_SRC_DIR}/interfaces/default_allocator.cpp
    ${ABY_RHI_SRC_DIR}/interfaces/default_fileio.cpp
    ${ABY_RHI_SRC_DIR}/interfaces/default_job_system.cpp
    ${ABY_RHI_SRC_DIR}/interfaces/default_logger.cpp


    # Vulkan
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-buffer.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-callbacks.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-common.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-descriptors.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-dispatch.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-frame.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-gc.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-helpers.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-image.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-immediate-cmd.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-pipeline.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-platform.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-renderer.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-render-pass.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-shader.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-texture.hpp

    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-buffer.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-callbacks.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-descriptors.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-dispatch.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-frame.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-gc.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-helpers.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-image.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-immediate-cmd.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-pipeline.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-platform.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-renderer.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-render-pass.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-shader.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-texture.cpp


)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

list(APPEND CMAKE_PREFIX_PATH "$ENV{VULKAN_SDK}/Lib/cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Vulkan::Vulkan
    vk-bootstrap::vk-bootstrap
    VulkanMemoryAllocator::Headers
    shaderc
    stb_image::stb_image
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ABY_RHI_INCLUDE_DIR}
    ${Vulkan_INCLUDE_DIR}
    ${VENDOR_DIR}/vma/include
    ${VENDOR_DIR}/shaderc/libshaderc/include
    ${VENDOR_DIR}/stb_image/include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /WX
        /Zc:preprocessor
    >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    ABY_RHI_ENABLE_LOG_INFO=$<BOOL:${ABY_RHI_ENABLE_LOG_INFO}>
    ABY_RHI_ENABLE_LOG_TRACE=$<BOOL:${ABY_RHI_ENABLE_LOG_TRACE}>
    ABY_RHI_ENABLE_LOG_WARN=$<BOOL:${ABY_RHI_ENABLE_LOG_WARN}> 
    ABY_RHI_ENABLE_ASSERT=$<BOOL:${ABY_RHI_ENABLE_ASSERT}>
    ABY_RHI_ENABLE_PROFILING=$<BOOL:${ABY_RHI_ENABLE_PROFILING}>
)

if (ABY_RHI_BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/win32)
endif()

if(MSVC)
    set_target_properties(vk-bootstrap PROPERTIES FOLDER "dependencies/vk")
    set_target_properties(VulkanMemoryAllocator PROPERTIES FOLDER "dependencies/vk")
endif()
