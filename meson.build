project(
  'gamescope',
  'cpp',
  meson_version: '>=0.54.0',
  default_options: [
    'cpp_std=c++14',
    'warning_level=2',
    'force_fallback_for=wlroots,libliftoff',
  ],
)

add_project_arguments([
  '-DWLR_USE_UNSTABLE',
], language: 'cpp')

cppc = meson.get_compiler('cpp')

add_project_arguments(cppc.get_supported_arguments([
  '-Wno-unused-parameter',
  '-Wno-missing-field-initializers',
  '-Wno-c99-designator'
]), language: 'cpp')

dep_x11 = dependency('x11')
dep_xdamage = dependency('xdamage')
dep_xcomposite = dependency('xcomposite')
dep_xrender = dependency('xrender')
dep_xext = dependency('xext')
dep_xfixes = dependency('xfixes')
dep_xxf86vm = dependency('xxf86vm')
dep_xtst = dependency('xtst')
dep_xres = dependency('xres')

drm_dep = dependency('libdrm', version: '>= 2.4.105')
vulkan_dep = dependency('vulkan')

wayland_server = dependency('wayland-server')
wayland_protos = dependency('wayland-protocols', version: '>=1.17')
xkbcommon = dependency('xkbcommon')
thread_dep = dependency('threads')
cap_dep = dependency('libcap')
sdl_dep = dependency('SDL2')
pipewire_dep = dependency('libpipewire-0.3', required: get_option('pipewire'))

stb_dep = dependency('stb')

wlroots_dep = dependency(
  'wlroots',
  version: ['>= 0.15.0', '< 0.16.0'],
  fallback: ['wlroots', 'wlroots'],
  default_options: ['default_library=static', 'examples=false', 'xwayland=enabled', 'backends=libinput', 'renderers=[]'],
)

required_wlroots_features = ['xwayland', 'libinput_backend']
foreach feat : required_wlroots_features
  if wlroots_dep.get_variable('have_' + feat) != 'true'
    error('Cannot use wlroots built without ' + feat + ' support')
  endif
endforeach

shadercompiler = find_program('glslangValidator', native: true)

spirv_shader = custom_target('shader_target',
  output : 'composite.h',
  input : 'src/composite.comp',
  command : [
    shadercompiler, '-V', '-e', 'main', '--vn',
    'composite_spv', '@INPUT@', '-o', '@OUTPUT@'
  ],
  install : false,
)

liftoff_dep = dependency(
  'libliftoff',
  version: ['>= 0.1.0', '< 0.2.0'],
  fallback: ['libliftoff', 'liftoff'],
  default_options: ['default_library=static'],
)

add_project_arguments(
  '-DHAVE_PIPEWIRE=@0@'.format(pipewire_dep.found().to_int()),
  language: 'cpp',
)

src = [
  'src/steamcompmgr.cpp',
  'src/main.cpp',
  'src/wlserver.cpp',
  'src/drm.cpp',
  'src/cvt.cpp',
  'src/sdlwindow.cpp',
  'src/vblankmanager.cpp',
  'src/rendervulkan.cpp',
  'src/log.cpp',
  spirv_shader,
]

if pipewire_dep.found()
  src += 'src/pipewire.cpp'
endif

subdir('protocol')

executable(
  'gamescope',
  src,
  dependencies: [
    dep_x11, dep_xdamage, dep_xcomposite, dep_xrender, dep_xext, dep_xfixes,
    dep_xxf86vm, dep_xres, drm_dep, wayland_server, wayland_protos,
    xkbcommon, thread_dep, sdl_dep, wlroots_dep,
    vulkan_dep, liftoff_dep, dep_xtst, cap_dep, pipewire_dep, stb_dep,
  ],
  install: true,
)
