cmake_minimum_required(VERSION 3.23)
project(learn_csapp C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

file(GLOB SRCS ./src/*/*.c)
foreach(SRC ${SRCS})
    get_filename_component(EXEC_NAME ${SRC} NAME_WE)
    add_executable(${EXEC_NAME} ${SRC})
    target_include_directories(${EXEC_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set_target_properties(${EXEC_NAME} PROPERTIES COMPILE_FLAGS "-std=c99")
endforeach()
