include(FetchContent)
#include(FindPkgConfig)

if(ESL_DEPENDENCIES_USE_CONAN)
    message(STATUS "Using Conan")
    include(${CMAKE_BINARY_DIR}/conan/conan_toolchain.cmake)
endif()

if(ESL_DEPENDENCIES_USE_VCPKG)
    message(STATUS "Using VCPKG")
    if(WIN32)
        set(USER_HOME_DIRECTORY $ENV{USERPROFILE})
    else()
        set(USER_HOME_DIRECTORY $ENV{HOME})
    endif()
    message(STATUS "User Home Directory: ${USER_HOME_DIRECTORY}")
    include(${USER_HOME_DIRECTORY}/opt/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

if(ESL_DEPENDENCIES_USE_PKGCONFIG)
    find_package(PkgConfig QUIET)
endif()

function(find_custom_package NAME URL TAG)
    # Default, try 'find_package'. VCPKG or Conan may be used, if enabled
    if(NOT ${NAME}_FOUND)
        message(STATUS "Try to find ${NAME} by find_package")
        find_package(${NAME} QUIET)
        if(${NAME}_FOUND)
            message(STATUS "${NAME} has been found by using find_package")
        endif()
    endif()

    if(NOT ${NAME}_FOUND)
        message(STATUS "Try to find ${NAME} by FetchContent")
        set(COMPILE_UNITTESTS OFF)
        FetchContent_Declare(
            ${NAME}
            GIT_REPOSITORY ${URL}
            GIT_TAG ${TAG}
            GIT_SHALLOW TRUE
            OVERRIDE_FIND_PACKAGE # 'find_package(...)' will call 'FetchContent_MakeAvailable(...)'
        )
        find_package(${NAME} QUIET)
        if(${NAME}_FOUND)
            message(STATUS "${NAME} has been found by using FetchContent")
        endif()
    endif()

    if(NOT ${NAME}_FOUND)
        message(FATAL_ERROR "${NAME} NOT found")
    endif()
endfunction()

function(find_package_esa)
    find_custom_package(esa https://github.com/SLukasDE/esa master)
endfunction()

function(find_package_esl)
    find_custom_package(esl https://github.com/SLukasDE/esl master)
endfunction()

function(find_package_GnuTLS) # GnuTLS::GnuTLS
    # Default, try 'find_package'. VCPKG or Conan is used, if enabled
    if(NOT GNUTLS_FOUND)
        message(STATUS "Try to find GnuTLS by find_package")
        find_package(GnuTLS QUIET)
        if(GNUTLS_FOUND)
            message(STATUS "GnuTLS has been found by using find_package")
        endif()
    endif()

    # If still not found, try to download the sourcecode and compile it or use the prebuild binaries from the original website
    if(NOT GNUTLS_FOUND)
        if(WIN32)
            message(STATUS "Try to fetch prebuild binaries of GnuTLS from original website")
            if(NOT EXISTS "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5-w64.zip")
                file(
                    DOWNLOAD "https://www.gnupg.org/ftp/gcrypt/gnutls/v3.7/gnutls-3.7.5-w64.zip" "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5-w64.zip"
                    EXPECTED_HASH MD5=d8bbbc6e0d168f0274eabba934e283cc
                    #SHOW_PROGRESS
                    )
            endif()
            if(NOT EXISTS "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5-w64")
                execute_process(
                    COMMAND $ENV{ProgramFiles}/7-Zip/7z x "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5-w64.zip" "-o${CMAKE_BINARY_DIR}/external/*"
                )
            endif()
            if(EXISTS "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5-w64/win64-build/lib/includes")
                file(RENAME ${CMAKE_BINARY_DIR}/external/gnutls-3.7.5-w64/win64-build/lib/includes ${CMAKE_BINARY_DIR}/external/gnutls-3.7.5-w64/win64-build/lib/gnutls)
            endif()
            if(EXISTS "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5-w64")
                add_library(GnuTLS::GnuTLS UNKNOWN IMPORTED)
                set_target_properties(GnuTLS::GnuTLS PROPERTIES
                    IMPORTED_LOCATION             "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5-w64/win64-build/bin/libgnutls-30.dll"
                    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5-w64/win64-build/lib")
                set(GNUTLS_FOUND TRUE)
            endif()

            if(GNUTLS_FOUND)
                message(STATUS "GnuTLS has been found by fetiching prebuild binaries from original website")
            endif()
        else()
            message(STATUS "Try to fetch source code of GnuTLS from original website")
            if(NOT EXISTS "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5.tar.xz")
                file(
                    DOWNLOAD "https://www.gnupg.org/ftp/gcrypt/gnutls/v3.7/gnutls-3.7.5.tar.xz" "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5.tar.xz"
                    EXPECTED_HASH MD5=749eb6f0e5646b90dd00521b7853b7c7
                    #SHOW_PROGRESS
                    )
            endif()
            if(EXISTS "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5.tar.xz")
                file(ARCHIVE_EXTRACT INPUT "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5.tar.xz" DESTINATION "${CMAKE_BINARY_DIR}/external")
            endif()
            if(EXISTS "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5")
                execute_process(
                    COMMAND "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5/configure" --prefix=${CMAKE_BINARY_DIR}/external/gnutls
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5"
                )
                execute_process(
                    COMMAND make
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5"
                )
                execute_process(
                    COMMAND make install
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/external/gnutls-3.7.5"
                )
            endif()
            if(EXISTS "${CMAKE_BINARY_DIR}/external/gnutls")
                add_library(GnuTLS::GnuTLS UNKNOWN IMPORTED)
                set_target_properties(GnuTLS::GnuTLS PROPERTIES
                    IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/external/gnutls/lib/libgnutls.so.30.33.0"
                    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/external/gnutls/include")
                set(GNUTLS_FOUND TRUE)
            endif()

            if(GNUTLS_FOUND)
                message(STATUS "GnuTLS has been found by fetiching the source code from original website")
            endif()
        endif()
    endif()

    if(NOT GNUTLS_FOUND)
        message(FATAL_ERROR "GnuTLS NOT found")
    endif()
endfunction()
