inc = include_directories('.')

lib = library('cttest',
)

dep = declare_dependency(
    include_directories: inc,
    dependencies: deps
)

meson.override_dependency('cttest', dep)