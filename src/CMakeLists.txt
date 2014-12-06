cmake_minimum_required(VERSION 2.8)

project(musli)

find_package(PkgConfig)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

set(DEBUG_FLAGS
  "-std=c99 -I .. -pthread -Wall -Wextra -g -Wfloat-equal -Wno-unused-result -Wshadow -Wstrict-aliasing"
)

set(OPTIMIZED_DEBUG_FLAGS
  "-std=c99 -I .. -pthread -Wall -Wextra -g -DNDEBUG -O2 -pipe -msse2 -fforce-addr -ftree-vectorize -funroll-loops" 
)

set(OPTIMIZING_FLAGS
  "-std=c99 -I .. -pthread -Wall -Wextra -DNDEBUG -O3 -pipe -msse2 -fforce-addr -ftree-vectorize -funroll-loops" 
)

set(CMAKE_C_FLAGS 
  ${OPTIMIZING_FLAGS}
  #${OPTIMIZED_DEBUG_FLAGS}
  #${DEBUG_FLAGS}
) 

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})


add_library(musli STATIC
  bitwise.c
  board.c
  bot_moves.c
  bot_stats.c
  game_config.c
  human.c
  player.c
  clickable_image.c
  args.c
  hash_table.c
)

add_executable(../debug
  debug.c
)

target_link_libraries(../debug
  musli
)

add_executable(../musli
  musli.c
  main_window.c
  game_config.c
)

target_link_libraries(../musli
  musli
  ${GTK3_LIBRARIES}
)
