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

set(DOUBLY_LINKED_LIST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/doubly_linked_list.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/doubly_linked_list.h
)

add_library(doubly_linked_list)
target_sources(doubly_linked_list
 PRIVATE
  ${DOUBLY_LINKED_LIST_SOURCES}
  )
target_include_directories(doubly_linked_list
 PRIVATE
 ${INCLUDE}
)

set(STACK_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/stack.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/stack.h
)

add_library(stack)
target_sources(stack
 PRIVATE
  ${STACK_SOURCES}
  )

target_include_directories(stack
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
  doubly_linked_list
  stack
  ui_stuff
)

set(UI_STUFF_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui_stuff.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ui_stuff.h
  )

add_library(ui_stuff)
target_sources(ui_stuff
 PRIVATE
  ${UI_STUFF_SOURCES}
  )

target_include_directories(ui_stuff
 PRIVATE
 ${INCLUDE}
 )
