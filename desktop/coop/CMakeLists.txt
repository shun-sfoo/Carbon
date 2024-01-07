set(CMAKE_C_STANDARD 99)

macro(ex name)
  add_executable(${name} ${name}.c)
  target_link_libraries(${name} PRIVATE PkgConfig::glib PkgConfig::gobject)
endmacro()

ex(ex1)
ex(ex2)
ex(ex3)
ex(ex4)
ex(ex5)

add_subdirectory(tdouble)
add_subdirectory(conn)
