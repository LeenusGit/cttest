test_deps = [
    dependency('cttest')
]

basic = executable('basic',
    cpp_args: ['-O0', '-fkeep-inline-functions'],
    sources: files('basic.cpp'),
    dependencies: test_deps
)

test('basic', basic)
