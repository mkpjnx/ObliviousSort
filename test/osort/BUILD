load("//build:common.bzl", "COPTS")
load("//build:common.bzl", "LOPTS")
load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "orba_test",
    srcs = [
        "orba_test.cpp",
    ],
    copts = COPTS,
    linkopts = LOPTS,
    deps = [
        "//src:osort",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)

cc_test(
    name = "recsort_test",
    srcs = [
        "recsort_test.cpp",
    ],
    copts = COPTS,
    linkopts = LOPTS,
    deps = [
        "//src:osort",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)

cc_test(
    name = "osort_test",
    srcs = [
        "osort_test.cpp",
    ],
    copts = COPTS,
    linkopts = LOPTS,
    deps = [
        "//src:osort",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)

cc_test(
    name = "overflow_test",
    srcs = [
        "overflow_test.cpp",
    ],
    copts = COPTS,
    linkopts = LOPTS,
    deps = [
        "//src:osort",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)
