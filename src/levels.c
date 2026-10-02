#include "levels.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "types.h"


int load_level(char *filename)
{
    State *state = GetState();
    
    char buffername[32] = {0};
    memset(buffername, 0, sizeof(buffername));
    sprintf(buffername, "levels/%s.txt", filename);
    
    FILE *file = fopen(buffername, "r");
    if(!file){
        return -1;
    }
        
    char line[256];
    int type = -1;
    char target[256] = {0};
    //int numof = 0;
    while(fgets(line, sizeof(line), file))
    { 
        if(line[0] == '#') continue;
        if(line[0] == '[') //file name in game
        {
            memset(state->currenteFileName, 0, sizeof(state->currenteFileName));
            sscanf(line, "[%s", state->currenteFileName);
            state->currenteFileName[strlen(state->currenteFileName)] = '\0';
            continue;
        }
        if(line[0] == '+') // type selection + numu of
                           // 0 backdoor
                           // 1 player
                           // 2 virus
                           // 3 rup
                           // 7 update teminal
                           // 8 archive log
                           // 9 debuger
        {
            sscanf(line, "+%d", &type);
        }
        
        Vector2 pos;
        if(type == 0 || type == 8 || type == 7)
        {
            memset(target, 0, sizeof(target));
            int parsed = sscanf(line, "%f, %f %[^\n]", &pos.x, &pos.y, target);
            if(parsed!=3)continue;
            if(type==0){ SpawnBackdoor(pos, target); }
            else if(type==8){ SpawnArchive(pos, target); }//target == text
            else if(type==7){ SpawnUp_temi(pos, 1, target); }//target == text
            continue;
        }
        int parsed = sscanf(line, "%f, %f", &pos.x, &pos.y);
        if(parsed == 2)
        {
            switch(type)
            {
                case 1:
                    state->player.pos = pos;
                    break;
                case 2:
                    SpawnVirus(pos, (rand()%2) ? 1:0 );
                    break;
                case 3:
                    SpawnRuptutre(pos);
                    break;
                case 9:
                    state->debuger.active = 1;
                    state->debuger.pos = pos;
                    break;   
            }
        }
        
    }
    
    fclose(file);
    return 1;


}
