project('hanoi', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'default_library=static',
    'cpp_std=c++17',
    'werror=true'
  ]
)

fmt_dep = dependency('fmt', version: '>=11.0.2')
sdl2_dep = dependency('sdl2', version:'>=2.30.6')
sdl2main_dep = dependency('sdl2main', required: false)

exe = executable('hanoi',
  'src/hanoi.cpp',
  'src/HanoiState.cpp',
  'src/DiskRod.cpp',
  dependencies: [
    sdl2_dep,
    sdl2main_dep,
    fmt_dep,
  ],
  install : true
)
