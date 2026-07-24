cmake_minimum_required(VERSION 3.28.3)
project(aby-rhi LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)
set(ABY_RHI_INCLUDE_DIR "include/aby-rhi")
set(ABY_RHI_SRC_DIR "src")

option(ABY_RHI_BUILD_TESTS "build aby-rhi tests for the specific platform" ON)

add_subdirectory(${VENDOR_DIR}/Vulkan-Headers)
set(VK_BOOTSTRAP_VULKAN_HEADER_DIR ${VENDOR_DIR}/Vulkan-Headers/include CACHE PATH "")
add_subdirectory(${VENDOR_DIR}/vk-bootstrap)
add_subdirectory(${VENDOR_DIR}/vma)
find_package(Vulkan REQUIRED)

add_library(${PROJECT_NAME} 
    # User API 
    ${ABY_RHI_INCLUDE_DIR}/aby-rhi.hpp
    ${ABY_RHI_INCLUDE_DIR}/common.hpp
    ${ABY_RHI_INCLUDE_DIR}/context.hpp
    ${ABY_RHI_INCLUDE_DIR}/renderer.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/interfaces.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/default_interfaces.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/default_allocator.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/default_fileio.hpp
    ${ABY_RHI_INCLUDE_DIR}/interfaces/default_logger.hpp


    ${ABY_RHI_SRC_DIR}/context.cpp
    ${ABY_RHI_SRC_DIR}/renderer.cpp
    ${ABY_RHI_SRC_DIR}/interfaces/interfaces.cpp
    ${ABY_RHI_SRC_DIR}/interfaces/default_allocator.cpp
    ${ABY_RHI_SRC_DIR}/interfaces/default_fileio.cpp
    ${ABY_RHI_SRC_DIR}/interfaces/default_logger.cpp


    # Vulkan
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-dispatch.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-renderer.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-callbacks.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-platform.hpp
    ${ABY_RHI_INCLUDE_DIR}/backends/vulkan/vulkan-helpers.hpp
    
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-dispatch.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-renderer.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-callbacks.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-platform.cpp
    ${ABY_RHI_SRC_DIR}/backends/vulkan/vulkan-helpers.cpp

)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Vulkan::Vulkan
    vk-bootstrap::vk-bootstrap
    VulkanMemoryAllocator::Headers
)

target_include_directories(${PROJECT_NAME} PRIVATE
     ${ABY_RHI_INCLUDE_DIR}
     ${Vulkan_INCLUDE_DIR}
     ${VENDOR_DIR}/vma/include
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

if (ABY_RHI_BUILD_TESTS)
    add_subdirectory(${CMAKE_SOURCE_DIR}/test/win32)
endif()

if(MSVC)
    set_target_properties(vk-bootstrap PROPERTIES FOLDER "dependencies/vk")
    set_target_properties(VulkanMemoryAllocator PROPERTIES FOLDER "dependencies/vk")
endif()