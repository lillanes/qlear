set(sources
  "${CMAKE_CURRENT_SOURCE_DIR}/qlear.cc"
  )

add_library(qlear ${sources})

target_include_directories(qlear
  PUBLIC "${PROJECT_SOURCE_DIR}/include")

install(
  TARGETS qlear
  DESTINATION lib
  )
