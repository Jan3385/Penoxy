project(
  'Penoxy',
  'cpp',
  'c',
  version: '0.1.0',
  default_options: ['warning_level=3', 'cpp_std=c++23', 'default_library=static'],
)

inc = include_directories('include')

sources = files(
  'src/Engine.cpp',
  'src/debug/Logger.cpp',
  'src/main.cpp',
  'src/rendering/OpenGL/GLRender.cpp',
  'src/rendering/OpenGL/GLWindow.cpp',
)

gl_dep = dependency('gl')
glfw_dep = dependency('glfw3', fallback: ['glfw', 'glfw_dep'])
imgui_dep = dependency(
  'imgui',
  fallback: ['imgui', 'imgui_dep'],
  default_options: ['glfw=enabled', 'opengl=enabled', 'vulkan=disabled'],
)

glad_inc = include_directories('libs/glad/include')
glad_sources = files('libs/glad/src/glad.c')

executable(
  'Penoxy',
  sources + glad_sources,
  include_directories: [inc, glad_inc],
  dependencies: [gl_dep, glfw_dep, imgui_dep],
  cpp_pch: 'pch/pch.h',
)
