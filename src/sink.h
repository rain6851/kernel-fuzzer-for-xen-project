/*
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: MIT
 */
#ifndef SINK_H
#define SINK_H

/*
 * List all sink points here. When the kernel executes any of these functions
 * we will report a crash to AFL and stop the fuzzer.
 */
enum sink_enum {
    OOPS_BEGIN,
    PANIC,
    PAGE_FAULT,
    KASAN_REPORT,
    UBSAN_PROLOGUE,

    // Add new sink declarations here

    __SINK_MAX
    // Don't place new sink declaration here
};

/* Now define what symbol each enum entry corresponds to in the debug json */
const char *sinks[] = {
    [PANIC] = "panic",

    /*
     * We can define as many sink points as we want. These sink points don't have
     * to be strictly functions that handle "crash" situations. We can define any
     * code location as a sink point that we would want to know about if it is reached
     * during fuzzing. For example the testmodule triggering a NULL-deref doesn't crash
     * the kernel, it simply causes an "oops" message to be printed to the kernel logs.
     * However, if there is an input that causes something like that then it warrants
     * being recorded.
     *
     * So in essence we can define the sink points as anything of interest that we would
     * want AFL to record if its reached.
     */
    [OOPS_BEGIN] = "oops_begin",

    /*
     * We interpret a page fault as a crash situation since we really shouldn't
     * encounter any. The VM forks are running without any devices so even if this
     * is a legitimate page-fault that would page memory back in, it won't be able
     * to do that since there is no disk.
     */
    [PAGE_FAULT] = "page_fault", // NOTE: after kernel 5.8 it is named asm_exc_page_fault

    /*
     * Catch when KASAN starts to report an error it caught.
     */
    [KASAN_REPORT] = "kasan_report",

    /*
     * Catch when UBSAN starts to report an error it caught.
     */
    [UBSAN_PROLOGUE] = "ubsan_prologue",
};

addr_t sink_vaddr[__SINK_MAX] =
{
    [0 ... __SINK_MAX-1] = 0,

    /*
     * You can manually define each sink's virtual address here. For example:
    [PAGE_FAULT] = 0xffffffdeadbeef,
     */
};

addr_t sink_paddr[__SINK_MAX] =
{
    [0 ... __SINK_MAX-1] = 0,

    /*
     * You can manually define each sink's physical address here. For example:
    [PAGE_FAULT] = 0xdeadbeef,
     */
};

#endif
