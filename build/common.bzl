""" Global variables exported to other BUILD files

COPTS are compiler options dependent on flags passed into bazel.
LOPTS are linker options dependent also on bazel options
"""

COPTS = select({
            "//build:enable_omp": ["-fopenmp"],
            "//conditions:default": ["-Wno-unknown-pragmas"],
        }) + \
        select({
            "//build:enable_profile": ["-pg"],
            "//conditions:default": [],
        }) + \
        select({
            "//build:skip_opt": [],
            "//conditions:default": ["-O3"],
        })

LOPTS = select({
            "//build:enable_omp": ["-lgomp"],
            "//conditions:default": [],
        }) + \
        select({
            "//build:enable_profile": ["-pg"],
            "//conditions:default": [],
        })
