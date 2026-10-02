#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>


static inline Rectangle GetPlayerRect(Player p) {
    Rectangle rect = { p.pos.x, p.pos.y, p.size.x, p.size.y };
    return rect;
}

static inline Rectangle GetBugRect(Bug b) {
    Rectangle rect = { b.pos.x, b.pos.y, 6, 6 };    
    return rect;
}

void clearTheScene();
void SpawnArchive(Vector2 pos, char text[256]);
void SpawnUp_temi(Vector2 pos, int type, char desc[256]);
void SpawnParticle(Vector2 pos, float lifetime, Color clr1, Color clr2);
void SpawnBug(Vector2 pos, int pickable);
void SpawnRuptutre(Vector2 pos);
void SpawnVirus(Vector2 pos, int type);
void SpawnBullet(Vector2 pos, Vector2 dist, int type);
void SpawnBackdoor(Vector2 pos, char target[32]);


#endif
