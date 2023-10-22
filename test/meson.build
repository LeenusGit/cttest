# test_deps = [
#     dependency('cttest')
# ]

# inc = include_directories(
#     'containers'
# )

# basic = executable('basic',
#     cpp_args: ['-O0', '-fkeep-inline-functions'],
#     sources: files('basic.cpp'),
#     dependencies: test_deps
# )

# test('basic', basic)

# suite = executable('suite',
#     # cpp_args: ['-O0', '-fkeep-inline-functions'],
#     include_directories: inc,
#     sources: files(
#         'suite.cpp',
#     ),
#     dependencies: test_deps
# )

# test('suite', suite)

# subdir('containers')

test(
    'ut_simple',
    executable(
        'ut_simple',
        sources: 'ut_simple.cpp',
        dependencies: [
            dependency('fmt'),
            dependency('cttest'),
        ],
        # cpp_args: ['-DCTT_DEBUG']
    )
)
