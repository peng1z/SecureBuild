add_library(securebuild src/tensor_ops.cpp)
add_library(SecureBuild::securebuild ALIAS securebuild)

target_compile_features(securebuild PUBLIC cxx_std_20)
target_include_directories(
  securebuild
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_options(securebuild PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Wconversion -Wshadow>)

set_target_properties(securebuild PROPERTIES
  EXPORT_NAME securebuild
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  OUTPUT_NAME securebuild)

install(TARGETS securebuild
  EXPORT SecureBuildTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/SecureBuildConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/SecureBuildConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SecureBuild)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/SecureBuildConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/SecureBuildConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/SecureBuildConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SecureBuild)

install(EXPORT SecureBuildTargets
  FILE SecureBuildTargets.cmake
  NAMESPACE SecureBuild::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SecureBuild)
