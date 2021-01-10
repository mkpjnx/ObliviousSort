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