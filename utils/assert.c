#ifdef VGA_VESA
#include "../drivers/vesa_text.h"
#else
#include "../drivers/vga_text.h"
#endif
#include "../misc/colours.h"

extern char ker_tty[4000];

void assert(uint8_t condition){
    if(!condition){
        clear_screen_col(ERROR_COLOUR);

        set_cursor_pos_raw(0);
        
        kprint("assert failed");
    }
}
