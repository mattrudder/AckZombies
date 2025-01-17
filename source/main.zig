const std = @import("std");

const Kernel = opaque {};

extern fn GetKernel() callconv(.C) *Kernel;
extern fn KernelInit(*Kernel) callconv(.C) bool;
extern fn KernelDestroy(*Kernel) callconv(.C) void;
extern fn KernelTick(*Kernel) callconv(.C) bool;

pub fn main() !void {
    const ptr = GetKernel();
    if (!KernelInit(ptr))
        @panic("kernel init failed");

    while (KernelTick(ptr)) {}
    KernelDestroy(ptr);
}
