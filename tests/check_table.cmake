# Binary search requires unique extension keys in case-insensitive order.
file(STRINGS "${SOURCE_FILE}" rows
  REGEX "^[ \t]*\\{[ \t]*\"[^\"]+\"[ \t]*,")
if(NOT rows)
  message(FATAL_ERROR "No extension table entries found in ${SOURCE_FILE}")
endif()

foreach(row IN LISTS rows)
  string(REGEX MATCH "\"([^\"]+)\"" match "${row}")
  string(TOLOWER "${CMAKE_MATCH_1}" extension)
  if(DEFINED previous)
    if(NOT "${previous}" STRLESS "${extension}")
      message(FATAL_ERROR
        "Extension keys must be sorted and unique: '${previous}' precedes '${extension}'")
    endif()
  endif()
  set(previous "${extension}")
endforeach()
