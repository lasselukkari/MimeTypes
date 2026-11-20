cmake_minimum_required(VERSION 3.20)
project(MimeTypes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MIMETYPES_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if(MIMETYPES_ENABLE_SANITIZERS AND NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  message(FATAL_ERROR "Sanitizers require GCC or Clang")
endif()

function(mimetypes_configure_target target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
  endif()

  if(MIMETYPES_ENABLE_SANITIZERS)
    target_compile_options(${target} PRIVATE
      -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer)
    target_link_options(${target} PRIVATE -fsanitize=address,undefined)
  endif()
endfunction()

add_library(MimeTypes STATIC MimeTypes.cpp)
target_include_directories(MimeTypes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
mimetypes_configure_target(MimeTypes)

include(CTest)
if(BUILD_TESTING)
  add_executable(mime_types_tests tests/mime_types_tests.cpp)
  target_link_libraries(mime_types_tests PRIVATE MimeTypes)
  mimetypes_configure_target(mime_types_tests)
  foreach(group forward reverse aliases case-folding skip null)
    add_test(NAME mime_types_${group} COMMAND mime_types_tests ${group})
  endforeach()
  add_test(NAME mime_types_table_order
    COMMAND ${CMAKE_COMMAND}
      "-DSOURCE_FILE=${CMAKE_CURRENT_SOURCE_DIR}/MimeTypes.cpp"
      -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/check_table.cmake")
endif()
