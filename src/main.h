#include <os.h>
#include <libndls.h>
#include <stdlib.h>
#include <SDL/SDL_config.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include <math.h>

#define K_ESC isKeyPressed(KEY_NSPIRE_ESC)
#define K_ENTER isKeyPressed(KEY_NSPIRE_ENTER)
#define K_TAB isKeyPressed(KEY_NSPIRE_TAB)
#define K_A isKeyPressed(KEY_NSPIRE_A)

#define SCORE_X 276
#define SCORE_Y 84
#define BESTSCORE_X 276
#define BESTSCORE_Y 147

#define GRAVITY_VALUE 12
#define SCALE_FACTOR 60
#define FLY_HEIGHT 3

#define ILFAUTBIENETREGENTILPARFOIS 2

typedef struct Image Image;
struct Image 
{
	SDL_Surface *surface;
	int x;
	int y;
};


typedef enum Screen Screen;
enum Screen
{
	MENU, GET_READY, PLAYING, GAMEOVER, SCORE
};


void drawImage(SDL_Surface*, Image);
int getEvt();
int rand_a_b(int, int);
void decalerTuyaux(Image **);
int getColision(Image, Image **);
void initialiserFichier();
void setHighscore(const unsigned int);
unsigned int getHighscore();