#ifndef USO_CONSOLE_H
#define USO_CONSOLE_H

#define USO_COLOR_BLACK   "\033[0;37;30m"
#define USO_COLOR_RED     "\033[0;37;31m"
#define USO_COLOR_GREEN   "\033[0;37;32m"
#define USO_COLOR_YELLOW  "\033[0;37;33m"
#define USO_COLOR_BLUE    "\033[0;37;34m"
#define USO_COLOR_MAGENTA "\033[0;37;35m"
#define USO_COLOR_CYAN    "\033[0;37;36m"
#define USO_COLOR_WHITE   "\033[0;37;37m"

#define USO_COLOR_NORMAL "\033[0m"

void uso_colorline (const char *label, const char *format, const char *color, ...);

#endif
