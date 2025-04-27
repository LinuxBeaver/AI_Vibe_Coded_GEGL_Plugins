project('hawaiian_flowers_1', 'c',
  version: '0.1.0',
  license: 'GPL-3.0-or-later')

# Find dependencies
gegl_dep = dependency('gegl-0.4', required: true)
math_lib = meson.get_compiler('c').find_library('m', required: true)

# Define the plugin as a shared module
smooth_plugin = shared_module('hawaiin_flowers_1',
  'hawaiian_flowers_1.c',
  dependencies: [gegl_dep, math_lib],
  c_args: ['-fPIC'],
  name_prefix: '',  # GEGL plugins don't use 'lib' prefix
  install: true,
  install_dir: join_paths(get_option('libdir'), 'gegl-0.4')
)

# Optional: Add a simple test
test('smooth-test', find_program('gegl'), args: ['--version'])
