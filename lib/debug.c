#include <debug.h>
#include <kernel/kernel.h>

PRINTF_FORMAT (4, 5) NO_RETURN
void debug_panic (const char *file, int line, const char *function,
                  const char *message, ...) {
    kpanic_message(message);
}