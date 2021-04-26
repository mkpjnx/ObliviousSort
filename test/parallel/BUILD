load("//build:common.bzl", "COPTS")
load("//build:common.bzl", "LOPTS")
load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "parallel_test",
    srcs = [
        "parallel_test.cpp",
    ],
    copts = COPTS + ["-fopenmp"],
    linkopts = LOPTS + ["-lgomp"],
    deps = [
        "//src:osort",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)

cc_binary(
    name = "parallel_timing",
    srcs = [
        "parallel_timing.cpp",
    ],
    copts = COPTS + ["-fopenmp"],
    linkopts = LOPTS + ["-lgomp"],
    deps = [
        "//src:osort",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)

cc_binary(
    name = "parallel_baseline",
    srcs = [
        "parallel_baseline.cpp",
    ],
    copts = COPTS + ["-fopenmp"],
    linkopts = LOPTS + ["-lgomp"],
    deps = [
        "//src:util",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)
