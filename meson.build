project('gcxx', 'cpp', default_options : [
  'cpp_std=c++23',
  'warning_level=3',
])

lib_src = [
  'src/alloc.cpp'
]

runtime = static_library('runtime', lib_src)

test_src = [
  'src/main.cpp'
]

executable('testt', test_src, link_with: [runtime])

