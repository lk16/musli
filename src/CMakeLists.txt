cmake_minimum_required(VERSION 2.8)

project(musli)

find_package(PkgConfig)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

set(FLAGS
" -std=c99 -I .. -pthread -Wall -Wmissing-prototypes -Wstrict-prototypes -Wextra -Wfloat-equal -Wno-unused-result -Wshadow -Wstrict-aliasing -pipe"
)


set(DEBUG_FLAGS "${FLAGS} -g")
set(OPTMISING_FLAGS "${FLAGS} -O3 -fforce-addr -ftree-vectorize -funroll-loops")

set(CMAKE_C_FLAGS 
  ${OPTMISING_FLAGS}
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
  hash_table.c
  pvs_helper.c
  player_name.c
  bot_random.c
  bot_least.c
  bot_stable.c
  svg_helper.c
  bot_most.c
  testmm.c
  mtdf_helper.c
)

add_executable(../tournament
  tournament.c
  player_name.c
)

target_link_libraries(../tournament
  musli
  m
)

add_executable(../musli
  musli.c
  main_window.c
  game_config.c
  args.c
)

target_link_libraries(../musli
  musli
  ${GTK3_LIBRARIES}
)
