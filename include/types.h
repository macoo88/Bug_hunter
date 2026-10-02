#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"

typedef enum {
    SCREEN_MENU,
    SCREEN_GAMEPLAY,
    SCREEN_GAMEOVER,
} Scenes;

typedef struct{
    Vector2 pos;
    char text[256];
    int active;
}ArchiveText;

typedef struct {
    Vector2 pos;
    int health;
    int active;
    int pickable;
} Bug;

typedef struct {
    Vector2 pos;
    int active;
    Rectangle rec;
    float actionTimer;
    int hp;
} Rupture;

typedef struct {
    Vector2 pos;
    int type;//0 virus; 1 corupt_struct; 2 shooting_vir
    int active;
    int hp;
    float speed;
    float damage_timer;
    float action_timer;
    int effect;
} Virus;

typedef struct {
    Vector2 pos;
    Vector2 vel;
    Rectangle rec;
    int active;
    float livetime;
    int owner;//0 = player; 1 = ennemy;
}Bullet;

typedef struct {
    Vector2 pos;
    Vector2 size;
    Vector2 input;
    float speed;
    
    Bug bugs[10];
    int corruption_level;
    float bug_update_timer;
    int corruption_immunity;
    
    int canShoot;
    
    float dashTimer;
    float dashCooldown;
    Vector2 dashDir;

} Player;

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float lifetime;
    Color color1;
    Color color2;
    int active;
} Particle;

typedef struct{
    Vector2 pos;
    int active;
    char targetLevel[32];
} Backdoor;

typedef struct{
    Vector2 pos;
    int active;
    float timer;
} Debuger;

typedef struct{
    Vector2 pos;
    char description[256];
    int type;
    int active;
    float timer;
} Update_teminal;//240 095-0II.

typedef struct{
    Scenes scene;
    Backdoor *backdoors;
    int backdoorCount;
    int backdoorMemCount;
    int backdoors_active;

    Debuger debuger;

    Update_teminal *up_termis;
    int up_Count;
    int up_MemCount;

    Player player;
    int player_id;
    Camera2D cam;

    Bug *bugs;
    int bugsCount;
    int bugsMemCount;
    
    Particle *particles;
    int particlesCount;
    int particlesMemCount;

    Rupture *rups;
    int rupCount;
    int rupMemCount;
    int rups_active;

    Virus *virs;
    int virCount;
    int virMemCount;

    Bullet *bullets;
    int bulletsCount;
    int bulletsMemCount;

    ArchiveText *archives;
    int archiveCount;
    int archiveMemCount;

    Vector2 nearest_door;
    Vector2 nearest_rup;
    int score;
    int changeScene;
    char newScene[32];
    char currenteFileName[32];

    float restart_timer;

} State;

static int rupture_template[100] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                    0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
                                    0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
                                    0, 0, 0, 1, 0, 0, 0, 1, 1, 0,
                                    0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                                    0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 
                                    0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
                                    0, 0, 1, 1, 0, 1, 0, 0, 0, 0,
                                    0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
                                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static int debuger_template[100] = {0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
                                    0, 1, 0, 1, 0, 0, 1, 0, 1, 0,
                                    0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                                    0, 1, 0, 0, 1, 1, 0, 0, 1, 0,
                                    1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
                                    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 
                                    0, 1, 0, 0, 1, 1, 0, 0, 1, 0,
                                    0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                                    0, 1, 0, 1, 0, 0, 1, 0, 1, 0,
                                    0, 0, 0, 0, 1, 1, 0, 0, 0, 0};

static int virus_template[25] =   {1, 0, 0, 0, 1,
                                   0, 1, 0, 1, 0,
                                   0, 1, 1, 0, 0,
                                   0, 1, 0, 1, 0,
                                   1, 0, 0, 1, 1 };

static int corupt_struct_template[25] =   {0, 0, 1, 0, 1,
                                           1, 1, 0, 1, 0,
                                           1, 0, 1, 0, 1,
                                           0, 1, 1, 1, 0,
                                           0, 0, 1, 0, 0 };

static int shooter_vir_template[25] =   {1, 1, 0, 1, 1,
                                         1, 0, 0, 0, 1,
                                         0, 0, 1, 0, 0,
                                         1, 0, 0, 0, 1,
                                         1, 1, 0, 0, 1 };

State* GetState(void);

#endif
