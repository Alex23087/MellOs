#include "../file_system/file_system.h"
#include "../utils/string.h"
#include "dynamic_mem.h"
#ifdef VGA_VESA
#include "../drivers/vesa/vesa.h"
#include "../drivers/vesa/vesa_text.h"
#include "../drivers/mouse.h"
#else
#include "../drivers/vga_text.h"
#endif
#include "../utils/conversions.h"

int malloc_test(){
    void * code_loc = (void*) kmalloc(10);

    if (code_loc == NULL){                          // null check
        return 1;
    }

    return 0;
}

int malloc_double_alloc_test(){
    void * code_loc = (void*) kmalloc(8 * 600 * 400);
    void * code_loc2 = (void*) kmalloc(8 * 600 * 400);

    if (code_loc == NULL || code_loc2 == NULL){     // null check
        return 1;
    }

    if (code_loc == code_loc2){                      // check if different locations
        return 1;
    } else {
        kprint("Allocated at ");
        char buf[256];
        tostring((int)code_loc, 16, buf);
        kprint(buf);
        kprint(" and ");
        tostring((int)code_loc2, 16, buf);
        kprint(buf);
        kprint("\n");
    }

    return 0;
}

int alloc_framebuffer_test(){
    Framebuffer* fb = allocate_framebuffer(600, 400);
    if (fb == NULL) return 1;
    void* mem = (void*)fb->fb;
    fb->fb = kmalloc(fb->width * fb->height * 4);
    if (fb->fb == NULL) return 1;
    if (fb->fb == mem) return 1; // Check if the allocator allocated the same block again
    kprint("Allocated framebuffer at ");
    char buf[256];
    tostring((int)fb->fb, 16, buf);
    kprint(buf);
    kprint(", previous pointer was ");
    tostring((int)mem, 16, buf);
    kprint(buf);
    kprint("\n");
    return 0;
}

int malloc_stress_test(){
    void* blocks[1000];
    int size = 2 * 1024;
    while (1) {
        for (int i = 0; 1; i++){
            blocks[i] = kmalloc(size);
            if (blocks[i] == NULL) return 1;
            kprint("Block ");
            char buf[16];
            tostring(i, 10, buf);
            kprint(buf);
            kprint(": ");
            tostring((int)blocks[i], 16, buf);
            kprint(buf);
            kprint(" Total memory allocated: ");
            tostring((i + 1) * size / 1024, 10, buf);
            kprint(buf);
            kprint(" KB");
            kprint("\n");
            if (i > 0 && blocks[i] == blocks[i - 1]) {
                kprint("Blocks ");
                char buf[16];
                tostring(i - 1, 10, buf);
                kprint(buf);
                kprint(" and ");
                tostring(i, 10, buf);
                kprint(buf);
                kprint(" are equal at ");
                tostring((int)blocks[i], 16, buf);
                kprint(buf);
                kprint("\n");
                khang();
                return 1;
            }
        }
        for (int i = 0; i < 1000; i++) {
            kfree(blocks[i], size);
        }
        size *= 2;
    }
    return 0;
}    

int run_all_mem_tests() {
    return malloc_test() + malloc_double_alloc_test() + alloc_framebuffer_test() + malloc_stress_test() + 1;
}