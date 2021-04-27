load("//build:common.bzl", "COPTS", "LOPTS")
load("@rules_cc//cc:defs.bzl", "cc_test")

# need to force link omp library, but not necessarily -fopenmp
PARA_COPTS = COPTS

PARA_LOPTS = LOPTS + ["-lgomp"]

cc_test(
    name = "parallel_test",
    srcs = [
        "parallel_test.cpp",
    ],
    copts = PARA_COPTS,
    linkopts = PARA_LOPTS,
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
    copts = PARA_COPTS,
    linkopts = PARA_LOPTS,
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
    copts = PARA_COPTS,
    linkopts = PARA_LOPTS,
    deps = [
        "//src:util",
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)
