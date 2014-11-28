cmake_minimum_required(VERSION 2.8)

project(musli)

find_package(PkgConfig)

#pkg_check_modules(GTKMM gtkmm-3.0)
#pkg_check_modules(GLIBMM glibmm-2.4)
#pkg_check_modules(GTK_PLUS gtk+-2.0)
#pkg_check_modules(GDK_PIXBUF gdk-pixbuf-2.0)


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
  #${OPTIMIZING_FLAGS}
  #${OPTIMIZED_DEBUG_FLAGS}
  ${DEBUG_FLAGS}
) 

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_executable(../debug
  board.c
  debug.c
  human.c
  player.c
  bitwise.c
)

target_link_libraries(../debug
#  ${GTKMM_LIBRARIES}
#  ${GLIBMM_LIBRARIES}
#  ${GDK_PIXBUF_LIBRARIES}
)
