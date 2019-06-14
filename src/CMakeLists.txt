set(sources
  "${CMAKE_CURRENT_SOURCE_DIR}/qlear.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/utils.cc"
  )

add_library(qlear ${sources})

target_include_directories(
  qlear PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(
  TARGETS qlear
  EXPORT qlear-config
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
  EXPORT qlear-config
  NAMESPACE qlear::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qlear)
