#ifndef sdcard_h
#define sdcard_h
#include "ff.h"

void sdcard_init(void);
FRESULT sdcard_scan_files(char* path);
void sdcard_open_file(char* filename);
void sdcard_close_file(void);
uint8_t sdcard_eol(void);
char* sdcard_get_line(void);

#endif
