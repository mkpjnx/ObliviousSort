load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test (
    name = "bitonic_sort_test",
    srcs = [
        "common/bitonic_sort_test.cpp",
    ],
    deps = [
        "//libUtil:bitonic_sort",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)