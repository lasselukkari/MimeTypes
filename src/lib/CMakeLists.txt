project(mime-types-lib C CXX)

file(GLOB_RECURSE src
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp

    ${CMAKE_CURRENT_LIST_DIR}/*.c
	${CMAKE_CURRENT_LIST_DIR}/*.cc
	${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

file(GLOB_RECURSE public_headers
    ${CMAKE_CURRENT_LIST_DIR}/include/*.h
    ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp
)

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
    PUBLIC fmt::fmt
    PUBLIC nlohmann_json::nlohmann_json
)