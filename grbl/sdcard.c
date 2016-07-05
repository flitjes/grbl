#include "grbl.h"
#include "sdcard.h"
#include "diskio.h"
#include "ff.h"
#include <util/delay.h>
#include <string.h>

FATFS fs;
FIL fin;

char line[80];
char sec[512];
char msg[30];

void sdcard_init(void){
    FRESULT res;
    _delay_ms(100);
	
	res = f_mount(&fs,"1", 0);
    printString("SDcard file list:\n");
    sdcard_scan_files("/");
}

FRESULT sdcard_scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
){
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                sprintf(&path[i = strlen(path)], "/%s", fno.fname);
                res = sdcard_scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printString(path);
                printString("/");
                printString(fno.fname);
                printString("\n");
            }
        }
        f_closedir(&dir);
    }

    return res;
}

void sdcard_open_file(char* filename){
	FRESULT res;
	res = f_open(&fin, filename, FA_OPEN_EXISTING | FA_READ);
	sprintf(msg,"fopen result: %02i\r\n",res);
	printString(msg);
}

void sdcard_close_file(void){
    printString("Closing\n");
    f_close(&fin);
}

uint8_t sdcard_eol(void){
    uint8_t eol = f_eof(&fin);
    return eol;
}

char* sdcard_get_line(void){
	f_gets(line, sizeof(line), &fin);
    return (char*)&line;
}


