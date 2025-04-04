﻿#ifndef GAME
#define GAME

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "glob.h"
#include "grob.h"
#include "keyboardInput.h"
#include "drawing_functions.h"
#include "soundManager.h"

class Game{
public:
	Mix_Music* song;

	AnimationSet* heroAnimSet;
	AnimationSet* globAnimSet;
	AnimationSet* grobAnimSet;
	AnimationSet* wallAnimSet;

	SDL_Texture* backgroundImage;
	SDL_Texture* splashImage;
	SDL_Texture* overlayImage;

	SDL_Texture* scoreTexture = NULL; // con trỏ tới texture của điểm số

	Hero *hero;
	KeyboardInput heroInput;

	list<Entity*> enemies;
	list<Entity*> walls;

	bool splashShowing;
	float overlayTimer;

	Game();
	~Game();

	void update();
	void draw();

};


#endif