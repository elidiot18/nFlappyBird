/*
 * nFlappyBird v1.0
 *
 * Copyright (C) 2014 pierrotdu18
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include <nspireio.h>
#include <stdio.h>
#include <os.h>
#include <libndls.h>
#include <stdlib.h>
#include <SDL/SDL_config.h>
#include <SDL/SDL.h>
#include <time.h>
#include <math.h>
#include "main.h"
#include "nSDL_CustomFonts.h"
#include "fonts.h"
#include "images.h"

#define TUYAUX_ESPACEMENT_H 80
#define TUYAUX_ESPACEMENT_W 115
	
int main()
{	
	/* --- Initialisation de la SDL --- */
	SDL_Surface *screen;
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(320, 240, has_colors ? 16 : 8, SDL_SWSURFACE);
	/* -------------------------------- */
 
	/* ------ Définition des variables ------ */
	
	/* --------- Images ---------*/
	SDL_Surface* temp = NULL;
	temp = nSDL_LoadImage(image_backwithoutbluetop);
	Image background = {SDL_DisplayFormat(temp), 0, 114};
	SDL_FreeSurface(temp);
	
	SDL_Surface* bgbottom = SDL_CreateRGBSurface (0, 320, 114, has_colors ? 16 : 8, 0, 0, 0, 0);
	SDL_FillRect(bgbottom, NULL, SDL_MapRGB(bgbottom->format, 113,197,207));
	SDL_Rect bgpos;
	bgpos.x = 0; bgpos.y = 0;
	
	temp = NULL;
	temp = nSDL_LoadImage(image_sprite);
	SDL_SetColorKey(temp,SDL_SRCCOLORKEY,SDL_MapRGB(temp->format,0,0,255));
	Image sprite = {SDL_DisplayFormatAlpha(temp), 30, SCREEN_HEIGHT/2 - 10};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_terre);
	Image terre = {SDL_DisplayFormat(temp), 0, SCREEN_HEIGHT - 33};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_tuyau_haut);
	SDL_SetColorKey(temp,SDL_SRCCOLORKEY,SDL_MapRGB(temp->format,0,0,255));
	Image tuyau_haut = {SDL_DisplayFormatAlpha(temp), 250 + SCREEN_WIDTH, -100};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_tuyau_bas);
	SDL_SetColorKey(temp,SDL_SRCCOLORKEY,SDL_MapRGB(temp->format,0,0,255));
	Image tuyau_bas = {SDL_DisplayFormatAlpha(temp), 250 + SCREEN_WIDTH, -100 + 150 + TUYAUX_ESPACEMENT_H};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_tab);
	SDL_SetColorKey(temp,SDL_SRCCOLORKEY,SDL_MapRGB(temp->format,82,189,205));
	Image pressTab = {SDL_DisplayFormatAlpha(temp), SCREEN_WIDTH/2 - 60, 70};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_getready);
	SDL_SetColorKey(temp,SDL_SRCCOLORKEY,SDL_MapRGB(temp->format,255,0,0));
	Image getready = {SDL_DisplayFormatAlpha(temp), SCREEN_WIDTH/2 - 53, 70};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_gameover);
	SDL_SetColorKey(temp,SDL_SRCCOLORKEY,SDL_MapRGB(temp->format,0,254,0));
	Image gameover = {SDL_DisplayFormatAlpha(temp), SCREEN_WIDTH/2 - 110, -48};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_panneau);
	SDL_SetColorKey(temp,SDL_SRCCOLORKEY,SDL_MapRGB(temp->format,255,255,255));
	Image panneau = {SDL_DisplayFormatAlpha(temp), SCREEN_WIDTH/2 - 288/2, SCREEN_HEIGHT/2 - 173/2};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_mb);
	Image medailleDeBronze = {SDL_DisplayFormat(temp), 42, 100};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_ma);
	Image medailleDArgent = {SDL_DisplayFormat(temp), 42, 100};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_mo);
	Image medailleDOr = {SDL_DisplayFormat(temp), 42, 100};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_mp);
	Image medailleDePlatine = {SDL_DisplayFormat(temp), 42, 100};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_sm);
	Image sansMedaille = {SDL_DisplayFormat(temp), 42, 100};
	SDL_FreeSurface(temp);
	
	temp = NULL;
	temp = nSDL_LoadImage(image_new);
	Image new = {SDL_DisplayFormat(temp), 190, 125};
	SDL_FreeSurface(temp);
	
	
	/* ------------------------- */
	
	srand(time(NULL)); //Initialisation du système de génération de nombres aléatoires
	
	int end = 0; // 0 si le jeu est en cours, 1 si on appuie sur ESC
	
	Screen currentScreen = MENU; //Screen manager
	
	/* ---- Polices ---- */
	nSDL_Font *scoreFont = nSDL_LoadCustomFont(fontChars, 0, 0, 0);
	nSDL_Font *scoreFont_mini = nSDL_LoadCustomFont(fontChars_mini, 0, 0, 0);
	nSDL_Font *aboutFont = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 29, 43, 61);
	/* ----------------- */
	
	/* Initialisation du fichier des scores */
	FILE* fp = NULL;
	fp = fopen("nFlappyBird_scores.dat","r");
	if (fp == NULL)
		initialiserFichier();
	else
		fclose(fp);
	/* ------------------------------------ */
	
	unsigned int i; //Itérations boucles for
	
	/* Initialisation des tuyaux */
	Image ** tuyaux;
	tuyaux = malloc(2 * sizeof(Image*));
	for (i = 0 ; i < 2 ; i++) {
		tuyaux[i] = malloc(5 * sizeof(Image));
	}
	for (i = 0 ; i < 5 ; i++) {
		tuyaux[0][i] = tuyau_haut;
		tuyaux[1][i] = tuyau_bas;
	}
	for (i = 1 ; i < 5 ; i++) {
		tuyaux[0][i].x = tuyaux[0][i-1].x + TUYAUX_ESPACEMENT_W;
		tuyaux[1][i].x = tuyaux[1][i-1].x + TUYAUX_ESPACEMENT_W;
		tuyaux[0][i].y = rand_a_b(20,187 - TUYAUX_ESPACEMENT_H) - 150;
		tuyaux[1][i].y = tuyaux[0][i].y + 150 + TUYAUX_ESPACEMENT_H;
	}
	/* ------------------------------------ */
	int velocite = 0; //Vélocité de l'oiseau
	unsigned int score = 0; //Est-ce besoin d'expliquer?
	char score_txt[5] = {"0"}; //Pour des soucis de gestion du score.....
	char bestScore_txt[5] = {"0"};//.....................................
	unsigned char bestScore = 0;
	Image *goodOne = 0; //Pointe sur la médaille à afficher
	int spritey; // Merci à Hayleia pour sa magnifique gestion du flap de l'oiseau :P
	
	/* -------------------------------------- */

	
	while (!end) {
		if (K_ESC) {
			end = 1;
		}
		switch (currentScreen) {
			case MENU:
					if (K_A) {
						show_msgbox("About","nFlappyBird v1.0\nCopyright (C) 2014 pierrotdu18\n\nThis program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License version 2\n\nContact : pierre.elis@hotmail.fr");
					}
					if (K_TAB) {
						currentScreen = GET_READY;
						break;
					}
					SDL_BlitSurface(bgbottom, NULL, screen, &bgpos);
					drawImage(screen, background);
					drawImage(screen, sprite);
					for (i = 0 ; i < 5 ; i++) {
						drawImage(screen, tuyaux[0][i]);
						drawImage(screen, tuyaux[1][i]);
					}
					drawImage(screen, terre);
					drawImage(screen, pressTab);
					terre.x-=2;
					if (terre.x == -26) {
						terre.x = 0;
					}
					nSDL_DrawStringCF(screen, aboutFont, 20, 20, "[a] : About...");
					SDL_Flip(screen);
					//SDL_Delay(10);
				break;
			case GET_READY:
					if (K_ENTER) {
						sprite.y=sprite.y*SCALE_FACTOR;
						currentScreen = PLAYING;
						break;
					}
					SDL_BlitSurface(bgbottom, NULL, screen, &bgpos);
					drawImage(screen, background);
					drawImage(screen, sprite);
					for (i = 0 ; i < 5 ; i++) {
						drawImage(screen, tuyaux[0][i]);
						drawImage(screen, tuyaux[1][i]);
					}
					drawImage(screen, terre);
					drawImage(screen, getready);
					terre.x-=2;
					if (terre.x == -26) {
						terre.x = 0;
					}
					SDL_Flip(screen);
					//SDL_Delay(10);
				break;
			case PLAYING:
					if (K_TAB && sprite.y>0) {
						velocite = -FLY_HEIGHT * SCALE_FACTOR;
					}
					velocite += GRAVITY_VALUE;
					//sprite.y += (velocite/SCALE_FACTOR!=0)?(velocite/SCALE_FACTOR):((velocite<0)?-1:1);
					sprite.y+=velocite;
					spritey=sprite.y;
					sprite.y=sprite.y/SCALE_FACTOR;
					SDL_BlitSurface(bgbottom, NULL, screen, &bgpos);
					drawImage(screen, background);
					drawImage(screen, sprite);
					for (i = 0 ; i < 5 ; i++) {
						drawImage(screen, tuyaux[0][i]);
						drawImage(screen, tuyaux[1][i]);
					}
					drawImage(screen, terre);
					for (i = 0 ; i < 5 ; i++) {
						tuyaux[0][i].x-=2;
						tuyaux[1][i].x-=2;
					}
					if (tuyaux[0][0].x < -32) {
						decalerTuyaux(tuyaux);
					}
					terre.x-=2;
					if (terre.x == -26) {
						terre.x = 0;
					}
					if (tuyaux[0][0].x == sprite.x) {	
						score++;
						sprintf(score_txt, "%d", score);
					}
					if (getColision(sprite, tuyaux)) {
						velocite = 0;
						currentScreen = GAMEOVER;
						break;
					}
					nSDL_DrawStringCF(screen, scoreFont, SCREEN_WIDTH/2 - nSDL_GetStringWidth(scoreFont, score_txt)/2, 50, score_txt);
					SDL_Flip(screen);
					sprite.y=spritey;
					//SDL_Delay(10);
				break;
			case GAMEOVER:
					velocite += GRAVITY_VALUE;
					if (sprite.y < SCREEN_HEIGHT - 33 - sprite.surface->h) {
						sprite.y += (velocite/SCALE_FACTOR!=0)?(velocite/SCALE_FACTOR):((velocite<0)?-1:1);
					}
					if (gameover.y < (SCREEN_HEIGHT-33)/2 - 24) {
						gameover.y += 2;
					}
					if (!(sprite.y < SCREEN_HEIGHT - 33 - sprite.surface->h) && !(gameover.y < (SCREEN_HEIGHT-33)/2 - 24)) {	
						SDL_Delay(500);
						currentScreen = SCORE;
						if (score >=10) {
							goodOne = &medailleDeBronze;
						}else if (score >= 20) {
							goodOne = &medailleDArgent;
						}else if (score >= 30) {
							goodOne = &medailleDOr;
						}else if (score >= 40) {
							goodOne = &medailleDePlatine;
						}else{
							goodOne = &sansMedaille;
						}
						if (score > getHighscore()) {	
							bestScore = 1;
							setHighscore(score);
						}
					}
					SDL_BlitSurface(bgbottom, NULL, screen, &bgpos);
					drawImage(screen, background);
					for (i = 0 ; i < 5 ; i++) {
						drawImage(screen, tuyaux[0][i]);
						drawImage(screen, tuyaux[1][i]);
					}
					drawImage(screen, terre);
					drawImage(screen, gameover);
					drawImage(screen, sprite);
					SDL_Flip(screen);
					//SDL_Delay(10);			
				break;
			case SCORE:
					sprintf(bestScore_txt, "%d", getHighscore());
					SDL_BlitSurface(bgbottom, NULL, screen, &bgpos);
					drawImage(screen, background);
					for (i = 0 ; i < 5 ; i++) {
						drawImage(screen, tuyaux[0][i]);
						drawImage(screen, tuyaux[1][i]);
					}
					drawImage(screen, terre);
					drawImage(screen, sprite);
					drawImage(screen, panneau);
					drawImage(screen, *goodOne);
					if (bestScore) {
						drawImage(screen, new);
					}
					
					nSDL_DrawStringCF(screen, scoreFont_mini, SCORE_X - nSDL_GetStringWidth(scoreFont_mini, score_txt), SCORE_Y, score_txt);
					nSDL_DrawStringCF(screen, scoreFont_mini, BESTSCORE_X - nSDL_GetStringWidth(scoreFont_mini, bestScore_txt), BESTSCORE_Y, bestScore_txt);
					
					if (K_ENTER) {
						for (i = 0 ; i < 5 ; i++) {
							tuyaux[0][i] = tuyau_haut;
							tuyaux[1][i] = tuyau_bas;
						}
						for (i = 1 ; i < 5 ; i++) {
							tuyaux[0][i].x = tuyaux[0][i-1].x + TUYAUX_ESPACEMENT_W;
							tuyaux[1][i].x = tuyaux[1][i-1].x + TUYAUX_ESPACEMENT_W;
							tuyaux[0][i].y = rand_a_b(20,187 - TUYAUX_ESPACEMENT_H) - 150;
							tuyaux[1][i].y = tuyaux[0][i].y + 150 + TUYAUX_ESPACEMENT_H;
						}
						sprite.y = SCREEN_HEIGHT/2 - sprite.surface->h/2;
						velocite = 0;
						score = 0;
						bestScore = 0;
						goodOne = NULL;
						sprintf(score_txt, "%d", score);
						gameover.y = -48;
						currentScreen = MENU;
					}
					SDL_Flip(screen);
					break;
			default:
				break;
			
		}
	}


	/* ---- Fin du programme ----*/
	
	for (i = 0 ; i < 2 ; i++) {
		free(tuyaux[i]);
	}
	free(tuyaux);
	
	nSDL_FreeFont(scoreFont);
	nSDL_FreeFont(scoreFont_mini);
	
	SDL_FreeSurface(background.surface);
	SDL_FreeSurface(bgbottom);
	SDL_FreeSurface(terre.surface);
	SDL_FreeSurface(sprite.surface);
	SDL_FreeSurface(tuyau_haut.surface);
	SDL_FreeSurface(tuyau_bas.surface);
	SDL_FreeSurface(pressTab.surface);
	SDL_FreeSurface(getready.surface);
	SDL_FreeSurface(gameover.surface);
	SDL_FreeSurface(medailleDeBronze.surface);
	SDL_FreeSurface(medailleDArgent.surface);
	SDL_FreeSurface(medailleDOr.surface);
	SDL_FreeSurface(medailleDePlatine.surface);
	SDL_FreeSurface(sansMedaille.surface);
	SDL_FreeSurface(new.surface);

	/* --- Fin de SDL --- */
	SDL_Quit();
	/* ----------------- */
 
	return 0;
}


void drawImage(SDL_Surface *screen, Image image)
{
	SDL_Rect position;
	position.x = image.x;
	position.y = image.y;
	SDL_BlitSurface(image.surface, NULL, screen, &position);
}

int rand_a_b(int a, int b)
{
    return rand()%(b-a) +a;
}

void decalerTuyaux(Image ** tuyaux)
{
	unsigned int i;
	for (i = 0 ; i < 4 ; i++) 
	{
		tuyaux[0][i] = tuyaux[0][i + 1];
		tuyaux[1][i] = tuyaux[1][i + 1];
	}
	
	tuyaux[0][4].x = tuyaux[0][3].x + 120;
	tuyaux[0][4].y = rand_a_b(20,187 - TUYAUX_ESPACEMENT_H) - 150;
	
	tuyaux[1][4].x = tuyaux[0][4].x;
	tuyaux[1][4].y = tuyaux[0][4].y + 150 + TUYAUX_ESPACEMENT_H;
}

int getColision(Image sprite, Image ** tuyaux)
{
	if (sprite.y >= (SCREEN_HEIGHT - 33 - sprite.surface->h))
	{
		return 1;
	}
	if (sprite.x + sprite.surface->w >= tuyaux[0][0].x + ILFAUTBIENETREGENTILPARFOIS && sprite.x <= tuyaux[0][0].x + tuyaux[0][0].surface->w - ILFAUTBIENETREGENTILPARFOIS)
	{
		if (!(sprite.y >= tuyaux[0][0].y + 150 - ILFAUTBIENETREGENTILPARFOIS && sprite.y + sprite.surface->h <= tuyaux[1][0].y + ILFAUTBIENETREGENTILPARFOIS))
		{
			return 1;
		}
	}
	
	return 0;
}

void initialiserFichier()
{
	FILE* fp = NULL;
	fp = fopen("nFlappyBird_scores.dat","bw+");
	if (fp != NULL)
	{
		fputc('0', fp);
		fclose(fp);
	}
	else
	{
		show_msgbox("Erreur","Erreur de fichier");
	}
}

void setHighscore(const unsigned int score)
{
	FILE* fp = NULL;
	fp = fopen("nFlappyBird_scores.dat","bw+");
	if (fp != NULL)
	{
		fprintf(fp, "%d", score);
		fclose(fp);
	}
	else
	{
		show_msgbox("Erreur","Erreur de fichier");
	}
}

unsigned int getHighscore()
{
	char txt[5];
	unsigned int score;
	FILE* fp = NULL;
	fp = fopen("nFlappyBird_scores.dat","br");
	if (fp != NULL)
	{
		fgets(txt, 5, fp);
		sscanf(txt, "%u", &score);
		fclose(fp);
		return score;
	}
	else
	{
		show_msgbox("Erreur","Erreur de fichier");
		return 1;
	}
}

