
test(
    'ut_staticmap',
    executable(
        'ut_staticmap',
        sources: 'ut_staticmap.cpp',
        dependencies: test_deps,
    )
)

test(
    'ut_staticvector',
    executable(
        'ut_staticvector',
        sources: 'ut_staticvector.cpp',
        dependencies: [
            test_deps
        ],
        cpp_args: ['-fconcepts-diagnostics-depth=2']
    )
)
