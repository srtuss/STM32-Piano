#ifndef __MAIN_APP_H
#define __MAIN_APP_H

extern void mainApp_init(void);
extern void mainApp_handler(void);
extern void mainApp_sendMessage(const char *str);
extern void mainApp_keyChange(int key, int state);

#endif
