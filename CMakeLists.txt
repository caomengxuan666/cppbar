cmake_minimum_required(VERSION 3.19)
project(cppbar VERSION 1.0.0 LANGUAGES CXX)

# ===== CMakeHub Integration =====
# Include CMakeHub loader
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/hub/loader.cmake)

# Set cache directory (optional) - use forward slashes for cross-platform compatibility
if(WIN32)
    set(CMH_CACHE_DIR "$ENV{USERPROFILE}/.cmakehub/cache")
else()
    set(CMH_CACHE_DIR "$ENV{HOME}/.cmakehub/cache")
endif()

# Enable verbose output (optional)
set(CMAKEHUB_VERBOSE ON)

# Set version check mode (STRICT, WARNING, SILENT)
set(CMAKEHUB_VERSION_CHECK_MODE "SILENT")

# Use CMakeHub modules for code quality
cmakehub_use(cpp_standards)
cmakehub_use(compiler_warnings)
cmakehub_use(code_formatter)
cmakehub_use(clang_tidy_tools)

# Build optimization modules
cmakehub_use(lto_optimization)
cmakehub_use(compile_options)


# Sanitizers for Debug builds only
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    cmakehub_use(sanitizers     
        ADDRESS_SANITIZER ON
        UNDEFINED_SANITIZER ON
    )
    # cmakehub_use(coverage_cg)  # Temporarily disabled due to compatibility issues
endif()

# ===== Header-only library setup =====
# CppBar is now a true header-only library
add_library(${PROJECT_NAME} INTERFACE)

# Set include directory
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Apply compiler warnings to targets that use this library
target_compile_options(${PROJECT_NAME} INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# Platform-specific optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:/O2>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O3>
    )
endif()

# Platform-specific definitions
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} INTERFACE
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
    )
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} INTERFACE
        _DARWIN_C_SOURCE
    )
endif()

# Enable testing
enable_testing()

# Add examples directory
add_subdirectory(examples)

# Add tests directory
add_subdirectory(tests)

# Installation rules for vcpkg
include(GNUInstallDirs)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
)

install(EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# ===== Single Header Generation =====
# Use amalgamate.py to generate single header file with LICENSE prologue
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/cppbar.hpp
    COMMAND ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/amalgamate.py
        -c ${CMAKE_CURRENT_SOURCE_DIR}/cppbar.json
        -s ${CMAKE_CURRENT_SOURCE_DIR}
        -p ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/cppbar/progress_bar.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/amalgamate.py
            ${CMAKE_CURRENT_SOURCE_DIR}/cppbar.json
    COMMENT "Generating single header file cppbar.hpp using amalgamate.py"
    VERBATIM
)

add_custom_target(single_header
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cppbar.hpp
    COMMENT "Single header file generation complete"
)
