const std = @import("std");

pub fn build(b: *std.Build) !void {
    const src_files: []const []const u8 = &.{
        "main.c",
    };

    const exe = b.addExecutable(.{
        .name = "urigin",
        .target = b.graph.host, //Should be PSP or PS2
    });

    exe.addCSourceFiles(.{
        .root = .{ .cwd_relative = "src" },
        .files = src_files,
        .flags = &.{"-d"},
    });

    exe.linkLibC();
    exe.linkSystemLibrary("SDL3");

    b.installArtifact(exe);

    const run_exe = b.addRunArtifact(exe);
    const run_step = b.step("run", "run produced executable.");
    run_step.dependOn(&run_exe.step);
}
