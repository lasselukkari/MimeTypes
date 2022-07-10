project(mime-types-lib C CXX)

file(GLOB_RECURSE src
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp

    ${CMAKE_CURRENT_LIST_DIR}/*.c
	${CMAKE_CURRENT_LIST_DIR}/*.cc
	${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

file(GLOB_RECURSE public_headers
    ${CMAKE_CURRENT_LIST_DIR}/include/public/*.h
    ${CMAKE_CURRENT_LIST_DIR}/include/public/*.hpp
)

# =================================================================
add_library(mime-types-lib
    STATIC ${src}
)

add_library(mime_types::mime_types ALIAS mime-types-lib)

target_include_directories(mime-types-lib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/public"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include/private"
)

target_link_libraries(mime-types-lib
    PUBLIC ${Boost_LIBRARIES}
    PRIVATE mime-types-lib-resources
    PUBLIC nlohmann_json::nlohmann_json
)

if(Boost_FOUND)
    target_link_libraries(mime-types-lib
        PUBLIC ${Boost_LIBRARIES}
    )

    target_compile_definitions(mime-types-lib PUBLIC "MIME_TYPES_USE_BOOST")
endif()

# =================================================================
add_library(mime-types-lib-header-only INTERFACE)
add_library(mime_types::mime_types_header_only ALIAS mime-types-lib-header-only)

target_compile_definitions(mime-types-lib-header-only INTERFACE MIME_TYPES_HEADER_ONLY=1)

target_include_directories(mime-types-lib-header-only INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/public>)
