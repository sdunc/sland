/* common.h contains all shared header files "common code" */
#include "common.h"

/* extern: compiler knows that this exists but no memory is supplied; 
these are the functions which are called inside main.c */
extern void cleanup(void);
extern void doInput(void);
extern void initSDL(void);
extern void initStage(void);
extern void prepareScene(void);
extern void presentScene(void);

App app;
Stage stage;