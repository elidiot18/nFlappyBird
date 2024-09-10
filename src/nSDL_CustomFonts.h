#ifndef NSDL_CUSTOM_FONTS_H
#define NSDL_CUSTOM_FONTS_H

#include <SDL\SDL.h>



nSDL_Font *nSDL_LoadCustomFont(unsigned char *fontChars, Uint8 r, Uint8 g, Uint8 b);  // � utiliser pour charger n'importe quelle police personnalis�e
int nSDL_DrawStringCF(SDL_Surface *surface, nSDL_Font *font, int x, int y, const char *format, ...);  // � utiliser si la hauteur de votre police est sup�rieure � 8
int nSDL_GetStringHeightCF(nSDL_Font *font, const char *s);  // � utiliser si la hauteur de votre police est diff�rente de 8



#endif