include_directories(../../3rdparty/fmt/include)
add_executable(foo foo.cpp)
target_compile_features(foo PUBLIC cxx_std_20)