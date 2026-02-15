project('gcxx', 'cpp', default_options : [
  'cpp_std=c++23',
  'warning_level=3',
])

lib_src = [
  'src/alloc.cpp',
  'src/gc.cpp',
]

if get_option('buildtype') == 'debug'
    add_project_arguments('-DDEBUG', language: 'cpp')
endif

runtime = static_library('runtime', lib_src)

test_src = [
  'src/main.cpp'
]

executable('testt', test_src, link_with: [runtime])

example_src = [
  'example/avl.cpp',
  'example/test.cpp',
]
executable('example', example_src, link_with: [runtime])
