load("@rules_cc//cc:defs.bzl", "cc_test")

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
