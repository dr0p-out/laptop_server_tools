project(LANGUAGES C)
add_executable(supplywatchd
  data.c
  pull_plug.c
  daemon.c)

install(TARGETS supplywatchd)
