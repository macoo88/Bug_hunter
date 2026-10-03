#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// My headers -----------------
#include "types.h"
#include "utils.h"
#include "renderer.h"
#include "levels.h"

#define MAX_BUGS 100


void UpdateViruses(State *state)
{
    float dt = GetFrameTime();
    for(Rupture *rup = state->rups; rup != state->rups + state->rupCount; rup++)
    {
        if(rup->active == 0){continue;}
        DrawBadObject(rupture_template, 10, rup->pos, rup->hp);
        rup->actionTimer -= dt;
        if(rup->actionTimer < 0)
        {
            SpawnVirus(rup->pos, rand()%3);
            rup->actionTimer = 10.0f;
        }
    }


    for(Virus *vir = state->virs; vir != state->virs + state->virCount; vir++)
    {
        if(vir->active == 0){continue;} 
        vir->action_timer += dt;
        float dist_to_player = Vector2Distance(state->player.pos, vir->pos);
       
       Vector2 vel = Vector2Subtract(state->player.pos, vir->pos);
       vel = Vector2Scale(Vector2Normalize(vel), vir->speed * dt);
    //------------------- TYPE 0 ---------------------- reg
        if(vir->type == 0)
        {
            vir->pos = Vector2Add(vir->pos, vel);
            DrawBadObject(virus_template, 5, vir->pos, vir->hp);
        }
    //------------------- TYPE 1 ---------------------- tank
        if(vir->type == 1)
        {
            if (vir->action_timer > 5.0f && vir->action_timer < 6.4f) 
            {
                if (dist_to_player < 200.0f) {
                    vir->effect = 1; // Aktivujeme stabilný glitch režim
                }
            
                if (vir->action_timer > 5.8f && vir->effect) 
                {
                    vir->pos = Vector2Add(vir->pos, Vector2Scale(vel, 150.0f));
                } 
                else 
                { 
                    vir->pos = Vector2Add(vir->pos, vel); 
                } 
            } 
            else 
            {
                vir->pos = Vector2Add(vir->pos, vel);
                
                if (vir->action_timer >= 6.4f) {
                    vir->action_timer = 0.0f;
                    vir->effect = 0;
                }
            }
            


            DrawBadObject( corupt_struct_template, 5, vir->pos, vir->hp);
        }
    //------------------- TYPE 2 ---------------------- shooter
        else if(vir->type == 2){
            
            if(dist_to_player < 600)
            {
                if(vir->action_timer > 2.2f){
                    SpawnBullet((Vector2){vir->pos.x + 25, vir->pos.y+25}, state->player.pos, 1);
                    SpawnParticle((Vector2){vir->pos.x + 25, vir->pos.y+25}, 0.48f, PURPLE, LIGHTGRAY);
                    SpawnParticle(Vector2Add((Vector2){vir->pos.x + 25, vir->pos.y+25}, Vector2Scale( Vector2Normalize(Vector2Subtract(state->player.pos, (Vector2){vir->pos.x + 25, vir->pos.y+25})), 15)) , 0.48f, PURPLE, LIGHTGRAY);
                    vir->action_timer = 0;
                }
            }
            else
            {
                vir->pos = Vector2Add(vir->pos, vel);
            }
            DrawBadObject(shooter_vir_template, 5, vir->pos, vir->hp);
        }


        //Chekc blee collisions for damage to player
        if(!vir->active) { continue; }
        vir->damage_timer += dt;
        if(CheckCollisionRecs(GetPlayerRect(state->player), (Rectangle){vir->pos.x, vir->pos.y, 50, 50}))
        {
            if(vir->damage_timer > 1){
                state->player.corruption_level = (state->player.corruption_level == 9) ? 9 : state->player.corruption_level+1;
                vir->damage_timer = 0.0f;
            }
        }
    }
}


void UpdatePhysics(State *state) {
    float dt = GetFrameTime();

// PLAYER -------------------------------
    Vector2 oldPos = state->player.pos;
    float dist;
    
    if(state->player.dashTimer > 0)
    {
        state->player.dashTimer -= dt;
        state->player.pos.x += state->player.dashDir.x * 9;
    }
    else{ state->player.pos.x += state->player.input.x * state->player.speed * dt; }
    for(Rupture *rup = state->rups; rup != state->rups + state->rupCount; rup++)
    {
        if(!rup->active) { continue;}
        if(CheckCollisionRecs(GetPlayerRect(state->player), rup->rec))
        {
            state->player.pos.x = oldPos.x;
        }
        dist = abs((int)Vector2Distance(state->player.pos, rup->pos)); 
        state->nearest_rup = rup->pos;
    }

    if(state->player.dashTimer > 0)
    {
        state->player.pos.y += state->player.dashDir.y * 9;
    }
    else { state->player.pos.y += state->player.input.y * state->player.speed * dt; }
     
    
    for(Rupture *rup = state->rups; rup != state->rups + state->rupCount; rup++)
    {
        if(!rup->active) { continue;}
        if(CheckCollisionRecs(GetPlayerRect(state->player), rup->rec))
        {
            state->player.pos.y = oldPos.y;
        }
        //DrawBugs();
        float rup_dist = abs((int)Vector2Distance(state->player.pos, rup->pos));
        if(rup_dist < dist){dist = rup_dist; state->nearest_rup = rup->pos;}
    }
 

//blee blee collisions
    if(state->debuger.active && CheckCollisionRecs(GetPlayerRect(state->player), (Rectangle){state->debuger.pos.x+10, state->debuger.pos.y+10, 90, 90}))
    {   
        state->debuger.timer -= dt;
        if(state->debuger.timer < 0)
        { 
            state->player.corruption_level = (state->player.corruption_level == -1) ? -1 : state->player.corruption_level-1;
            state->debuger.timer = 1.0f;
        }
    }

//backddors
    dist = 1000000;
    for(Backdoor *bd = state->backdoors; bd != state->backdoors + state->backdoorCount; bd++)
    {
        float bd_dist = abs((int)Vector2Distance(state->player.pos, bd->pos));
        if(bd_dist < dist){dist = bd_dist; state->nearest_door = bd->pos;}
        
        if(!CheckCollisionRecs(GetPlayerRect(state->player), (Rectangle){bd->pos.x, bd->pos.y, 30, 40})) { continue; }
        if(strlen(bd->targetLevel) > 0)
        {
            state->changeScene = 1;
            memset(state->newScene, 0, sizeof(state->newScene));
            strncpy(state->newScene, bd->targetLevel, strlen(bd->targetLevel));
        }
    }
//collect bugs that dont exist at the moment
    for (int i = 0; i < state->bugsCount; i++) {
        if (state->bugs[i].active == 1 && state->bugs[i].pickable) {
            if (CheckCollisionRecs(GetPlayerRect(state->player), GetBugRect(state->bugs[i]))) {
                state->bugs[i].active = 0;
                state->score += 1;
            }
        }
    }


// BULLETS -------------------------------
    for(Bullet *bullet = state->bullets; bullet != state->bullets + state->bulletsCount; bullet++)
    {
        if(bullet->active == 0){ continue; }
        if(bullet->livetime <= 0){ bullet->active = 0; continue;;}
        
        bullet->livetime -= dt;
        bullet->pos = Vector2Add(bullet->pos, Vector2Scale(bullet->vel, dt));
        
        if(bullet->owner == 1){ 
            if(CheckCollisionRecs(GetPlayerRect(state->player), (Rectangle){bullet->pos.x, bullet->pos.y, 8, 8}))
            { 
                state->player.corruption_level = (state->player.corruption_level == 9) ? 9 : state->player.corruption_level+1;
                bullet->active = 0; 
            }
            continue;
        }
        

        for(Rupture *rup = state->rups; rup != state->rups + state->rupCount; rup++)
        {
            if(!rup->active) { continue;}
            if( CheckCollisionRecs(rup->rec, (Rectangle){bullet->pos.x, bullet->pos.y, 8, 8}) )
            {
                rup->hp-= 1;
                bullet->active = 0;
                if(rup->hp <= 0){ 
                    state->rups_active--;
                    rup->active = 0;
                    state->score += 13;
                    SpawnBug((Vector2){rup->pos.x+rand()%50 - 25+50, rup->pos.y+rand()%50 - 25+50}, 1);
                    SpawnBug((Vector2){rup->pos.x+rand()%50 - 25+50, rup->pos.y+rand()%50 - 25+50}, 1);
                    SpawnBug((Vector2){rup->pos.x+rand()%50 - 25+50, rup->pos.y+rand()%50 - 25+50}, 1);
                    SpawnBug((Vector2){rup->pos.x+rand()%50 - 25+50, rup->pos.y+rand()%50 - 25+50}, 1);
                }
                continue;
            }
        }
        
        for(Virus *vir = state->virs; vir != state->virs + state->virCount; vir++)
        {
            if(!vir->active) { continue;}
            if( CheckCollisionRecs((Rectangle){vir->pos.x, vir->pos.y, 50, 50}, (Rectangle){bullet->pos.x, bullet->pos.y, 8, 8}) )
            {
                vir->hp -= 1;
                bullet->active = 0;
                if(vir->hp <= 0){ 
                    vir->active = 0;
                    state->score += 2; 
                    SpawnBug((Vector2){vir->pos.x+rand()%50 - 25+50, vir->pos.y+rand()%50 - 25+50}, 1);

                }
                continue;
            }
            
            
        }

    }
}

void HandlePlayerInput(Player* player) {
    if      (IsKeyDown(KEY_D)) player->input.x = 1;
    else if (IsKeyDown(KEY_A)) player->input.x = -1;
    else                       player->input.x = 0;
    
    if      (IsKeyDown(KEY_S)) player->input.y = 1;
    else if (IsKeyDown(KEY_W)) player->input.y = -1;
    else                       player->input.y = 0;
    
    //fix diag speed
    if (player->input.x != 0 && player->input.y != 0) {
        player->input = Vector2Normalize(player->input);
    }
    
    if(player->dashCooldown <= 0 && (IsKeyPressed(KEY_LEFT_SHIFT)))
    { 
        player->dashDir = player->input; 
        player->dashTimer = 0.25f;
        
        player->dashCooldown = 1.2f;
    }else if(player->dashCooldown >0){ player->dashCooldown-=GetFrameTime();}

    if((IsMouseButtonDown(0) || IsKeyDown(KEY_SPACE)) && player->canShoot)
    {
        SpawnBullet((Vector2){ player->pos.x+12.5f, player->pos.y+12.5f }, GetScreenToWorld2D(GetMousePosition(), GetState()->cam), 0);
        player->canShoot = 0;
        //play sound
    }
}

int main(void) {
    InitWindow(800, 450, "File Healer/Bug Hunter");
    SetTargetFPS(60);
    srand(time(NULL));
    
//###################################################-------------------------------------------
// ------------------------- I N I T    S T A R T
//PLAYER
    State* state = GetState(); 
    
    state->player.pos = (Vector2){ 0, 0 };
    state->player.size = (Vector2){ 25, 25 };
    state->player.input = (Vector2){ 0, 0 };
    state->player.speed = 250.0f;
    state->player.canShoot = 1;
    state->player.corruption_level = -1;
    state->player_id = 0;
    state->player.dashTimer = 0;
    state->player.dashCooldown = 0;
    state->player.corruption_immunity = 3;

    state->cam.rotation = 0;
    state->cam.target = (Vector2){ 400, 225 };
    state->cam.offset = (Vector2){ 400, 225 };
    state->cam.zoom = 1.0f;

    state->restart_timer = 5.0f;

    clearTheScene();

    state->score = 0;
    //float spawnTimer = 0.0f;
    //float spawnInterval = 1.0f;
    float fireRate = 0.45;//flatline = 0.1f
    
    int loaded = load_level("hub");
    if(loaded == -1)
    {
        fprintf(stderr, "!!! An error has occured while loading map file !!!\n");
    }
    


//###################################################-------------------------------------------
// ------------------------------ I N I T    S T O P
// ------------------------- M A I N   L O O P    S T A R T

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if(state->changeScene == 1)
        {
            clearTheScene();
            loaded = load_level(state->newScene);
            if(loaded == -1){fprintf(stderr, "Error loading this"); }
            state->changeScene = 0;
        }

        if(state->player.corruption_level >= state->player.corruption_immunity-1)
        {
            state->restart_timer -= dt;
            if(state->restart_timer < 0)
            {
                clearTheScene();
                load_level("hub");
                state->score = (state->score >= 100) ? state->score-100 : 0;
                state->player.corruption_level = -1;
                state->player_id += 1 * rand()%29;
                state->restart_timer = 6.5f;
            }
        }

     //##################
     // --- UPDATE ---
     // ###############
        if(IsWindowResized())
        {
            state->cam.offset = (Vector2){(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
        }


        HandlePlayerInput(&state->player);
        UpdatePhysics(state);
   
        if(state->player.dashTimer > 0)
        {
             SpawnParticle((Vector2){state->player.pos.x + 12.5f, state->player.pos.y + 12.5f}, 0.6f, BLUE, WHITE);
        }

        //camera
        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), state->cam);

        Vector2 dir = Vector2Subtract(mouseWorld, state->player.pos);
        float dist = Vector2Distance(state->player.pos, mouseWorld);
        if(dist > 400){dist = 400;}

        Vector2 normalizedDir = Vector2Normalize(dir);
        Vector2 scaledDir = Vector2Scale(normalizedDir, dist * dt * 10);
        
        Vector2 targetPos = Vector2Add(state->player.pos, scaledDir);
        state->cam.target = targetPos;
   

    // Random Bugs spawning  -- legacy
        //spawnTimer += dt;
        //if (spawnTimer >= spawnInterval) {
        // 
        //    Vector2 new_bugs_pos = {rand() % (800 - 20) + 25, rand() % (450 - 20) + 25};
        //    SpawnBug(new_bugs_pos, 1);
        //    spawnTimer = 0.0f;
        //}
        if(state->player.canShoot == 0)
        {
            fireRate -= dt;
            if(fireRate <= 0){ state->player.canShoot = 1; fireRate = 0.45; }
        }
        

     //#################
     // --- DRAW ---
     //################
        BeginDrawing();
        BeginMode2D(state->cam);
        ClearBackground(BLACK);
//-----------------------------

        //D R A W   B A C K G R O U N D
        int playerX = (int)state->player.pos.x;
        int playerY = (int)state->player.pos.y;
        
        int startX  = (playerX - 1200) / 40 * 40;
        int endX    = (playerX + 1200) / 40 * 40;
        
        int startY  = (playerY - 1000) / 30 * 30;
        int endY    = (playerY + 1000) / 30 * 30;
        
        for (int x = startX; x < endX; x += 40) {
            for (int y = startY; y < endY; y += 30) {
                
                //const char* bit = (abs(x * 13 + y * 37) % 2 == 0) ? "1" : "0";
                const char* bit = (rand()%2) ? "1" : "0";
                DrawText(bit, x, y, 12, ColorAlpha(WHITE, 0.25f));  
            }
        } 
        
        //U P D A T E  +  D R A W
        DrawArchives();
        DrawUpdateTermis();

        UpdateViruses(state);
        
        DrawBullets();
        for(Particle *par = state->particles; par != state->particles + state->particlesCount; par++)
        {
            DrawParticles(par, rand()%2);
            DrawParticles(par, rand()%2);
        }
        for(Backdoor *bd = state->backdoors; bd != state->backdoors + state->backdoorCount; bd++)
        {
            DrawTheBackdoor(bd->pos, bd->targetLevel);
        }
        
        if(state->backdoors_active > 0){ DrawHelpArrow( state->player.pos, state->nearest_door, GREEN); }
        if(state->rups_active > 0){ DrawHelpArrow( state->player.pos, state->nearest_rup, PURPLE ); }
        
        
        if(state->debuger.active == 1){ DrawDebuger(state->debuger.pos); }
        //DrawRectangle(deb.pos.x+10, deb.pos.y+10, 90, 90, YELLOW);
    
        DrawBugs();
        DrawPlayer(state);
        
//----------------------------
        EndMode2D();
        DrawHud(state);
        EndDrawing();
    }
    free(state->bugs);
    free(state->bullets);
    free(state->rups);
    free(state->virs);
    free(state->archives);
    free(state->backdoors);
    free(state->particles);
    free(state->up_termis);
    CloseWindow();
    return 0;
}

