#include "common.h"

/* Functions which are defined in a seperate compilation unit but used in this file are declared here */
extern void blit(SDL_Texture* texture, int x, int y);
extern int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
extern SDL_Texture* loadTexture(char* filename);

extern App app;
extern Stage stage;
