load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test (
    name = "sort_test",
    srcs = [
        "sort_test.cpp",
    ],
    deps = [
        "//src:util",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)

cc_test (
    name = "binplace_test",
    srcs = [
        "binplace_test.cpp",
    ],
    deps = [
        "//src:util",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)

cc_test (
    name = "storage_test",
    srcs = [
        "storage_test.cpp",
    ],
    deps = [
        "//src:storage",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)

cc_test (
    name = "orba_test",
    srcs = [
        "orba_test.cpp",
    ],
    deps = [
        "//src:osort",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)

cc_test (
    name = "recsort_test",
    srcs = [
        "recsort_test.cpp",
    ],
    deps = [
        "//src:osort",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)

cc_test (
    name = "osort_test",
    srcs = [
        "osort_test.cpp",
    ],
    deps = [
        "//src:osort",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)

cc_test (
    name = "overflow_test",
    srcs = [
        "overflow_test.cpp",
    ],
    deps = [
        "//src:osort",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)

cc_test (
    name = "parallel_test",
    copts =["-fopenmp"],
    linkopts= ["-lgomp"],
    srcs = [
        "parallel_test.cpp",
    ],
    deps = [
        "//src:osort",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)
