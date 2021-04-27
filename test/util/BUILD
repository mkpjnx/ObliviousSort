load("//build:common.bzl", "COPTS", "LOPTS")
load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "sort_test",
    srcs = [
        "sort_test.cpp",
    ],
    copts = COPTS,
    linkopts = LOPTS,
    deps = [
        "//src:util",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)

cc_test(
    name = "binplace_test",
    srcs = [
        "binplace_test.cpp",
    ],
    copts = COPTS,
    linkopts = LOPTS,
    deps = [
        "//src:util",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)

cc_test(
    name = "transpose_test",
    srcs = [
        "transpose_test.cpp",
    ],
    copts = COPTS,
    linkopts = LOPTS,
    deps = [
        "//src:util",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)

cc_test(
    name = "meta_orba_test",
    srcs = [
        "meta_orba_test.cpp",
    ],
    copts = COPTS,
    linkopts = LOPTS,
    deps = [
        "//src:util",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)

