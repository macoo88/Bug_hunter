#include "raylib.h"
#include "raymath.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void clearTheScene()
{
    State *state = GetState();
    if(state->bugs != NULL)   { free(state->bugs);   }
    if(state->bullets != NULL){ free(state->bullets);}
    if(state->rups != NULL)   { free(state->rups);   }
    if(state->virs != NULL)   { free(state->virs);   }
    if(state->backdoors != NULL)  { free(state->backdoors); }
    if(state->archives != NULL)   { free(state->archives);  }
    if(state->particles != NULL)  { free(state->particles); }
    if(state->up_termis != NULL)  { free(state->up_termis); }


    state->debuger.active = 0;
    state->debuger.timer = 1.0f;
    
    state->backdoorCount = 0;
    state->backdoorMemCount = 4;
    state->backdoors_active = 0;
    state->backdoors = malloc(sizeof(Backdoor) * state->backdoorMemCount);
    memset(state->backdoors, 0, sizeof(Backdoor) * state->backdoorMemCount);
    
    
    state->up_Count = 0;
    state->up_MemCount = 4;
    state->up_termis = malloc(sizeof(Update_teminal) * state->up_MemCount);
    memset(state->up_termis, 0, sizeof(Update_teminal) * state->up_MemCount);

    
    state->archiveCount = 0;
    state->archiveMemCount = 4;
    state->archives = malloc(sizeof(ArchiveText) * state->archiveMemCount);
    memset(state->archives, 0, sizeof(ArchiveText) * state->archiveMemCount);

    state->bulletsCount = 0;
    state->bulletsMemCount = 10;
    state->bullets = malloc(sizeof(Bullet) * state->bulletsMemCount);
    memset(state->bullets, 0, sizeof(Bullet) * state->bulletsMemCount);

    state->bugsCount = 0;
    state->bugsMemCount = 25;
    state->bugs = malloc(sizeof(Bug) * state->bugsMemCount);

    state->particlesCount = 0;
    state->particlesMemCount = 100;
    state->particles = malloc(sizeof(Particle) * state->particlesMemCount);

    state->rupCount = 0;
    state->rupMemCount = 8;
    state->rups_active = 0;
    state->rups = malloc(sizeof(Rupture) * state->rupMemCount);
    memset(state->rups, 0, sizeof(Rupture) * state->rupMemCount);
    
    state->virCount = 0;
    state->virMemCount = 8;
    state->virs = malloc(sizeof(Virus) * state->virMemCount);
    memset(state->virs, 0, sizeof(Virus) * state->virMemCount);
}


void SpawnArchive(Vector2 pos, char text[256])
{
    State *state = GetState();
    int index = -1;
    for (int i = 0; i < state->archiveCount; i++) {
        if (state->archives[i].active == 0) {
            index = i;
            break;
        }
    }
    if(index == -1){
        if(state->archiveCount + 1 >= state->archiveMemCount)
        {
            state->archiveMemCount += 8;
            state->archives = realloc(state->archives, sizeof(ArchiveText) * state->archiveMemCount);
        }
        index = state->archiveCount;
        state->archiveCount+=1;
    }

    state->archives[index].pos.x = pos.x;
    state->archives[index].pos.y = pos.y;
    char buf[256] = {0};
    int write_index = 0;
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        if (text[i] == '\\' && i + 1 < len && text[i + 1] == 'n') {
            buf[write_index++] = '\n';
            i++;
        } else {
            buf[write_index++] = text[i];
        }
    }
    buf[write_index] = '\0';

    strcpy(state->archives[index].text, buf);
    state->archives[index].active = 1;
}

void SpawnUp_temi(Vector2 pos, int type, char desc[256])
{
    State *state = GetState();
    int index = -1;
    for (int i = 0; i < state->up_Count; i++) {
        if (state->up_termis[i].active == 0) {
            index = i;
            break;
        }
    }
    if(index == -1){
        if(state->up_Count + 1 >= state->up_MemCount)
        {
            state->up_MemCount += 8;
            state->up_termis = realloc(state->up_termis, sizeof(Bug) * state->up_MemCount);
        }
        index = state->up_Count;
        state->up_Count+=1;
    }

    state->up_termis[index].pos.x = pos.x;
    state->up_termis[index].pos.y = pos.y;
    state->up_termis[index].active = 1;
    state->up_termis[index].type = type;
    char buf[256] = {0};
    int write_index = 0;
    int len = strlen(desc);
    for (int i = 0; i < len; i++) {
        if (desc[i] == '\\' && i + 1 < len && desc[i + 1] == 'n') {
            buf[write_index++] = '\n';
            i++;
        } else {
            buf[write_index++] = desc[i];
        }
    }
    buf[write_index] = '\0';

    strcpy(state->up_termis[index].description, buf);
}

void SpawnParticle(Vector2 pos, float lifetime, Color clr1, Color clr2)
{
    State *state = GetState();
    int index = -1;
    for (int i = 0; i < state->particlesCount; i++) {
        if (state->particles[i].active == 0) {
            index = i;
            break;
        }
    }
    if(index == -1){
        if(state->particlesCount + 1 >= state->particlesMemCount)
        {
            state->particlesMemCount += 8;
            state->particles = realloc(state->particles, sizeof(Bug) * state->particlesMemCount);
        }
        index = state->particlesCount;
        state->particlesCount+=1;
    }

    state->particles[index].pos.x = pos.x;
    state->particles[index].pos.y = pos.y;
    state->particles[index].color1 = clr1;
    state->particles[index].color2 = clr2;
    state->particles[index].active = 1;
    state->particles[index].lifetime = lifetime;
}

void SpawnBug(Vector2 pos, int pickable)
{
    State *state = GetState();
    int index = -1;
    for (int i = 0; i < state->bugsCount; i++) {
        if (state->bugs[i].active == 0) {
            index = i;
            break;
        }
    }
    if(index == -1){
        if(state->bugsCount + 1 >= state->bugsMemCount)
        {
            state->bugsMemCount += 8;
            state->bugs = realloc(state->bugs, sizeof(Bug) * state->bugsMemCount);
        }
        index = state->bugsCount;
        state->bugsCount+=1;
    }

    state->bugs[index].pos.x = pos.x;
    state->bugs[index].pos.y = pos.y;
    state->bugs[index].pickable = pickable;
    state->bugs[index].active = 1;
}


void SpawnRuptutre(Vector2 pos)
{
    State *state = GetState();
    int index = -1;
    for (int i = 0; i < state->rupCount; i++) {
        if (state->rups[i].active == 0) {
            index = i;
            break;
        }
    }
    if(index == -1){
        if(state->rupCount + 1 >= state->rupMemCount)
        {
            state->rupMemCount += 8;
            state->rups = realloc(state->rups, sizeof(Rupture) * state->rupMemCount);
        }
        index = state->rupCount;
        state->rupCount+=1;
    }

    state->rups[index].pos.x = pos.x;
    state->rups[index].pos.y = pos.y;
    state->rups[index].active = 1;
    state->rups[index].actionTimer = 2.0f;
    state->rups[index].rec = (Rectangle){state->rups[index].pos.x+5, state->rups[index].pos.y+5, 100, 100};
    state->rups[index].hp = 10;
    state->rups_active++;
    //memset(state->rups[0].bugs, 0, sizeof(state->rups[0].bugs));
}
void SpawnVirus(Vector2 pos, int type)
{
    //type 0 = virus; type 1 = corupt_struct;
    State *state = GetState();
    int index = -1;
    for (int i = 0; i < state->virCount; i++) {
        if (state->virs[i].active == 0) {
            index = i;
            break;
        }
    }
    if(index == -1){
        if(state->virCount + 1 >= state->virMemCount)
        {
            state->virMemCount += 8;
            state->virs = realloc(state->virs, sizeof(Virus) * state->virMemCount);
        }
        index = state->virCount;
        state->virCount+=1;
    }

    state->virs[index].pos.x = pos.x;
    state->virs[index].pos.y = pos.y;
    state->virs[index].active = 1;
    state->virs[index].type = type;
    switch (type){
        case 0:
            state->virs[index].hp = 2;
            break;
        case 1:
            state->virs[index].hp = 4;
            break;
        case 2:
            state->virs[index].hp = 3;
            break;
    }
    
    state->virs[index].speed = (type == 0) ? 190 : 125;
    //state->virs[index].rec = (Rectangle){state->virs[index].pos.x, state->virs[index].pos.y, 50, 50};
}

void SpawnBackdoor(Vector2 pos, char target[32])
{
    //type 0 = virus; type 1 = corupt_struct;
    State *state = GetState();
    int index = -1;
    for (int i = 0; i < state->backdoorCount; i++) {
        if (state->backdoors[i].active == 0) {
            index = i;
            break;
        }
    }
    if(index == -1){
        if(state->backdoorCount + 1 >= state->backdoorMemCount)
        {
            state->backdoorMemCount += 8;
            state->backdoors = realloc(state->backdoors, sizeof(Bug) * state->backdoorMemCount);
        }
        index = state->backdoorCount;
        state->backdoorCount+=1;
    }

    state->backdoors[index].pos.x = pos.x;
    state->backdoors[index].pos.y = pos.y;
    state->backdoors[index].active = 1;
    strncpy(state->backdoors[index].targetLevel, target, strlen(target));
    state->backdoors[index].targetLevel[strlen(target)] = '\0';
    state->backdoors_active++;
    //fprintf(stderr, "Backdoor spawned with target level : %s\n\n", state->backdoors[index].targetLevel);
}


void SpawnBullet(Vector2 pos, Vector2 dist, int type)
{
    State *state = GetState();
    int index = -1;
    for (int i = 0; i < state->bulletsCount; i++) {
        if (state->bullets[i].active == 0) {
            index = i;
            break;
        }
    }
    if(index == -1){
        if(state->bulletsCount + 1 >= state->bulletsMemCount)
        {
            state->bulletsMemCount += 8;
            state->bullets = realloc(state->bullets, sizeof(Bullet) * state->bulletsMemCount);
        }
        index = state->bulletsCount;
        state->bulletsCount+=1;
    }

    //Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), state->cam);
    Vector2 vel = Vector2Subtract(dist, pos);
    vel = Vector2Normalize(vel);

    
    vel = Vector2Scale(vel, 600);

    state->bullets[index].pos.x = pos.x;
    state->bullets[index].pos.y = pos.y;
    state->bullets[index].vel = vel;
    state->bullets[index].active = 1;
    state->bullets[index].livetime = 6;
    state->bullets[index].owner = type;

    //fprintf(stderr, "Bullets vel = %d %d\n", (int)state->bullets[index].vel.x, (int)state->bullets[index].vel.y);
}
