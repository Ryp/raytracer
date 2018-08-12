# Ignore level-4 warning C4201: nonstandard extension used : nameless struct/union
# Ignore level-1 warning C4251: 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
# Ignore level-3 warning C4996: Various members of std::allocator are deprecated in C++17.
set(RAYTRACER_MSVC_DISABLE_WARNINGS "/W0")
set(RAYTRACER_MSVC_DEBUG_FLAGS "/W4" "/wd4201" "/wd4251" "/wd4996")
set(RAYTRACER_MSVC_RELEASE_FLAGS ${RAYTRACER_MSVC_DISABLE_WARNINGS})
set(RAYTRACER_GCC_DEBUG_FLAGS
    "-Wall" "-Wextra" "-Wundef" "-Wshadow" "-funsigned-char"
    "-Wchar-subscripts" "-Wcast-align" "-Wwrite-strings" "-Wunused" "-Wuninitialized"
    "-Wpointer-arith" "-Wredundant-decls" "-Winline" "-Wformat"
    "-Wformat-security" "-Winit-self" "-Wdeprecated-declarations"
    "-Wmissing-include-dirs" "-Wmissing-declarations")
set(RAYTRACER_GCC_RELEASE_FLAGS "")

# Use the same flags as GCC
set(RAYTRACER_CLANG_DEBUG_FLAGS ${RAYTRACER_GCC_DEBUG_FLAGS})
set(RAYTRACER_CLANG_RELEASE_FLAGS ${RAYTRACER_GCC_RELEASE_FLAGS})

# Common helper that sets relevant C++ warnings and compilation flags
# see below for specific versions of the function.
# NOTE: call this function AFTER target_sources() calls
function(reaper_configure_target_common target project_label)
    target_include_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/src)
    target_compile_definitions(${target} PRIVATE RAYTRACER_BUILD_${RAYTRACER_BUILD_TYPE})
    if(MSVC)
        target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
        set_target_properties(${target} PROPERTIES PROJECT_LABEL ${project_label})
        reaper_fill_vs_source_tree(${target} ${CMAKE_CURRENT_SOURCE_DIR})
        # /MP:              enable multi-threaded compilation
        # /std:c++latest:   added for nested namespaces (vs2015)
        # NOTE: it is mutually exclusive with /std:c++14
        target_compile_options(${target} PRIVATE "/MP" "/std:c++latest")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 14)
        target_compile_options(${target} PRIVATE "-fvisibility=hidden")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 14)
        target_compile_options(${target} PRIVATE "-fvisibility=hidden")
        add_clang_tidy_flags(${target})
    else()
        message(FATAL_ERROR "Could not detect compiler")
    endif()
endfunction()

# Helper function that add default compilation flags for reaper targets
function(reaper_configure_warnings target enabled)
    if(${enabled})
        if(MSVC)
            target_compile_options(${target} PRIVATE "$<$<CONFIG:DEBUG>:${RAYTRACER_MSVC_DEBUG_FLAGS}>")
            target_compile_options(${target} PRIVATE "$<$<CONFIG:RELEASE>:${RAYTRACER_MSVC_RELEASE_FLAGS}>")
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(${target} PRIVATE "$<$<CONFIG:DEBUG>:${RAYTRACER_GCC_DEBUG_FLAGS}>")
            target_compile_options(${target} PRIVATE "$<$<CONFIG:RELEASE>:${RAYTRACER_GCC_RELEASE_FLAGS}>")
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_compile_options(${target} PRIVATE "$<$<CONFIG:DEBUG>:${RAYTRACER_CLANG_DEBUG_FLAGS}>")
            target_compile_options(${target} PRIVATE "$<$<CONFIG:RELEASE>:${RAYTRACER_CLANG_RELEASE_FLAGS}>")
        else()
            message(FATAL_ERROR "Could not detect compiler")
        endif()
    else()
        if(MSVC)
            target_compile_options(${target} PRIVATE "$<$<CONFIG:DEBUG>:${RAYTRACER_MSVC_DISABLE_WARNINGS}>")
            target_compile_options(${target} PRIVATE "$<$<CONFIG:RELEASE>:${RAYTRACER_MSVC_DISABLE_WARNINGS}>")
        endif()
    endif()
endfunction()

# Use this function for executables
function(reaper_configure_executable target project_label)
    reaper_configure_target_common(${target} ${project_label})
    reaper_configure_warnings(${target} ON)
endfunction()

# Main executable
set(RAYTRACER_BIN raytracer)

add_executable(${RAYTRACER_BIN})

target_sources(${RAYTRACER_BIN} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/float2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/float3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/types.h
)

target_link_libraries(${RAYTRACER_BIN} PRIVATE
)

reaper_configure_executable(${RAYTRACER_BIN} "Raytracer")
