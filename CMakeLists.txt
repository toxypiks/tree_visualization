cmake_minimum_required(VERSION 3.16)
project(tree_visualization)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(raylib)

set(TREE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tree.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tree.h
  )

add_library(tree)
target_sources(tree
 PRIVATE
  ${TREE_SOURCES}
  )

target_include_directories(tree
 PRIVATE
 ${INCLUDE}
 )

set(MAIN_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
  )

add_executable(main)
target_sources(main
 PRIVATE
  ${MAIN_SRC}
)

target_link_libraries(main
  ${raylib_LIBRARIES}
  tree
)

