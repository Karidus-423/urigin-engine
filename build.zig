const std = @import("std");

pub fn build(b: *std.Build) !void {
    const src_files: []const []const u8 = &.{
        "main.c",
        "events.c",
        "setup.c",
        "update.c",
    };

    const exe = b.addExecutable(.{
        .name = "Pichoo",
        .target = b.graph.host,
    });

    exe.addCSourceFiles(.{
        .root = .{ .cwd_relative = "src" },
        .files = src_files,
    });

    exe.linkLibC();
    exe.linkSystemLibrary("SDL3");

    b.installArtifact(exe);

    const run_exe = b.addRunArtifact(exe);
    const run_step = b.step("run", "run produced executable.");
    run_step.dependOn(&run_exe.step);
}
