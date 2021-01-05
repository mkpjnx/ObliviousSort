load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "sorting",
    srcs = ["sorting.cpp"],
    hdrs = glob(["include/*.h"]),
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "binplace",
    srcs = ["binplace.cpp"],
    hdrs = glob(["include/*.h"]),
    visibility = ["//test:__pkg__"],
)