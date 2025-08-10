cmake_minimum_required(VERSION 3.15)
project(CppSpring VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find all header files
file(GLOB_RECURSE CPPSPRING_HEADERS
        "include/*.hpp"
        "include/*.h"
)

# Find all source files
file(GLOB_RECURSE CPPSPRING_SOURCES
        "src/*.cpp"
        "src/*.cc"
)

# Create the library
if(CPPSPRING_SOURCES)
    # If we have source files, create a compiled library
    add_library(cppspring ${CPPSPRING_SOURCES} ${CPPSPRING_HEADERS})
else()
    # If header-only, create an interface library
    add_library(cppspring INTERFACE)
endif()

# Include directories
if(CPPSPRING_SOURCES)
    target_include_directories(cppspring PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
else()
    target_include_directories(cppspring INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
endif()

# Set target properties
set_target_properties(cppspring PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

# Optional: Add debug/release specific settings
if(CPPSPRING_SOURCES)
    target_compile_definitions(cppspring PUBLIC
            $<$<CONFIG:Debug>:CPPSPRING_DEBUG>
            $<$<CONFIG:Release>:CPPSPRING_RELEASE>
    )
else()
    target_compile_definitions(cppspring INTERFACE
            $<$<CONFIG:Debug>:CPPSPRING_DEBUG>
            $<$<CONFIG:Release>:CPPSPRING_RELEASE>
    )
endif()

# IDE support - organize headers in folders
if(CPPSPRING_HEADERS)
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include"
            PREFIX "Header Files"
            FILES ${CPPSPRING_HEADERS})
endif()

if(CPPSPRING_SOURCES)
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
            PREFIX "Source Files"
            FILES ${CPPSPRING_SOURCES})
endif()

# Installation
include(GNUInstallDirs)

install(TARGETS cppspring
        EXPORT CppSpringTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# Export configuration
install(EXPORT CppSpringTargets
        FILE CppSpringTargets.cmake
        NAMESPACE CppSpring::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CppSpring
)

# Create config file (optional - only if template exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CppSpringConfig.cmake.in")
    include(CMakePackageConfigHelpers)
    configure_package_config_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CppSpringConfig.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/CppSpringConfig.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CppSpring
    )

    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/CppSpringConfigVersion.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion
    )

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/CppSpringConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/CppSpringConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CppSpring
    )
endif()

# Tests
option(CPPSPRING_BUILD_TESTS "Build tests" OFF)
if(CPPSPRING_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
option(CPPSPRING_BUILD_EXAMPLES "Build examples" ON)
if(CPPSPRING_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Documentation
option(CPPSPRING_BUILD_DOCS "Build documentation" OFF)
if(CPPSPRING_BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        add_subdirectory(docs)
    else()
        message(WARNING "Doxygen not found, documentation will not be built")
    endif()
endif()

# Print configuration summary
message(STATUS "CppSpring Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build Tests: ${CPPSPRING_BUILD_TESTS}")
message(STATUS "  Build Examples: ${CPPSPRING_BUILD_EXAMPLES}")
message(STATUS "  Build Docs: ${CPPSPRING_BUILD_DOCS}")

list(LENGTH CPPSPRING_HEADERS HEADER_COUNT)
list(LENGTH CPPSPRING_SOURCES SOURCE_COUNT)
message(STATUS "  Headers found: ${HEADER_COUNT}")
message(STATUS "  Sources found: ${SOURCE_COUNT}")

if(SOURCE_COUNT EQUAL 0)
    message(STATUS "  Library type: INTERFACE (header-only)")
else()
    message(STATUS "  Library type: COMPILED")
endif()