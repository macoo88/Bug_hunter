#ifndef RENDERER_H
#define RENDERER_H


#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "types.h"


void DrawHud(State *state);
void DrawPlayer(State *state);
void DrawArchives();
void DrawParticles(Particle *par, int pass);
void DrawBug(Bug bug, int pass);
void DrawUpdateTermis();
void DrawDebuger(Vector2 pos);
void DrawBadObject(int *sprite_template, int template_res, Vector2 pos, int hp);
void DrawBugs();
void DrawBullets();
void DrawTheBackdoor(Vector2 pos, char target[32]);
void DrawHelpArrow(Vector2 pos, Vector2 dir, Color clr);

#endif
