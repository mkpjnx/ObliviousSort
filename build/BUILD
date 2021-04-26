load("@bazel_skylib//rules:common_settings.bzl", "bool_flag")

bool_flag(
    name = "use_omp",
    build_setting_default = 0,
)

bool_flag(
    name = "use_gprof",
    build_setting_default = 0,
)

bool_flag(
    name = "skip_optimize",
    build_setting_default = 0,
)

config_setting(
    name = "enable_omp",
    flag_values = {"use_omp": "1"},
)

config_setting(
    name = "enable_profile",
    flag_values = {"use_gprof": "1"},
)

config_setting(
    name = "skip_opt",
    flag_values = {"skip_optimize": "1"},
)
