project('colorcubes1', 'c',
  version: '0.1.0',
  license: 'GPL-3.0-or-later',
  meson_version: '>=0.50.0',
  default_options: ['c_std=gnu99']
)

# Find dependencies with version check
gegl = dependency('gegl-0.4', version: '>=0.4.26', required: true)
m_dep = meson.get_compiler('c').find_library('m', required: false)

# Plugin definition with proper install path
gegl_plugin_dir = gegl.get_pkgconfig_variable('libdir') / 'gegl-0.4'

shared_module('colorcubes1',
  'colorcubes1.c',
  dependencies: [gegl, m_dep],
  name_prefix: '',
  c_args: [
    '-DHAVE_CONFIG_H',
    '-DG_LOG_DOMAIN="GEGL-grok"',
    '-fPIC'
  ],
  install: true,
  install_dir: gegl_plugin_dir
)
