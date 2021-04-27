load("//build:common.bzl", "COPTS", "LOPTS")
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "util",
    srcs = glob(["util/*.cpp"]),
    hdrs = glob(["include/util/*.h"]),
    copts = COPTS,
    linkopts = LOPTS,
    visibility = ["//test:__subpackages__"],
)

cc_library(
    name = "storage",
    srcs = glob(["storage/*.cpp"]),
    hdrs = glob(["include/storage/*.h"]),
    copts = COPTS,
    linkopts = LOPTS,
    visibility = ["//test:__subpackages__"],
    deps = ["util"],
)

cc_library(
    name = "osort",
    srcs = glob(["osort/*.cpp"]),
    hdrs = glob(["include/osort/*.h"]),
    copts = COPTS,
    linkopts = LOPTS,
    visibility = ["//test:__subpackages__"],
    deps = [
        "storage",
        "util",
    ],
)
