#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "utils.h"
#include "renderer.h"


//####################################################--------------------------------------------
//##########----------------  S T A R T    O F    D R A W   funcs -------

void DrawHud(State *state)
{
    DrawText(TextFormat("%s", state->currenteFileName), GetScreenWidth()/2, 10, 20, GREEN);
    DrawText(TextFormat("CLEANED DATA: %d KB", state->score), 10, 10, 20, GREEN);
    
    char imunitybuffer[18];
    sprintf(imunitybuffer, "Bug immunity : %d", (int)state->player.corruption_immunity);

    DrawText(imunitybuffer , 10, 30, 20, GREEN);
    
    


    if(state->player.corruption_level >= state->player.corruption_immunity-1)//bug hunter corrupet beyond trust levels
    {
        char buff[256];
        sprintf(buff, "bug_hunter 0x%d copy has become too corrupted to be trusted ...\nCutting of support branch for this copy..", state->player_id);
        DrawText(buff, 
                GetScreenWidth()/2-400, GetScreenHeight()/2-49, 22, GREEN );
        
    }
}


void DrawPlayer(State *state)
{
    DrawRectangleRec(GetPlayerRect(state->player), BLUE);
    state->player.bug_update_timer -= GetFrameTime();

    for(int i = 0; i < 10; i++)
    {   
        if(i > state->player.corruption_level){ state->player.bugs[i].active = 0; state->player.bugs[i].pos = state->player.pos; continue; }
        state->player.bugs[i].active = 1;
        if(state->player.bug_update_timer < 0)
        {
            state->player.bugs[i].pos = (Vector2){state->player.pos.x + rand()%23, state->player.pos.y + rand()%23};
        }
        DrawBug(state->player.bugs[i], 1);
        DrawBug(state->player.bugs[i], 2);
    }
    if(state->player.bug_update_timer < 0){ state->player.bug_update_timer=0.3f; }
    
    if(state->player.dashTimer > 0)
    {

    }

    char buffer[64];
    char healthbuffer[18];
    sprintf(buffer, "x: %i, y: %i", (int)state->player.pos.x, (int)state->player.pos.y);
    sprintf(healthbuffer, "bugs\n%i", (int)state->player.corruption_level +1);

    DrawText(buffer, state->player.pos.x - 25, state->player.pos.y - 30, 10, WHITE);
    DrawText(healthbuffer ,state->player.pos.x - 25, state->player.pos.y - 5, 10, WHITE);
}

void DrawArchives()
{
    State *state = GetState();
    for(ArchiveText *arch = state->archives; arch != state->archives + state->archiveCount; arch++)
    {
        int OfsetX = (rand()%6-3);
        int OfsetY = (rand()%6-3);
        DrawRectangle(arch->pos.x, arch->pos.y, 15, 18, WHITE);
        DrawRectangle(arch->pos.x+OfsetX, arch->pos.y+OfsetY, 15, 18, LIGHTGRAY);
        if(CheckCollisionRecs(GetPlayerRect(state->player), (Rectangle){arch->pos.x, arch->pos.y, 15, 18}) )
        {
            Vector2 textSize = MeasureTextEx(GetFontDefault(), arch->text, 15.0f, 2.0f);
            float textX = (state->player.pos.x + 5.0f) - (textSize.x / 2.0f);
            float textY = state->player.pos.y - textSize.y - 20.0f - 30; 

            DrawTextEx(GetFontDefault(), arch->text, (Vector2){textX, textY}, 15.0f, 2.0f, WHITE);
        }
    }
}

void DrawParticles(Particle *par, int pass)
{
    int OfsetX = (pass == 0) ? 0 :(rand()%15-3);
    int OfsetY = (pass == 0) ? 0 :(rand()%15-3);
    
    Color tint;
    if (pass == 0) tint = par->color1;
    else if (pass == 1) tint = par->color2;
    if(par->lifetime > 0){ par->lifetime-= GetFrameTime(); }
    if(par->lifetime <= 0) { par->active = 0;}
    if (par->active == 0){ return; }
    DrawRectangle((int)par->pos.x + OfsetX, (int)par->pos.y + OfsetY, 6, 6, tint);
}
void DrawBug(Bug bug, int pass)
{
    int OfsetX = (pass == 0) ? 0 :(rand()%6-3);
    int OfsetY = (pass == 0) ? 0 :(rand()%6-3);
    
    Color tint;
    if (pass == 0) tint = BLUE;
    else if (pass == 1) tint = GRAY;
    else tint = RED;
    if (bug.active == 0){ return; }
    DrawRectangle((int)bug.pos.x + OfsetX, (int)bug.pos.y + OfsetY, 6, 6, tint);
}

void DrawUpdateTermis()
{
    State *state = GetState();
    for(Update_teminal *up = state->up_termis; up != state->up_termis + state->up_Count; up++)
    {
        if(up->active == 0){ continue; }
        
        int OfsetX = (rand()%6-3);
        int OfsetY = (rand()%6-3);
        DrawRectangle(up->pos.x, up->pos.y, 20, 18, WHITE);
        DrawRectangle(up->pos.x+OfsetX, up->pos.y+OfsetY, 20, 18, BLUE);
        if(CheckCollisionRecs(GetPlayerRect(state->player), (Rectangle){up->pos.x, up->pos.y, 15, 18}) )
        {
            up->timer -= GetFrameTime();
            Vector2 textSize = MeasureTextEx(GetFontDefault(), up->description, 15.0f, 2.0f);
            float textX = (state->player.pos.x + 5.0f) - (textSize.x / 2.0f);
            float textY = state->player.pos.y - textSize.y - 20.0f - 30; 
            char timerBuffer[8] = {0};
            sprintf(timerBuffer, "%d", (int)up->timer);

            DrawText(timerBuffer, state->player.pos.x, state->player.pos.y+100, 15.0f, WHITE);
            DrawTextEx(GetFontDefault(), up->description, (Vector2){textX, textY}, 15.0f, 2.0f, WHITE);

            if(up->timer <= 0)//update bought
            {
                switch (up->type)
                {
                    case 1:
                        if(state->score < 100){break;}
                        state->score -= 100;
                        state->player.corruption_immunity+=1;
                        up->timer = 6.0f;
                        break;
                }
            }
        }
        else{ up->timer = 6.0f; }
    }
}

void DrawDebuger(Vector2 pos)
{
    DrawText("Debuger", pos.x+30, pos.y-25, 12, WHITE);
    for(int i = 0; i < 10; i++)
    {
        for(int q = 0; q < 10; q++)
        {
            if(debuger_template[i * 10 + q] == 0){ continue;}
            DrawBug((Bug){ (Vector2){pos.x + q * 12, pos.y + i * 12}, 1, 1 }, 0);
            DrawBug((Bug){ (Vector2){pos.x + q * 12, pos.y + i * 12}, 1, 1 }, 1);
        }
    }
}

void DrawBadObject(int *sprite_template, int template_res, Vector2 pos, int hp)
{
    for(int pass = 0; pass < 3; pass++)
    {
        
        for(int i = 0; i < template_res; i++)
        {
            for(int q = 0; q < template_res; q++)
            {
                if(sprite_template[i * template_res + q] == 0){ continue;}
                DrawBug((Bug){ (Vector2){pos.x + q * 10, pos.y + i * 10}, 1, 1 }, pass);
                DrawBug((Bug){ (Vector2){pos.x + q * 12, pos.y + i * 12}, 1, 1 }, pass);
            }
        }
    }
    
    float radius = 15.0f; 
    float pi = 3.1415926535f;
    int thing = (template_res == 5) ? 4 : 10;

    for(int i = 0; i < hp; i++)
    {
        float angle = i * (2.0f * pi / thing);

        float offsetX = cosf(angle) * radius;
        float offsetY = sinf(angle) * radius;

        int drawX = (int)(pos.x + (template_res * 5) + offsetX);
        int drawY = (int)(pos.y + (template_res * 5) + offsetY);

        DrawText("x", drawX, drawY, 12, WHITE);
    }
    //75 * 75       75 /2 = 37.5/5= 7 ?
    DrawHelpArrow((Vector2){pos.x + template_res * 2, pos.y + template_res * 2}, GetState()->player.pos, BLUE);
}

void DrawBugs()
{
    int passes = 3;
    State *state = GetState();
    for(int pass = 0; pass < passes; pass++)
    {
        for (int i = 0; i < state->bugsCount; i++) {
            
            DrawBug(state->bugs[i], pass);
        }
    }
}

void DrawBullets()
{
    State *state = GetState();
    for(Bullet *bullet = state->bullets; bullet != state->bullets + state->bulletsCount; bullet++)
    {
        if(!bullet->active){ continue; }
        Vector2 backDir = Vector2Normalize(bullet->vel);
        
        DrawRectangle(bullet->pos.x, bullet->pos.y, 8, 8, (bullet->owner==0) ? WHITE : BLUE );
        
        int offX = (int)(-backDir.x * 10) + (rand() % 10 - 2);
        int offY = (int)(-backDir.y * 10) + (rand() % 10 - 2);
        DrawRectangle(bullet->pos.x + offX, bullet->pos.y + offY, 9, 9, (bullet->owner==0) ? BLUE : RED);

    }
}

void DrawTheBackdoor(Vector2 pos, char target[32])
{
    DrawText("Backdoor", pos.x - 10, pos.y -20, 12, WHITE);
    DrawRectangle(pos.x, pos.y, 30, 40, GREEN);
    DrawText(target, pos.x - 10, pos.y + 50, 12, WHITE);
}

void DrawHelpArrow(Vector2 pos, Vector2 dir, Color clr)
{
    Vector2 dirToTarget = Vector2Subtract(dir, pos);
    dirToTarget = Vector2Scale(Vector2Normalize(dirToTarget), 25);
    pos = Vector2Add(pos, dirToTarget);

    DrawRectangle(pos.x+12.5f, pos.y+12.5f, 5, 5, clr);
}

//####################################################--------------------------------------------
//##########----------------  E N D    O F    D R A W   funcs -------
