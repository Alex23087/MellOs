#ifdef VGA_VESA

#include "../../drivers/vesa/vesa.h"
#include "../../drivers/vesa/vesa_text.h"
#include "../../misc/colours.h"
#include "../../GUI/gui.h"

#include "../../utils/typedefs.h"
#include "../../data_structures/list.h"
#include "../../GUI/vterm.h"
#include "../../GUI/mouse_handler.h"

static List* windows = NULL;
static Window* mouse_window = NULL;

#define FDEF(name) void name(const char* s)

static Framebuffer* fb;
static Framebuffer* fb2;

static void swap_framebuffers(){
    Framebuffer* tmp = fb;
    fb = fb2;
    fb2 = tmp;
}

void __vell_draw(int from_x, int from_y, int to_x, int to_y){
    // fb_clear_screen_col_VESA(VESA_CYAN, *fb);
    fb_draw_gradient(0, 0, fb->width, fb->height, VESA_MAGENTA, VESA_CYAN, fb);
    
    List* current = windows;
    while(current != NULL){
        Window* w = (Window*)current->data;
        if (w->draw_frame){
            fb_fill_rect(w->x, w->y, w->width + BORDER_WIDTH * 2, TITLEBAR_HEIGHT, VESA_WHITE, *fb);
            fb_draw_rect(w->x, w->y + TITLEBAR_HEIGHT, w->width + BORDER_WIDTH, w->height + BORDER_WIDTH, BORDER_WIDTH, VESA_WHITE, fb);
            fb_draw_string(w->x + BORDER_WIDTH * 2, w->y + BORDER_WIDTH, w->title, VESA_BLACK, 1.8, 1.8, *fb);
            blit_all_at_only(w->fb, fb, w->x+BORDER_WIDTH, w->y + BORDER_WIDTH + TITLEBAR_HEIGHT, from_x, from_y, to_x, to_y);
        }else{
            blit_all_at_only(w->fb, fb, w->x, w->y, from_x, from_y, to_x, to_y);
        }
        current = current->next;
    }
    
    blit_all_at_only(fb, vga_fb, 0, 0, from_x, from_y, to_x, to_y);
    if (mouse_window != NULL){
        blit_all_at_only(mouse_window->fb, vga_fb, mouse_window->x, mouse_window->y, from_x, from_y, to_x, to_y);
    }
    swap_framebuffers();
}

void _vell_draw(){
    __vell_draw(0, 0, vga_fb->width, vga_fb->height);
}

FDEF(vell){
    if (fb == NULL) {
        fb = allocate_full_screen_framebuffer();
        fb->fb = kmalloc(fb->width * fb->height * 4);
        fb2 = allocate_full_screen_framebuffer();
        fb2->fb = kmalloc(fb2->width * fb2->height * 4);
        _init_vterm();
        _init_mouse_handler();
        _vell_draw();
    } else {
        _deinit_mouse_handler();
        _deinit_vterm();
        deallocate_framebuffer(fb);
        deallocate_framebuffer(fb2);
        fb_clear_screen_col_VESA(VESA_BLACK, *vga_fb);
        fb = NULL;
        fb2 = NULL;
    }
}

void _vell_register_window(Window* w){
    list_append(&windows, w);
    // kprint("Window registered\n");
}

void _vell_deregister_window(Window* w){
    list_remove(&windows, w);
}

void _vell_register_mouse(Window* w){
    mouse_window = w;
}

void _vell_deregister_mouse(Window* w){
    mouse_window = NULL;
}

void _vell_mouse_move(int old_x, int old_y, int new_x, int new_y){
    // blit_all_at_only(mouse_window->fb, vga_fb, new_x, new_y, new_x-16, new_y-16, new_x + 16, new_y + 16);
    // __vell_draw(old_x, old_y, old_x + 16, old_y + 16);
    blit_all_at_only(fb2, vga_fb, 0, 0, old_x, old_y, old_x + 16, old_y + 16);
    // blit_all_at(fb, vga_fb, 0, 0);
    blit_all_at(mouse_window->fb, vga_fb, new_x, new_y);
    // _vell_draw();
    // kprint("Mouse moved\n");
    // kprint_hex(old_x);
    // kprint(" ");
    // kprint_hex(old_y);
    // kprint(" ");
    // kprint_hex(new_x);
    // kprint(" ");
    // kprint_hex(new_y);
    // kprint("\n");
}

#endif