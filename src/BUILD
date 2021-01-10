load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "util",
    srcs = glob(["util/*.cpp"]),
    hdrs = glob(["include/util/*.h"]),
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "storage",
    srcs = glob(["storage/*.cpp"]),
    hdrs = glob(["include/storage/*.h"]),
    visibility = ["//test:__pkg__"],
)