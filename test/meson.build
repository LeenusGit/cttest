test_deps = [
    dependency('cttest')
]

inc = include_directories(
    'containers'
)

basic = executable('basic',
    cpp_args: ['-O0', '-fkeep-inline-functions'],
    sources: files('basic.cpp'),
    dependencies: test_deps
)

test('basic', basic)

suite = executable('suite',
    # cpp_args: ['-O0', '-fkeep-inline-functions'],
    include_directories: inc,
    sources: files(
        'suite.cpp',
    ),
    dependencies: test_deps
)

test('suite', suite)
