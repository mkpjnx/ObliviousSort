load("@rules_cc//cc:defs.bzl", "cc_test")

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

cc_binary (
    name = "parallel_timing",
    copts =["-fopenmp"],
    linkopts= ["-lgomp"],
    srcs = [
        "parallel_timing.cpp",
    ],
    deps = [
        "//src:osort",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)

cc_binary (
    name = "parallel_baseline",
    copts =["-fopenmp"],
    linkopts= ["-lgomp"],
    srcs = [
        "parallel_baseline.cpp",
    ],
    deps = [
        "//src:util",
        "@gtest//:gtest",
        "@gtest//:gtest_main" # Only if hello_test.cc has no main()
    ],
)
