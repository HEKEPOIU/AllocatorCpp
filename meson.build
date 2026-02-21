project(
  'allocator_cpp',
  'c',
  'cpp',
  default_options: ['default_library=static', 'cpp_std=c++23'],
)

allocator_include_dir = include_directories('./src/')

allocator_lib = static_library(
  meson.project_name(),
  'src/allocator/linear_allocator.cpp',
  include_directories: allocator_include_dir,
)
allocator_dep = declare_dependency(link_with: allocator_lib, include_directories: allocator_include_dir)

test_dep = dependency(
  'catch2-with-main',
  default_options: ['tests=false', 'install=false'],
  static: true,
)

test_exe = executable(
  'allocator_cpp_test',
  'tests/linear_allocator_test.cpp',
  dependencies: [test_dep, allocator_dep],
)

test('allocator test', test_exe, args: ['--colour-mode', 'ansi'])
