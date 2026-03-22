include(FetchContent)

find_package(GTest QUIET CONFIG)

if(NOT TARGET GTest::gtest_main)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  set(BUILD_GTEST ON CACHE BOOL "" FORCE)

  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
  FetchContent_MakeAvailable(googletest)
endif()

add_executable(securebuild_tests test_tensor_ops.cpp)

target_link_libraries(securebuild_tests PRIVATE SecureBuild::securebuild GTest::gtest_main)

target_compile_features(securebuild_tests PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(securebuild_tests)
