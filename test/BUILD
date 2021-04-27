load("//build:common.bzl", "COPTS", "LOPTS")
load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "recsort_comp_test",
    srcs = [
        "recsort_comp_test.cpp",
    ],
    copts = COPTS,
    linkopts = LOPTS,
    deps = [
        "//src:osort",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)
