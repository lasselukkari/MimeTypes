find_package(Threads REQUIRED)

if(UNIX)
    find_package(DL REQUIRED)
endif()

set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE STRING "FETCHCONTENT_UPDATES_DISCONNECTED" FORCE)

include(FetchContent)

# boost-cmake
if(WIN32)
    set(Boost_USE_STATIC_LIBS ON CACHE STRING "Boost_USE_STATIC_LIBS" FORCE)
    set(Boost_USE_STATIC_RUNTIME ON CACHE STRING "Boost_USE_STATIC_RUNTIME" FORCE)
endif()

find_package(Boost 1.65.1 COMPONENTS thread log log_setup system program_options filesystem coroutine locale regex unit_test_framework serialization)
if(Boost_FOUND)
    message(STATUS "** Boost Include: ${Boost_INCLUDE_DIR}")
    message(STATUS "** Boost Libraries Directory: ${Boost_LIBRARY_DIRS}")
    message(STATUS "** Boost Libraries: ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    add_compile_definitions("MIME_TYPES_USE_BOOST")
else()
    if(WIN32)
        message(WARNING "Plase check your vcpkg settings or global environment variables for the boost library.")
    else()
        FetchContent_Declare(boost_cmake
            GIT_REPOSITORY https://github.com/Orphis/boost-cmake.git
            GIT_TAG d3951bc7f0b9d09005f92aedcf6acfc595f050ea)

        FetchContent_GetProperties(boost_cmake)
        if(NOT boost_cmake_POPULATED)
            FetchContent_Populate(boost_cmake)
            add_subdirectory(${boost_cmake_SOURCE_DIR} ${boost_cmake_BINARY_DIR} EXCLUDE_FROM_ALL)
            add_compile_definitions("MIME_TYPES_USE_BOOST")
        endif()
    endif()
endif()


# cmrc
FetchContent_Declare(cmrc
    GIT_REPOSITORY https://github.com/vector-of-bool/cmrc.git
    GIT_TAG a64bea50c05594c8e7cf1f08e441bb9507742e2e)

FetchContent_GetProperties(cmrc)
if(NOT cmrc_POPULATED)
    FetchContent_Populate(cmrc)
    add_subdirectory(${cmrc_SOURCE_DIR} ${cmrc_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


# nlohmann_json
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.10.4)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


# mime_js
FetchContent_Declare(mime_js
  GIT_REPOSITORY https://github.com/broofa/mime.git
  GIT_TAG main)

FetchContent_GetProperties(mime_js)
if(NOT mime_js_POPULATED)
  FetchContent_Populate(mime_js)
  cmrc_add_resource_library(mime-types-lib-resources
    NAMESPACE mime_types::data::rc
    WHENCE ${mime_js_SOURCE_DIR}
    "${mime_js_SOURCE_DIR}/types/standard.js" "${mime_js_SOURCE_DIR}/types/other.js")
endif()