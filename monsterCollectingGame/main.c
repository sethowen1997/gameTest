#include "raylib.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#define INV_SQRT2 (1.0f / sqrtf(2.0f))
#include "flecs.h"

typedef enum
{
    IDLE,
    MOVE,
    PACE,
    ENCOUNTER,
    FIGHT,
    FOLLOW
} States;

typedef enum
{
    EXPLORE,
    BATTLE,
    ENDBATTLE
} GameStates;

typedef enum
{
    CHOOSE,
    ATTACK,
    DAMAGE
} BattleStates;

typedef enum
{
    FIRE,
    WATER,
    GRASS,
    EARTH,
    BASIC
} Elements;

typedef enum
{
    PHYSICAL,
    ELEMENTAL,
    STATUS
} StatType;

typedef struct
{
    float x;
    float y;
} Position;

typedef struct
{
    float x;
    float y;
} Velocity;

typedef struct
{
    uint16_t ID;
} TextureIndex;

typedef struct
{
    char dir;
} Direction;

typedef struct
{
    uint16_t frames[2];
    float timer;
} RightFrames;

typedef struct
{
    uint16_t frames[2];
    float timer;
} LeftFrames;

typedef struct
{
    uint16_t frames[4];
    float timer;
} UpFrames;

typedef struct
{
    uint16_t frames[4];
    float timer;
} DownFrames;

typedef struct
{
    uint16_t frame;
} DamagedLeftFrame;

typedef struct
{
    uint16_t frame;
} DamagedRightFrame;

typedef struct
{
    uint16_t frame;
} AttackRightFrame;

typedef struct
{
    uint16_t frame;
} AttackLeftFrame;

typedef struct
{
    States state;
} State;

typedef struct
{
    Rectangle col;
} Collision;

typedef struct
{
    uint16_t z;
} Depth;

typedef struct
{
    float x;
    float y;
} PastPosition;

typedef struct
{
    uint16_t ID[4];   
} AbilitySet;

typedef struct
{
    uint16_t damage;
    Elements element;
    StatType type;
    uint16_t rf[5];
    uint16_t lf[5];
    float timer;
} Ability;

typedef struct
{
    uint16_t gpoints;
} GeneticHPValue;

typedef struct
{
    uint16_t bpoints;
} BaseHP;

typedef struct
{
    int16_t points;
} HP;

typedef struct
{
    uint16_t gAV;
} GeneticAttackValue;

typedef struct
{
    uint16_t bAtk;
} BaseAttack;

typedef struct
{
    uint16_t atk;
} Attack;

typedef struct
{
    uint16_t gEAV;
} GeneticElementalAttackValue;

typedef struct
{
    uint16_t beAtk;
} BaseElementalAttack;

typedef struct
{
    uint16_t eAtk;
} ElementalAttack;

typedef struct
{
    uint16_t gDV;
} GeneticDefenseValue;

typedef struct
{
    uint16_t bDef;
} BaseDefense;

typedef struct
{
    uint16_t def;
} Defense;

typedef struct
{
    uint16_t gEDV;
} GeneticElementalDefenseValue;

typedef struct
{
    uint16_t beDef;
} BaseElementalDefense;

typedef struct
{
    uint16_t eDef;
} ElementalDefense;

typedef struct
{
    uint16_t gSV;
} GeneticSpeedValue;

typedef struct
{
    uint16_t bSpeed;
} BaseSpeed;

typedef struct
{
    uint16_t speed;
} Speed;

typedef struct
{
    uint16_t lvl;
} Level;

typedef struct
{
    uint32_t xp;
} Experience;

typedef struct
{
    uint16_t speed;
} WalkSpeed;

typedef struct {} Player;
typedef struct {} Monster;
typedef struct {} NPC;
typedef struct {} Tile;

int main(void)
{
    uint16_t scale = 8;
    
    int screenWidth  = 320 * scale;
    int screenHeight = 180 * scale;

    //SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "game");    
        
    Texture2D textureAtlas = LoadTexture("data/art/atlas.png");
    SetTextureFilter(textureAtlas, TEXTURE_FILTER_POINT);
    
    Rectangle textureRects[54];
    textureRects[0]  = (Rectangle){0, 0, 16, 16};     //Player down
    textureRects[1]  = (Rectangle){0, 36, 32, 32};    //Tree
    textureRects[2]  = (Rectangle){18, 0, 16, 16};    //player move down
    textureRects[3]  = (Rectangle){36, 0, 16, 16};    //player move down 2
    textureRects[4]  = (Rectangle){54, 0, 16, 16};    //player up
    textureRects[5]  = (Rectangle){72, 0, 16, 16};    //player move up
    textureRects[6]  = (Rectangle){90, 0, 16, 16};    //player move up 2
    textureRects[7]  = (Rectangle){108, 0, 16, 16};   //player left
    textureRects[8]  = (Rectangle){126, 0, 16, 16};   //player move left
    textureRects[9]  = (Rectangle){144, 0, 16, 16};   //player right
    textureRects[10] = (Rectangle){162, 0, 16, 16};   //player move right
    textureRects[11] = (Rectangle){34, 36, 16, 16};   //Rock
    textureRects[12] = (Rectangle){0, 18, 24, 16};    //Gnawmp left
    textureRects[13] = (Rectangle){26, 18, 24, 16};   //Gnawmp move left
    textureRects[14] = (Rectangle){52, 18, 24, 16};   //Gnawmp right
    textureRects[15] = (Rectangle){78, 18, 24, 16};   //Gnawmp move right
    textureRects[16] = (Rectangle){104, 18, 20, 16};  //Boarpy left
    textureRects[17] = (Rectangle){126, 18, 20, 16};  //Boarpy move left
    textureRects[18] = (Rectangle){148, 18, 20, 16};  //Boarpy right
    textureRects[19] = (Rectangle){170, 18, 20, 16};  //Boarpy move right
    textureRects[20] = (Rectangle){60, 40, 48, 48};   //Slash first frame
    textureRects[21] = (Rectangle){120, 40, 48, 48};
    textureRects[22] = (Rectangle){0, 95, 48, 48};
    textureRects[23] = (Rectangle){60, 95, 48, 48};
    textureRects[24] = (Rectangle){120, 95, 48, 48};  //Slash last frame
    textureRects[25] = (Rectangle){0, 150, 48, 48};   //Slash left first frame
    textureRects[26] = (Rectangle){60, 150, 48, 48};
    textureRects[27] = (Rectangle){110, 150, 48, 48};
    textureRects[28] = (Rectangle){0, 200, 48, 48};
    textureRects[29] = (Rectangle){60, 200, 48, 48};  //Slash left last frame
    textureRects[30] = (Rectangle){0, 256, 16, 16};   //Grass
    textureRects[51] = (Rectangle){20, 260, 22, 6};   //Healthbar
    textureRects[50] = (Rectangle){48, 260, 22, 6};
    textureRects[49] = (Rectangle){74, 260, 22, 6};
    textureRects[48] = (Rectangle){100, 260, 22, 6};
    textureRects[47] = (Rectangle){125, 260, 22, 6};
    textureRects[46] = (Rectangle){150, 260, 22, 6};
    textureRects[45] = (Rectangle){0, 275, 22, 6};
    textureRects[44] = (Rectangle){25, 275, 22, 6};
    textureRects[43] = (Rectangle){50, 275, 22, 6};
    textureRects[42] = (Rectangle){75, 275, 22, 6};
    textureRects[41] = (Rectangle){100, 275, 22, 6};
    textureRects[40] = (Rectangle){125, 275, 22, 6};
    textureRects[39] = (Rectangle){150, 275, 22, 6};
    textureRects[38] = (Rectangle){0, 284, 22, 6};
    textureRects[37] = (Rectangle){25, 284, 22, 6};
    textureRects[36] = (Rectangle){50, 284, 22, 6};
    textureRects[35] = (Rectangle){75, 284, 22, 6};
    textureRects[34] = (Rectangle){100, 284, 22, 6};
    textureRects[33] = (Rectangle){125, 284, 22, 6};
    textureRects[32] = (Rectangle){150, 284, 22, 6};
    textureRects[31] = (Rectangle){0, 292, 22, 6};   //Healthbar end
    textureRects[52] = (Rectangle){110, 200, 24, 16};//Gnawmp Damaged Left
    textureRects[53] = (Rectangle){150, 200, 24, 16};//Boarpy Attack
    
    Ability abilities[1];
    abilities[0].damage = 20;
    abilities[0].element = BASIC;
    abilities[0].type = PHYSICAL;
    abilities[0].rf[0] = 20;
    abilities[0].rf[1] = 21;
    abilities[0].rf[2] = 22;
    abilities[0].rf[3] = 23;
    abilities[0].rf[4] = 24;
    abilities[0].lf[0] = 25;
    abilities[0].lf[1] = 26;
    abilities[0].lf[2] = 27;
    abilities[0].lf[3] = 28;
    abilities[0].lf[4] = 29;
    abilities[0].timer = 0;
    
    uint16_t experienceGiven[100];
    experienceGiven[0] = 0;
    experienceGiven[1] = 10;
    experienceGiven[2] = 10;
    experienceGiven[3] = 10;
    experienceGiven[4] = 10;
    experienceGiven[5] = 10;
    
    uint32_t experienceLevel[100];
    experienceLevel[0] = 0;
    experienceLevel[1] = 0;
    experienceLevel[2] = 83;
    experienceLevel[3] = 174;
    experienceLevel[4] = 276;
    experienceLevel[5] = 388;
    
    ecs_world_t *world = ecs_init();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, TextureIndex);
    ECS_COMPONENT(world, Direction);
    ECS_COMPONENT(world, RightFrames);
    ECS_COMPONENT(world, LeftFrames);
    ECS_COMPONENT(world, UpFrames);
    ECS_COMPONENT(world, DownFrames);
    ECS_COMPONENT(world, State);
    ECS_COMPONENT(world, Collision);
    ECS_COMPONENT(world, Depth);
    ECS_COMPONENT(world, PastPosition);
    ECS_COMPONENT(world, AbilitySet);
    ECS_COMPONENT(world, GeneticHPValue);
    ECS_COMPONENT(world, BaseHP);
    ECS_COMPONENT(world, HP);
    ECS_COMPONENT(world, GeneticAttackValue);
    ECS_COMPONENT(world, BaseAttack);
    ECS_COMPONENT(world, Attack);
    ECS_COMPONENT(world, GeneticDefenseValue);
    ECS_COMPONENT(world, BaseDefense);
    ECS_COMPONENT(world, Defense);
    ECS_COMPONENT(world, GeneticElementalAttackValue);
    ECS_COMPONENT(world, BaseElementalAttack);
    ECS_COMPONENT(world, ElementalAttack);
    ECS_COMPONENT(world, GeneticElementalDefenseValue);
    ECS_COMPONENT(world, BaseElementalDefense);
    ECS_COMPONENT(world, ElementalDefense);
    ECS_COMPONENT(world, GeneticSpeedValue);
    ECS_COMPONENT(world, BaseSpeed);
    ECS_COMPONENT(world, Speed);
    ECS_COMPONENT(world, Level);
    ECS_COMPONENT(world, Experience);
    ECS_COMPONENT(world, WalkSpeed);
    ECS_COMPONENT(world, DamagedLeftFrame);
    ECS_COMPONENT(world, DamagedRightFrame);
    ECS_COMPONENT(world, AttackLeftFrame);
    ECS_COMPONENT(world, AttackRightFrame);
    
    ECS_TAG(world, Monster);
    ECS_TAG(world, NPC);
    ECS_TAG(world, Player);
    ECS_TAG(world, Tile);

    ecs_entity_t player = ecs_new(world);

    ecs_set(world, player, Position,     {0.0f, 64.0f});
    ecs_set(world, player, Velocity,     {0.0f, 0.0f});
    ecs_set(world, player, TextureIndex, {0});
    ecs_set(world, player, Direction,    {'d'});
    ecs_set(world, player, RightFrames,  {{10, 9}, 0.5f});
    ecs_set(world, player, LeftFrames,   {{8, 7}, 0.5f});
    ecs_set(world, player, UpFrames,     {{5, 4, 6, 4}, 0.5f});
    ecs_set(world, player, DownFrames,   {{2, 0, 3, 0}, 0.5f});
    ecs_set(world, player, State,        {IDLE});
    ecs_set(world, player, Collision,    {{3, 10, 13, 16}});
    ecs_set(world, player, Depth,        {8});
    ecs_set(world, player, PastPosition, {0.0f, 0.0f});
    ecs_set(world, player, Speed,        {30});
    ecs_add(world, player, Player);
    
    ecs_entity_t monster = ecs_new(world);
    
    ecs_set(world, monster, Position,     {64.0f, 64.0f});
    ecs_set(world, monster, Velocity,     {-1.0f, 0.0f});
    ecs_set(world, monster, TextureIndex, {12});
    ecs_set(world, monster, Direction,    {'l'});
    ecs_set(world, monster, RightFrames,  {{15, 14}, 0.5f});
    ecs_set(world, monster, LeftFrames,   {{13, 12}, 0.5f});
    ecs_set(world, monster, State,        {PACE});
    ecs_set(world, monster, Collision,    {{1, 7, 16, 16}});
    ecs_set(world, monster, Depth,        {8});
    ecs_set(world, monster, PastPosition, {64.0f, 64.0f});
    ecs_set(world, monster, AbilitySet,   {{0, 0, 0, 0}});
    ecs_set(world, monster, Experience,   {0});
    ecs_set(world, monster, WalkSpeed,    {30});
    ecs_set(world, monster, DamagedLeftFrame, {52});
    //ecs_set(world, monster, AttackRightFrame, {53});
    
    ecs_set(world, monster, Level, {1});
    ecs_set(world, monster, GeneticHPValue, {31});
    ecs_set(world, monster, BaseHP, {44});
    ecs_set(world, monster, HP, {0});
    ecs_set(world, monster, GeneticAttackValue, {31});
    ecs_set(world, monster, BaseAttack, {48});
    ecs_set(world, monster, Attack, {0});
    ecs_set(world, monster, GeneticDefenseValue, {31});
    ecs_set(world, monster, BaseDefense, {65});
    ecs_set(world, monster, Defense, {0});
    ecs_set(world, monster, GeneticElementalAttackValue, {31});
    ecs_set(world, monster, BaseElementalAttack, {50});
    ecs_set(world, monster, ElementalAttack, {0});
    ecs_set(world, monster, GeneticElementalDefenseValue, {31});
    ecs_set(world, monster, BaseElementalDefense, {64});
    ecs_set(world, monster, ElementalDefense, {0}); 
    ecs_set(world, monster, GeneticSpeedValue, {31});
    ecs_set(world, monster, BaseSpeed, {43});
    ecs_set(world, monster, Speed, {0});

    ecs_add(world, monster, Monster);
    
    ecs_entity_t activeTester = ecs_new(world);
    
    Position *pt = ecs_get_mut(world, player, Position);
    
    ecs_set(world, activeTester, Position,     {pt->x - 16, pt->y});
    ecs_set(world, activeTester, Velocity,     {0.0f, 0.0f});
    ecs_set(world, activeTester, TextureIndex, {18});
    ecs_set(world, activeTester, Direction,    {'r'});
    ecs_set(world, activeTester, RightFrames,  {{19, 18}, 0.5f});
    ecs_set(world, activeTester, LeftFrames,   {{17, 16}, 0.5f});
    ecs_set(world, activeTester, State,        {FOLLOW});
    ecs_set(world, activeTester, Depth,        {8});
    ecs_set(world, activeTester, PastPosition, {0.0f, 0.0f});
    ecs_set(world, activeTester, AbilitySet,   {{0, 0, 0, 0}});
    ecs_set(world, activeTester, Experience,   {0});
    ecs_set(world, activeTester, WalkSpeed,    {30});
    ecs_set(world, activeTester, DamagedLeftFrame, {52});
    ecs_set(world, activeTester, AttackRightFrame, {53});
    
    ecs_set(world, activeTester, Level, {1});
    ecs_set(world, activeTester, GeneticHPValue, {31});    
    ecs_set(world, activeTester, BaseHP, {44});  
    ecs_set(world, activeTester, HP, {0});
    ecs_set(world, activeTester, GeneticAttackValue, {31});
    ecs_set(world, activeTester, BaseAttack, {48});
    ecs_set(world, activeTester, Attack, {0});
    ecs_set(world, activeTester, GeneticDefenseValue, {31});
    ecs_set(world, activeTester, BaseDefense, {65});
    ecs_set(world, activeTester, Defense, {0});
    ecs_set(world, activeTester, GeneticElementalAttackValue, {31});
    ecs_set(world, activeTester, BaseElementalAttack, {50});
    ecs_set(world, activeTester, ElementalAttack, {0});
    ecs_set(world, activeTester, GeneticElementalDefenseValue, {31});
    ecs_set(world, activeTester, BaseElementalDefense, {64});
    ecs_set(world, activeTester, ElementalDefense, {0});
    ecs_set(world, activeTester, GeneticSpeedValue, {31});
    ecs_set(world, activeTester, BaseSpeed, {43});    
    ecs_set(world, activeTester, Speed, {0});
    
    ecs_add(world, activeTester, Monster);
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, -96});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, -80});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, -64});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, -48});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, -32});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, -16});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 0});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {160 + 16 * i, 0});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 16});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {160 + 16 * i, 16});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 32});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {160 + 16 * i, 32});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 48});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {160 + 16 * i, 48});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 64});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {160 + 16 * i, 64});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 80});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 11; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {160 + 16 * i, 80});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 96});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 112});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 128});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 144});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 160});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 176});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 36; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-240 + 16 * i, 192});
        ecs_set(world, e, TextureIndex, {1});
        ecs_set(world, e, Depth,        {24});
    }
    
    for (int i = 0; i < 15; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-64 + 16 * i, 0});
        ecs_set(world, e, TextureIndex, {30});
        ecs_add(world, e, Tile);
    }
    
    for (int i = 0; i < 15; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-64 + 16 * i, 16});
        ecs_set(world, e, TextureIndex, {30});
        ecs_add(world, e, Tile);
    }
    
    for (int i = 0; i < 15; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-64 + 16 * i, 32});
        ecs_set(world, e, TextureIndex, {30});
        ecs_add(world, e, Tile);
    }
    
    for (int i = 0; i < 15; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-64 + 16 * i, 48});
        ecs_set(world, e, TextureIndex, {30});
        ecs_add(world, e, Tile);
    }
    
    for (int i = 0; i < 15; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-64 + 16 * i, 64});
        ecs_set(world, e, TextureIndex, {30});
        ecs_add(world, e, Tile);
    }
    
    for (int i = 0; i < 15; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-64 + 16 * i, 80});
        ecs_set(world, e, TextureIndex, {30});
        ecs_add(world, e, Tile);
    }
    
    for (int i = 0; i < 15; i++)
    {
        ecs_entity_t   e = ecs_new(world);
        ecs_set(world, e, Position,     {-64 + 16 * i, 96});
        ecs_set(world, e, TextureIndex, {30});
        ecs_add(world, e, Tile);
    }
    
    ecs_query_t *tileQuery = ecs_query_init
    (
        world, &(ecs_query_desc_t)
        {
            .expr = "Position, TextureIndex, Tile"
        }
    );
    
    ecs_query_t *moveQuery = ecs_query_init
    (
        world, &(ecs_query_desc_t)
        {
            .expr = "Position, Velocity, Direction, TextureIndex, RightFrames, "
                    "LeftFrames, UpFrames, DownFrames, State, WalkSpeed, NPC"
        }
    );
    
    ecs_query_t *moveMonstersQuery = ecs_query_init
    (
        world, &(ecs_query_desc_t)
        {
            .expr = "Position, Velocity, Direction, TextureIndex, RightFrames, "
                    "LeftFrames, State, PastPosition, Collision, WalkSpeed, Monster"
        }
    );
    
    ecs_query_t *statsQuery = ecs_query_init
    (
        world, &(ecs_query_desc_t)
        {
            .expr = "Level, GeneticHPValue, BaseHP, HP, GeneticAttackValue, BaseAttack, Attack, GeneticElementalAttackValue, "
                    "BaseElementalAttack, ElementalAttack, GeneticDefenseValue, BaseDefense, Defense, GeneticElementalDefenseValue, "
                    "BaseElementalDefense, ElementalDefense, GeneticSpeedValue, BaseSpeed, Speed, Monster"
        }
    );
    
    ecs_iter_t statsIter = ecs_query_iter(world, statsQuery);
    
    while (ecs_query_next(&statsIter))
    {
        Level                          *statsLvl    = ecs_field(&statsIter, Level,                          0);
        GeneticHPValue                 *statsgHP    = ecs_field(&statsIter, GeneticHPValue,                 1);
        BaseHP                         *statsbHP    = ecs_field(&statsIter, BaseHP,                         2);
        HP                             *statsHP     = ecs_field(&statsIter, HP,                             3);
        GeneticAttackValue             *statsgAtk   = ecs_field(&statsIter, GeneticAttackValue,             4);
        BaseAttack                     *statsbAtk   = ecs_field(&statsIter, BaseAttack,                     5);  
        Attack                         *statsAtk    = ecs_field(&statsIter, Attack,                         6);   
        GeneticElementalAttackValue    *statsgeAtk  = ecs_field(&statsIter, GeneticElementalAttackValue,    7);
        BaseElementalAttack            *statsbeAtk  = ecs_field(&statsIter, BaseElementalAttack,            8);  
        ElementalAttack                *statseAtk   = ecs_field(&statsIter, ElementalAttack,                9); 
        GeneticDefenseValue            *statsgDef   = ecs_field(&statsIter, GeneticDefenseValue,           10);
        BaseDefense                    *statsbDef   = ecs_field(&statsIter, BaseDefense,                   11);  
        Defense                        *statsDef    = ecs_field(&statsIter, Defense,                       12);  
        GeneticElementalDefenseValue   *statsgeDef  = ecs_field(&statsIter, GeneticElementalDefenseValue,  13);
        BaseElementalDefense           *statsbeDef  = ecs_field(&statsIter, BaseElementalDefense,          14);  
        ElementalDefense               *statseDef   = ecs_field(&statsIter, ElementalDefense,              15); 
        GeneticSpeedValue              *statsgSpd   = ecs_field(&statsIter, GeneticSpeedValue,             16);
        BaseSpeed                      *statsbSpd   = ecs_field(&statsIter, BaseSpeed,                     17);  
        Speed                          *statsSpd    = ecs_field(&statsIter, Speed,                         18);         
        
        for (int i = 0; i < statsIter.count; i++)
        {
            statsHP[i].points = (2 * statsbHP[i].bpoints + statsgHP[i].gpoints) * statsLvl[i].lvl / 100 + statsLvl[i].lvl + 10;
            statsAtk[i].atk   = (2 * statsbAtk[i].bAtk + statsgAtk[i].gAV) * statsLvl[i].lvl / 100 + 5;
            statseAtk[i].eAtk   = (2 * statsbeAtk[i].beAtk + statsgeAtk[i].gEAV) * statsLvl[i].lvl / 100 + 5;
            statsDef[i].def   = (2 * statsbDef[i].bDef + statsgDef[i].gDV) * statsLvl[i].lvl / 100 + 5;
            statseDef[i].eDef   = (2 * statsbeDef[i].beDef + statsgeDef[i].gEDV) * statsLvl[i].lvl / 100 + 5;
            statsSpd[i].speed   = (2 * statsbSpd[i].bSpeed + statsgSpd[i].gSV) * statsLvl[i].lvl / 100 + 5;
        }
    }
    
    ecs_query_t *collisionQuery = ecs_query_init
    (
        world, &(ecs_query_desc_t)
        {
            .expr = "Position, Collision, !Player, !Monster"
        }
    );
    
    ecs_query_t *drawQuery = ecs_query_init
    (
        world, &(ecs_query_desc_t)
        {
            .expr = "Position, TextureIndex, Depth"
        }
    );
    
    uint16_t visibleEntityCount = 0;
    ecs_entity_t visibleEntities[500];
    
    uint16_t visibleTileCount = 0;
    ecs_entity_t visibleTiles[500];
    
    Camera2D camera = {0};
    camera.offset = (Vector2){screenWidth / 2.0f - 8 * scale, screenHeight / 2.0f - 8 * scale};
    camera.rotation = 0.0f;
    camera.zoom = scale;
    Vector2 cameraVel = (Vector2){0, 0};
    
    ecs_entity_t animation = 0;
    
    ecs_entity_t enemyHealthBar = 0;
    
    GameStates     gameState = EXPLORE;
    BattleStates battleState = CHOOSE;
    
    int16_t chosenAttack = -1;
    
    ecs_entity_t battleSlots[2];
    
    battleSlots[0] = activeTester;
    
    ecs_remove(world, battleSlots[0], Collision);
    
    float deltaTime;
    
    while (!WindowShouldClose()) 
    {
        deltaTime = GetFrameTime();
        
        Position      *p   = ecs_get_mut(world, player, Position);
        Velocity      *v   = ecs_get_mut(world, player, Velocity);
        Direction     *d   = ecs_get_mut(world, player, Direction);
        TextureIndex  *t   = ecs_get_mut(world, player, TextureIndex);
        RightFrames   *rf  = ecs_get_mut(world, player, RightFrames);
        LeftFrames    *lf  = ecs_get_mut(world, player, LeftFrames);
        UpFrames      *uf  = ecs_get_mut(world, player, UpFrames);
        DownFrames    *df  = ecs_get_mut(world, player, DownFrames);
        State         *st  = ecs_get_mut(world, player, State);
        Collision     *c   = ecs_get_mut(world, player, Collision);
        Speed         *sp  = ecs_get_mut(world, player, Speed);
        
        v->x = 0;
        v->y = 0;
        
        if (gameState == EXPLORE)
        {
            st->state = IDLE;
            
            if (IsKeyDown(KEY_D)) v->x =  1, d->dir = 'r', st->state = MOVE;
            if (IsKeyDown(KEY_A)) v->x = -1, d->dir = 'l', st->state = MOVE;
            if (IsKeyDown(KEY_W)) v->y = -1, d->dir = 'u', st->state = MOVE;
            if (IsKeyDown(KEY_S)) v->y =  1, d->dir = 'd', st->state = MOVE;
        }
        
        if (v->x != 0 && v->y != 0)
        {
            v->x *= INV_SQRT2;
            v->y *= INV_SQRT2;
        }
        
        p->x += v->x * sp->speed * deltaTime;
        
        ecs_iter_t collisionIter = ecs_query_iter(world, collisionQuery);

        while (ecs_query_next(&collisionIter))
        {
            Position  *clP  = ecs_field(&collisionIter, Position, 0);
            Collision *cl = ecs_field(&collisionIter, Collision, 1);            
            
            for (int i = 0; i < collisionIter.count; i++)
            {
                if (v->x > 0)
                {
                    if 
                    (
                            p->x + c->col.width  > clP[i].x + cl[i].col.x
                        &&  p->x + c->col.width  < clP[i].x + cl[i].col.width
                        &&  p->y + c->col.height > clP[i].y + cl[i].col.y
                        &&  p->y + c->col.y      < clP[i].y + cl[i].col.height
                    )
                    {
                        p->x = clP[i].x + cl[i].col.x - c->col.width;
                    }
                }
                
                else if (v->x < 0)
                {
                    if
                    (
                            p->x + c->col.x      < clP[i].x + cl[i].col.width
                        &&  p->x + c->col.x      > clP[i].x + cl[i].col.x
                        &&  p->y + c->col.height > clP[i].y + cl[i].col.y
                        &&  p->y + c->col.y      < clP[i].y + cl[i].col.height
                    )
                    {
                        p->x = clP[i].x + cl[i].col.width - c->col.x;
                    }
                       
                }
            }      
        }
        
        p->y += v->y * sp->speed * deltaTime;
        
        collisionIter = ecs_query_iter(world, collisionQuery);
        
        while (ecs_query_next(&collisionIter))
        {
            Position  *clP  = ecs_field(&collisionIter, Position, 0);
            Collision *cl   = ecs_field(&collisionIter, Collision, 1);            
            
            for (int i = 0; i < collisionIter.count; i++)
            {
                if (v->y > 0)
                {
                    if 
                    (
                            p->y + c->col.height > clP[i].y + cl[i].col.y
                        &&  p->y + c->col.height < clP[i].y + cl[i].col.height
                        &&  p->x + c->col.width  > clP[i].x + cl[i].col.x
                        &&  p->x + c->col.x      < clP[i].x + cl[i].col.width
                    )
                    {
                        p->y = clP[i].y + cl[i].col.y - c->col.height;
                    }
                }
                
                else if (v->y < 0)
                {
                    if 
                    (
                            p->y + c->col.y      < clP[i].y + cl[i].col.height
                        &&  p->y + c->col.y      > clP[i].y + cl[i].col.y
                        &&  p->x + c->col.width  > clP[i].x + cl[i].col.x
                        &&  p->x + c->col.x      < clP[i].x + cl[i].col.width
                    )
                    {
                        p->y = clP[i].y + cl[i].col.height - c->col.y;
                    }    
                }
            }        
        }
               
        switch (st->state)
        {
            case MOVE:
                
                switch (d->dir)
                {
                    case 'r':
                        t->ID = rf->frames[(uint16_t)rf->timer];
                        rf->timer += 4 * deltaTime;
                        if (rf->timer > 2) rf->timer = 0;
                        break;
                        
                    case 'l':
                        t->ID = lf->frames[(uint16_t)lf->timer];
                        lf->timer += 4 * deltaTime;
                        if (lf->timer > 2) lf->timer = 0;
                        break;
                        
                    case 'u':
                        t->ID = uf->frames[(uint16_t)uf->timer];
                        uf->timer += 4 * deltaTime;
                        if (uf->timer > 4) uf->timer = 0;
                        break;
                        
                    case 'd':
                        t->ID = df->frames[(uint16_t)df->timer];
                        df->timer += 4 * deltaTime;
                        if (df->timer > 4) df->timer = 0;
                        break;
                }
                
                break;
                
            case IDLE:
            
                switch (d->dir)
                {
                    case 'r':
                        t->ID = rf->frames[1];
                        rf->timer = 0.5;
                        break;
                        
                    case 'l':
                        t->ID = lf->frames[1];
                        lf->timer = 0.5;
                        break;
                        
                    case 'u':
                        t->ID = uf->frames[1];
                        uf->timer = 0.5;
                        break;
                        
                    case 'd':
                        t->ID = df->frames[1];
                        df->timer = 0.5;
                        break;
                }
            
                break;
                
            case PACE:
            
                break;
                
            case ENCOUNTER:
                
                
                break;   

            case FIGHT:
                
                
                break; 
                
            case FOLLOW:
                

                
                break; 
                
        }
        
        Position      *b0p   = ecs_get_mut(world, battleSlots[0], Position);
        Velocity      *b0v   = ecs_get_mut(world, battleSlots[0], Velocity);
        Direction     *b0d   = ecs_get_mut(world, battleSlots[0], Direction);
        TextureIndex  *b0t   = ecs_get_mut(world, battleSlots[0], TextureIndex);
        RightFrames   *b0rf  = ecs_get_mut(world, battleSlots[0], RightFrames);
        LeftFrames    *b0lf  = ecs_get_mut(world, battleSlots[0], LeftFrames);
        State         *b0st  = ecs_get_mut(world, battleSlots[0], State);
        WalkSpeed     *b0sp  = ecs_get_mut(world, battleSlots[0], WalkSpeed);
        AbilitySet    *b0as  = ecs_get_mut(world, battleSlots[0], AbilitySet);
        
        b0v->x = 0;
        
        if (b0st->state == FOLLOW)
        {
            if (p->x >= b0p->x + 24)
            {
                if (v->x > 0) 
                {
                    b0p->x = p->x - 24, b0v->x = 1, b0d->dir = 'r';
                    if (b0p->y < p->y) b0p->y += b0sp->speed * deltaTime;
                    if (b0p->y > p->y) b0p->y += -b0sp->speed * deltaTime;
                    if (b0p->y - p->y <= 1 && b0p->y - p->y >= -1) b0p->y = p->y;
                }
            }
            
            if (p->x <= b0p->x - 20)
            {
                if (v->x < 0) 
                {
                    b0p->x = p->x + 20, b0v->x = -1, b0d->dir = 'l';
                    if (b0p->y < p->y) b0p->y += b0sp->speed * deltaTime;
                    if (b0p->y > p->y) b0p->y += -b0sp->speed * deltaTime;
                    if (b0p->y - p->y <= 1 && b0p->y - p->y >= -1) b0p->y = p->y;
                }
            }
            
            if (p->y <= b0p->y - 17)
            {
                if (v->y < 0) 
                {
                    b0p->y = p->y + 17, b0v->x = 1, b0d->dir = 'r';
                    if (b0p->x < p->x - 3) b0p->x += b0sp->speed * deltaTime, b0d->dir = 'r';
                    
                    if (b0p->x > p->x - 3) b0p->x += -b0sp->speed * deltaTime, b0d->dir = 'l';
                    
                    if (b0p->x > p->x - 3.5 && b0p->x < p->x - 2.5) 
                    {
                        b0p->x = p->x - 3;
                        if (v->x > 0) b0v->x = 1, b0d->dir = 'r';
                        else if (v->x < 0) b0v->x = -1, b0d->dir = 'l';
                    }
                    
                    if (b0p->x > p->x - 3.5 && b0p->x <= p->x - 1 && v->x < 0) 
                    {
                        b0p->x = p->x - 1;
                    }
                    
                }
                

            }
            
            if (p->y >= b0p->y + 17)
            {               
                if (v->y > 0) 
                {
                    b0p->y = p->y - 17, b0v->x = 1, b0d->dir = 'r';
                    if (b0p->x < p->x - 3) b0p->x += b0sp->speed * deltaTime, b0d->dir = 'r';
                    
                    if (b0p->x > p->x - 3) b0p->x += -b0sp->speed * deltaTime, b0d->dir = 'l';
                    
                    if (b0p->x > p->x - 3.5 && b0p->x < p->x - 2.5) 
                    {
                        b0p->x = p->x - 3;
                        if (v->x > 0) b0v->x = 1, b0d->dir = 'r';
                        else if (v->x < 0) b0v->x = -1, b0d->dir = 'l';
                    }
                    
                    if (b0p->x > p->x - 3.5 && b0p->x <= p->x - 1 && v->x < 0) 
                    {
                        b0p->x = p->x - 1;
                    }
                }
            }
            
            if (b0v->x != 0)
            {
                switch (b0d->dir)
                {

                    case 'r':
                        b0t->ID = b0rf->frames[(uint16_t)b0rf->timer];
                        b0rf->timer += 4 * deltaTime;
                        if (b0rf->timer > 2) b0rf->timer = 0;
                        break;
                        
                    case 'l':
                        b0t->ID = b0lf->frames[(uint16_t)b0lf->timer];
                        b0lf->timer += 4 * deltaTime;
                        if (b0lf->timer > 2) b0lf->timer = 0;
                        break;

                }
            }
            
            else
            {
                switch (b0d->dir)
                {
                    case 'r':
                        b0t->ID = b0rf->frames[1];
                        b0rf->timer = 0.5;
                        break;
                        
                    case 'l':
                        b0t->ID = b0lf->frames[1];
                        b0lf->timer = 0.5;
                        break;
                }
            }
        }
        
        ecs_iter_t moveNPCsIter = ecs_query_iter(world, moveQuery);

        while (ecs_query_next(&moveNPCsIter))
        {
            Position       *moveP  = ecs_field(&moveNPCsIter, Position,       0);
            Velocity       *moveV  = ecs_field(&moveNPCsIter, Velocity,       1);
            Direction      *moveD  = ecs_field(&moveNPCsIter, Direction,      2);
            TextureIndex   *moveT  = ecs_field(&moveNPCsIter, TextureIndex,   3);
            RightFrames    *moveRF = ecs_field(&moveNPCsIter, RightFrames,    4);
            LeftFrames     *moveLF = ecs_field(&moveNPCsIter, LeftFrames,     5);
            UpFrames       *moveUF = ecs_field(&moveNPCsIter, UpFrames,       6);
            DownFrames     *moveDF = ecs_field(&moveNPCsIter, DownFrames,     7);
            State          *moveSt = ecs_field(&moveNPCsIter, State,          8);
            WalkSpeed      *moveSp = ecs_field(&moveNPCsIter, WalkSpeed,      9);
            
            for (int i = 0; i < moveNPCsIter.count; i++)
            {
                if (moveV[i].x != 0 && moveV[i].y != 0)
                {
                    moveV[i].x *= INV_SQRT2;
                    moveV[i].y *= INV_SQRT2;
                }
                
                moveP[i].x += moveV[i].x * moveSp[i].speed * deltaTime;
                moveP[i].y += moveV[i].y * moveSp[i].speed * deltaTime;
                
                switch (moveSt[i].state)
                {
                    case MOVE:
                        
                        switch (moveD[i].dir)
                        {
                            case 'r':
                                moveT[i].ID = moveRF[i].frames[(uint16_t)moveRF[i].timer];
                                moveRF[i].timer += 4 * deltaTime;
                                if (moveRF[i].timer > 2) moveRF[i].timer = 0;
                                break;
                                
                            case 'l':
                                moveT[i].ID = moveLF[i].frames[(uint16_t)moveLF[i].timer];
                                moveLF[i].timer += 4 * deltaTime;
                                if (moveLF[i].timer > 2) moveLF[i].timer = 0;
                                break;
                                
                            case 'u':
                                moveT[i].ID = moveUF[i].frames[(uint16_t)moveUF[i].timer];
                                moveUF[i].timer += 4 * deltaTime;
                                if (moveUF[i].timer > 4) moveUF[i].timer = 0;
                                break;
                                
                            case 'd':
                                moveT[i].ID = moveDF[i].frames[(uint16_t)moveDF[i].timer];
                                moveDF[i].timer += 4 * deltaTime;
                                if (moveDF[i].timer > 4) moveDF[i].timer = 0;
                                break;
                        }
                        
                        break;
                        
                    case IDLE:
                    
                        switch (moveD[i].dir)
                        {
                            case 'r':
                                moveT[i].ID = moveRF[i].frames[1];
                                moveRF[i].timer = 0.5;
                                break;
                                
                            case 'l':
                                moveT[i].ID = moveLF[i].frames[1];
                                moveLF[i].timer = 0.5;
                                break;
                                
                            case 'u':
                                moveT[i].ID = moveUF[i].frames[1];
                                moveUF[i].timer = 0.5;
                                break;
                                
                            case 'd':
                                moveT[i].ID = moveDF[i].frames[1];
                                moveDF[i].timer = 0.5;
                                break;
                        }
                    
                        break;
                        
                    case PACE:
                        break;
                        
                    case ENCOUNTER:
                        
                        break;

                    case FIGHT:
                        
                        break; 

                    case FOLLOW:
                        
                        break;    
                        
                }
            }
        }
        
        ecs_iter_t moveMonstersIter = ecs_query_iter(world, moveMonstersQuery);

        while (ecs_query_next(&moveMonstersIter))
        {
            Position       *moveP   = ecs_field(&moveMonstersIter, Position,       0);
            Velocity       *moveV   = ecs_field(&moveMonstersIter, Velocity,       1);
            Direction      *moveD   = ecs_field(&moveMonstersIter, Direction,      2);
            TextureIndex   *moveT   = ecs_field(&moveMonstersIter, TextureIndex,   3);
            RightFrames    *moveRF  = ecs_field(&moveMonstersIter, RightFrames,    4);
            LeftFrames     *moveLF  = ecs_field(&moveMonstersIter, LeftFrames,     5);
            State          *moveSt  = ecs_field(&moveMonstersIter, State,          6);
            PastPosition   *movePP  = ecs_field(&moveMonstersIter, PastPosition,   7);
            Collision      *moveC   = ecs_field(&moveMonstersIter, Collision,      8);
            WalkSpeed      *moveSp  = ecs_field(&moveMonstersIter, WalkSpeed,      9);
            
            for (int i = 0; i < moveMonstersIter.count; i++)
            {
                if (moveV[i].x != 0 && moveV[i].y != 0)
                {
                    moveV[i].x *= INV_SQRT2;
                    moveV[i].y *= INV_SQRT2;
                }
                
                moveP[i].x += moveV[i].x * moveSp[i].speed * deltaTime;
                moveP[i].y += moveV[i].y * moveSp[i].speed * deltaTime;
                
                if 
                (
                        p->x + c->col.width  > moveP[i].x + moveC[i].col.x
                    &&  p->x + c->col.width  < moveP[i].x + moveC[i].col.width
                    &&  p->y + c->col.height > moveP[i].y + moveC[i].col.y
                    &&  p->y + c->col.y      < moveP[i].y + moveC[i].col.height
                )
                {
                    st->state = ENCOUNTER;
                    moveSt[i].state = ENCOUNTER;
                    gameState = BATTLE;
                    cameraVel.x = 1;
                    moveV[i].x = 1;
                    moveD[i].dir = 'r';
                    battleSlots[1] = moveMonstersIter.entities[i];
                    ecs_get_mut(world, battleSlots[0], State)->state = ENCOUNTER;
                }

                else if
                (
                        p->x + c->col.x      < moveP[i].x + moveC[i].col.width
                    &&  p->x + c->col.x      > moveP[i].x + moveC[i].col.x
                    &&  p->y + c->col.height > moveP[i].y + moveC[i].col.y
                    &&  p->y + c->col.y      < moveP[i].y + moveC[i].col.height
                )
                {
                    st->state = ENCOUNTER;
                    moveSt[i].state = ENCOUNTER;
                    gameState = BATTLE;
                    cameraVel.x = -1;
                    moveV[i].x = -1;
                    moveD[i].dir = 'l';
                    battleSlots[1] = moveMonstersIter.entities[i];
                    ecs_get_mut(world, battleSlots[0], State)->state = ENCOUNTER;
                }
                
                switch (moveSt[i].state)
                {
                    case MOVE:
                        
                        switch (moveD[i].dir)
                        {
                            case 'r':
                                moveT[i].ID = moveRF[i].frames[(uint16_t)moveRF[i].timer];
                                moveRF[i].timer += 4 * deltaTime;
                                if (moveRF[i].timer > 2) moveRF[i].timer = 0;
                                break;
                                
                            case 'l':
                                moveT[i].ID = moveLF[i].frames[(uint16_t)moveLF[i].timer];
                                moveLF[i].timer += 4 * deltaTime;
                                if (moveLF[i].timer > 2) moveLF[i].timer = 0;
                                break;
                        }
                        
                        break;
                        
                    case IDLE:
                    
                        switch (moveD[i].dir)
                        {
                            case 'r':
                                moveT[i].ID = moveRF[i].frames[1];
                                moveRF[i].timer = 0.5;
                                break;
                                
                            case 'l':
                                moveT[i].ID = moveLF[i].frames[1];
                                moveLF[i].timer = 0.5;
                                break;
                        }
                    
                        break;
                        
                    case PACE:
                    
                        if (moveP[i].x < movePP[i].x - 50) moveV[i].x = 1, moveD[i].dir = 'r';
                        else if (moveP[i].x > movePP[i].x + 50) moveV[i].x = -1, moveD[i].dir = 'l';
                        
                        switch (moveD[i].dir)
                        {
                            case 'r':
                                moveT[i].ID = moveRF[i].frames[(uint16_t)moveRF[i].timer];
                                moveRF[i].timer += 4 * deltaTime;
                                if (moveRF[i].timer > 2) moveRF[i].timer = 0;
                                break;
                                
                            case 'l':
                                moveT[i].ID = moveLF[i].frames[(uint16_t)moveLF[i].timer];
                                moveLF[i].timer += 4 * deltaTime;
                                if (moveLF[i].timer > 2) moveLF[i].timer = 0;
                                break;
                        }
                        
                        break;
                        
                    case ENCOUNTER:
                        
                        if (moveV[i].x != 0)
                        {
                            switch (moveD[i].dir)
                            {
                                case 'r':
                                    moveT[i].ID = moveRF[i].frames[(uint16_t)moveRF[i].timer];
                                    moveRF[i].timer += 4 * deltaTime;
                                    if (moveRF[i].timer > 2) moveRF[i].timer = 0;
                                    break;
                                    
                                case 'l':
                                    moveT[i].ID = moveLF[i].frames[(uint16_t)moveLF[i].timer];
                                    moveLF[i].timer += 4 * deltaTime;
                                    if (moveLF[i].timer > 2) moveLF[i].timer = 0;
                                    break;
                            }
                        }
                        
                        break;
                        
                    case FIGHT:

                        
                        break; 
                        
                    case FOLLOW:
                        

                        break; 
                
                }
            }
        }
        
        switch (gameState)
        {
            case EXPLORE:
            
                camera.target.x = p->x;
                camera.target.y = p->y;
                
                camera.target.x = roundf(camera.target.x * camera.zoom) / camera.zoom;
                camera.target.y = roundf(camera.target.y * camera.zoom) / camera.zoom;
                
                break;
            
            case BATTLE:
                
                Position     *b1P  = ecs_get_mut(world, battleSlots[1], Position);
                Velocity     *b1V  = ecs_get_mut(world, battleSlots[1], Velocity);
                Direction    *b1D  = ecs_get_mut(world, battleSlots[1], Direction);
                TextureIndex *b1T  = ecs_get_mut(world, battleSlots[1], TextureIndex);
                State        *b1st = ecs_get_mut(world, battleSlots[1], State);
                LeftFrames   *b1LF = ecs_get_mut(world, battleSlots[1], LeftFrames);
                RightFrames  *b1RF = ecs_get_mut(world, battleSlots[1], RightFrames);
                PastPosition *b1PP = ecs_get_mut(world, battleSlots[1], PastPosition);
                //AbilitySet   *b1AS = ecs_get_mut(world, battleSlots[1], AbilitySet);
                
                
                Level          *b1LVL = ecs_get_mut(world, battleSlots[1], Level);
                BaseHP         *b1bHP = ecs_get_mut(world, battleSlots[1], BaseHP);
                GeneticHPValue *b1gHP = ecs_get_mut(world, battleSlots[1], GeneticHPValue);
                HP             *b1HP = ecs_get_mut(world, battleSlots[1], HP);
                //Attack       *b1atk = ecs_get_mut(world, battleSlots[1], Attack);
                Defense      *b1def = ecs_get_mut(world, battleSlots[1], Defense);
                ElementalDefense      *b1edef = ecs_get_mut(world, battleSlots[1], ElementalDefense);
                WalkSpeed    *b1sp = ecs_get_mut(world, battleSlots[1], WalkSpeed);
                
                Level             *b0LVL = ecs_get_mut(world, battleSlots[0], Level);
                HP                *b0HP = ecs_get_mut(world, battleSlots[0], HP);
                Attack            *b0atk = ecs_get_mut(world, battleSlots[0], Attack);
                Defense           *b0def = ecs_get_mut(world, battleSlots[0], Defense);
                ElementalAttack   *b0eatk = ecs_get_mut(world, battleSlots[0], ElementalAttack);
                ElementalDefense  *b0edef = ecs_get_mut(world, battleSlots[0], ElementalDefense);
                Speed             *b0spd = ecs_get_mut(world, battleSlots[0], Speed);
                
                BaseHP                     *b0bHP  = ecs_get_mut(world, battleSlots[0], BaseHP);
                BaseAttack                 *b0batk = ecs_get_mut(world, battleSlots[0], BaseAttack);
                BaseDefense                *b0bdef = ecs_get_mut(world, battleSlots[0], BaseDefense);
                BaseElementalAttack        *b0beatk = ecs_get_mut(world, battleSlots[0], BaseElementalAttack);
                BaseElementalDefense       *b0bedef = ecs_get_mut(world, battleSlots[0], BaseElementalDefense);
                BaseSpeed                  *b0bspd = ecs_get_mut(world, battleSlots[0], BaseSpeed);
                
                GeneticHPValue                *b0gHP   = ecs_get_mut(world, battleSlots[0], GeneticHPValue);
                GeneticAttackValue            *b0gatk  = ecs_get_mut(world, battleSlots[0], GeneticAttackValue);
                GeneticDefenseValue           *b0gdef  = ecs_get_mut(world, battleSlots[0], GeneticDefenseValue);
                GeneticElementalAttackValue   *b0geatk = ecs_get_mut(world, battleSlots[0], GeneticElementalAttackValue);
                GeneticElementalDefenseValue  *b0gedef = ecs_get_mut(world, battleSlots[0], GeneticElementalDefenseValue);
                GeneticSpeedValue             *b0gspd  = ecs_get_mut(world, battleSlots[0], GeneticSpeedValue);
                
                Experience        *b0exp = ecs_get_mut(world, battleSlots[0], Experience);
                
                if (st->state == ENCOUNTER)
                {
                    b1sp->speed = 60;
                    b0sp->speed = 30;
                    
                    if (cameraVel.x > 0)
                    {
                        camera.target.x += 35 * deltaTime;
                        
                        b1V->x = 1;
                        b0p->x += b0sp->speed * deltaTime;
                        b0v->x = 1;
                        b0d->dir = 'r';
                        
                        t->ID = rf->frames[1];
                        
                        if (b1P->x >= p->x + 74)
                        {
                            b1P->x = p->x + 74;
                            b1T->ID = b1LF->frames[1];
                        }
                        
                        if (b0p->x >= p->x + 24)
                        {
                            b0p->x = p->x + 24;
                            b0v->x = 0;
                            b0t->ID = b0rf->frames[1];
                        }
                        
                        if (camera.target.x >= p->x + 50) 
                        {
                            camera.target.x = p->x + 50;                           
                        }
                        
                        if (b1P->x >= p->x + 74
                        &&  camera.target.x >= p->x + 50
                        &&  b0p->x >= p->x + 24)
                        {
                            st->state = FIGHT;
                            
                            b1V->x = 0;
                            b1V->y = 0;
                            b1D->dir = 'l';
                            b1T->ID = b1LF->frames[1];
                            
                            enemyHealthBar = ecs_new(world);
                            ecs_set(world, enemyHealthBar, Position, {b1P->x, b1P->y + 18}); 
                            ecs_set(world, enemyHealthBar, TextureIndex, {31}); 
                            ecs_set(world, enemyHealthBar, Depth, {50});
                            
                            b1st->state = FIGHT;
                            b0st->state = FIGHT;
                        }
                    }
                    
                    else if (cameraVel.x < 0)
                    {
                        camera.target.x += -35 * deltaTime;
                        
                        b1V->x = -1;
                        b0p->x += -b0sp->speed * deltaTime;
                        b0v->x = -1;
                        b0d->dir = 'l';
                        
                        t->ID = lf->frames[1];
                        
                        if (b1P->x <= p->x - 74)
                        {
                            b1P->x = p->x - 74;
                            b1T->ID = b1RF->frames[1];
                        }
                        
                        if (b0p->x <= p->x - 24)
                        {
                            b0p->x = p->x - 24;
                            b0v->x = 0;
                            b0t->ID = b0lf->frames[1];
                        }
                        
                        if (camera.target.x <= p->x - 50) 
                        {
                            camera.target.x = p->x - 50;                            
                        }
                        
                        if (b1P->x <= p->x - 74
                        &&  camera.target.x <= p->x - 50
                        &&  b0p->x <= p->x - 24)
                        {
                            st->state = FIGHT;
                            
                            b1V->x = 0;
                            b1V->y = 0;
                            b1D->dir = 'r';
                            b1T->ID = b1RF->frames[1];
                            
                            enemyHealthBar = ecs_new(world);
                            ecs_set(world, enemyHealthBar, Position, {b1P->x, b1P->y}); 
                            ecs_set(world, enemyHealthBar, TextureIndex, {31}); 
                            ecs_set(world, enemyHealthBar, Depth, {50});
                            
                            b1st->state = FIGHT;
                            b0st->state = FIGHT;
                        }
                    }
                    
                    if (b1P->y < p->y) b1V->y =  1;
                    if (b1P->y > p->y) b1V->y = -1;
                    if (b1P->y < (p->y + 1) && b1P->y > (p->y - 1)) b1P->y = p->y;
                    
                    if (b0p->y < p->y) b0p->y += b0sp->speed * deltaTime;
                    if (b0p->y > p->y) b0p->y += -b0sp->speed * deltaTime;
                    if (b0p->y < (p->y + 1) && b0p->y > (p->y - 1)) b0p->y = p->y;
                }
                
                else if (st->state == FIGHT)
                {
                    b1V->x = 0;
                    b1V->y = 0;
                    
                    TextureIndex *ehbT = ecs_get_mut(world, enemyHealthBar, TextureIndex);
                                        
                    int maxHP = (2 * b1bHP->bpoints + b1gHP->gpoints) * b1LVL->lvl / 100 + b1LVL->lvl + 10;
                    ehbT->ID = (int)((float)b1HP->points / maxHP * 20 + 31);
                    
                    if (IsKeyPressed(KEY_E))
                    {
                        gameState = EXPLORE;
                        st->state = IDLE;
                        b1st->state = PACE;
                        b1sp->speed = 30;
                        b1PP->x = b1P->x;
                        b1PP->y = b1P->y;
                        b1V->x = -1;
                        b1D->dir = 'l';
                        b0st->state = FOLLOW;
                        b0sp->speed = 30;                          
                    }
                    
                    if (battleState == CHOOSE)
                    {
                        if (IsKeyPressed(KEY_ONE)) 
                        {
                            chosenAttack = 0;
                            battleState = ATTACK;
                            
                            animation = ecs_new(world);
                            if (b0d->dir == 'r')
                            {
                                ecs_set(world, animation, Position, {b0p->x + 20, b0p->y - 16}); 
                                ecs_set(world, animation, TextureIndex, {abilities[b0as->ID[chosenAttack]].rf[0]}); 
                            }
                            
                            else if (b0d->dir == 'l')
                            {
                                ecs_set(world, animation, Position, {b0p->x - 44, b0p->y - 16}); 
                                ecs_set(world, animation, TextureIndex, {abilities[b0as->ID[chosenAttack]].lf[0]}); 
                            }
                            
                            ecs_set(world, animation, Depth, {48});
                        }
                    }
                    
                    if (chosenAttack != -1)
                    {
                        if (battleState == ATTACK)
                        {                                
                            TextureIndex *AT = ecs_get_mut(world, animation, TextureIndex);
                            DamagedLeftFrame *b1dlF = ecs_get_mut(world, battleSlots[1], DamagedLeftFrame);
                            
                            AttackRightFrame *b0arF = ecs_get_mut(world, battleSlots[0], AttackRightFrame);
                            
                            if (b0d->dir == 'r')
                            {
                                AT->ID = abilities[b0as->ID[chosenAttack]].rf[(uint16_t)abilities[b0as->ID[chosenAttack]].timer];
                                abilities[b0as->ID[chosenAttack]].timer += 10 * deltaTime;
                                
                                b0t->ID = b0arF->frame;
                                b1T->ID = b1dlF->frame;
                                
                                if (abilities[b0as->ID[chosenAttack]].timer > 4) 
                                {
                                    abilities[b0as->ID[chosenAttack]].timer = 0;
                                    ecs_delete(world, animation);
                                    battleState = DAMAGE;
                                    b1T->ID = b1LF->frames[1];
                                    b0t->ID = b0rf->frames[1];
                                }         
                            }
                            
                            else if (b0d->dir == 'l')
                            {
                                AT->ID = abilities[b0as->ID[chosenAttack]].lf[(uint16_t)abilities[b0as->ID[chosenAttack]].timer];
                                abilities[b0as->ID[chosenAttack]].timer += 10 * deltaTime;
                                if (abilities[b0as->ID[chosenAttack]].timer > 4) 
                                {
                                    abilities[b0as->ID[chosenAttack]].timer = 0;
                                    ecs_delete(world, animation);
                                    battleState = DAMAGE;
                                }
                            }
                        }
                        
                        else if (battleState == DAMAGE)
                        {   
                            uint16_t damage = 0;
                            if (abilities[b0as->ID[chosenAttack]].type == PHYSICAL)
                            {
                                damage = (2 * b0LVL->lvl / 5) * abilities[b0as->ID[chosenAttack]].damage *
                                b0atk->atk / b1def->def / 50 + 2;
                            }
                            
                            else if (abilities[b0as->ID[chosenAttack]].type == ELEMENTAL)
                            {
                                damage = (2 * b0LVL->lvl / 5) * abilities[b0as->ID[chosenAttack]].damage *
                                b0eatk->eAtk / b1edef->eDef / 50 + 2;
                            }
                            
                            b1HP->points -= damage;
                            
                            chosenAttack = -1;
                            battleState = CHOOSE;
                        }
                    }
                    
                    if (b1HP->points <= 0)
                    {
                        gameState = ENDBATTLE;
                        gameState = ENDBATTLE;
                        
                        ecs_delete(world, battleSlots[1]);
                        ecs_delete(world, enemyHealthBar);

                        b0exp->xp += experienceGiven[b1LVL->lvl];
                        
                        if (b0exp->xp >= experienceLevel[b0LVL->lvl + 1])
                        {
                            b0LVL->lvl++;
                            
                            b0HP->points = (2 * b0bHP->bpoints + b0gHP->gpoints) * b0LVL->lvl / 100 + b0LVL->lvl + 10;
                            
                            b0atk->atk = (2 * b0batk->bAtk + b0gatk->gAV) * b0LVL->lvl / 100 + 5;
                            
                            b0def->def = (2 * b0bdef->bDef + b0gdef->gDV) * b0LVL->lvl / 100 + 5;
                            
                            b0eatk->eAtk = (2 * b0beatk->beAtk + b0geatk->gEAV) * b0LVL->lvl / 100 + 5;
                            
                            b0edef->eDef = (2 * b0bedef->beDef + b0gedef->gEDV) * b0LVL->lvl / 100 + 5;
                            
                            b0spd->speed = (2 * b0bspd->bSpeed + b0gspd->gSV) * b0LVL->lvl / 100 + 5;
                        }
                    }
                }
                
                if (b0v->x != 0)
                {
                    switch (b0d->dir)
                    {
                        case 'r':
                            b0t->ID = b0rf->frames[(uint16_t)b0rf->timer];
                            b0rf->timer += 4 * deltaTime;
                            if (b0rf->timer > 2) b0rf->timer = 0;
                            break;
                            
                        case 'l':
                            b0t->ID = b0lf->frames[(uint16_t)b0lf->timer];
                            b0lf->timer += 4 * deltaTime;
                            if (b0lf->timer > 2) b0lf->timer = 0;
                            break;
                    }
                }
            
                break;
                
            case ENDBATTLE:
                
                if (b0d->dir == 'r')
                {
                    camera.target.x += -50 * deltaTime;
                    if (camera.target.x <= p->x) 
                    {
                        camera.target.x = p->x;
                        gameState = EXPLORE;
                        st->state = IDLE;
                        b0st->state = FOLLOW;
                        
                    }
                }
                
                else if (b0d->dir == 'l')
                {
                    camera.target.x += 50 * deltaTime;
                    if (camera.target.x >= p->x) 
                    {
                        camera.target.x = p->x;
                        gameState = EXPLORE;
                        st->state = IDLE;
                        b0st->state = FOLLOW;
                    }
                }
                
                break;
        }
        
        float leftBoundary   = camera.target.x - (camera.offset.x / camera.zoom);
        float rightBoundary  = camera.target.x + ((screenWidth  - camera.offset.x) / camera.zoom);
        float topBoundary    = camera.target.y - (camera.offset.y / camera.zoom);
        float bottomBoundary = camera.target.y + ((screenHeight - camera.offset.y) / camera.zoom);
        
        visibleTileCount = 0;
        
        ecs_iter_t tileIter = ecs_query_iter(world, tileQuery);

        while (ecs_query_next(&tileIter))
        {
            Position       *tileP  = ecs_field(&tileIter, Position,       0);
            TextureIndex   *tileT  = ecs_field(&tileIter, TextureIndex,   1);
            
            for (int i = 0; i < tileIter.count; i++)
            {
                if (tileP[i].x + textureRects[tileT[i].ID].width  >= leftBoundary &&
                    tileP[i].x                                    <= rightBoundary &&
                    tileP[i].y + textureRects[tileT[i].ID].height >= topBoundary &&
                    tileP[i].y                                    <= bottomBoundary)
                {
                    visibleTiles[visibleTileCount++] = tileIter.entities[i];
                }
            }
        }

        visibleEntityCount = 0;

        ecs_iter_t drawIter = ecs_query_iter(world, drawQuery);

        while (ecs_query_next(&drawIter))
        {
            Position     *drawP   = ecs_field(&drawIter, Position, 0);
            TextureIndex *drawTex = ecs_field(&drawIter, TextureIndex, 1);

            for (int i = 0; i < drawIter.count; i++)
            {
                if (drawP[i].x + textureRects[drawTex[i].ID].width  >= leftBoundary &&
                    drawP[i].x                                      <= rightBoundary &&
                    drawP[i].y + textureRects[drawTex[i].ID].height >= topBoundary &&
                    drawP[i].y                                      <= bottomBoundary)
                {
                    visibleEntities[visibleEntityCount++] = drawIter.entities[i];
                }
            }
        }

        for (int i = 1; i < visibleEntityCount; i++)
        {
            ecs_entity_t key = visibleEntities[i];

            const Position *kp = ecs_get(world, key, Position);
            const Depth    *kd = ecs_get(world, key, Depth);
            float keyVal = kp->y + kd->z;

            int j = i - 1;

            while (j >= 0)
            {
                const Position *jp = ecs_get(world, visibleEntities[j], Position);
                const Depth    *jd = ecs_get(world, visibleEntities[j], Depth);
                float jVal = jp->y + jd->z;

                if (jVal <= keyVal) break;

                visibleEntities[j + 1] = visibleEntities[j];
                j--;
            }

            visibleEntities[j + 1] = key;
        }
        
        BeginDrawing();

        ClearBackground(WHITE);
        
        BeginMode2D(camera);
        
        for (int i = 0; i < visibleTileCount; i++)
        {
            ecs_entity_t e = visibleTiles[i];

            const Position     *p = ecs_get(world, e, Position);
            const TextureIndex *t = ecs_get(world, e, TextureIndex);

            DrawTextureRec
            (
                textureAtlas,
                textureRects[t->ID],
                (Vector2){p->x, p->y},
                WHITE
            );
        }
        
        for (int i = 0; i < visibleEntityCount; i++)
        {
            ecs_entity_t e = visibleEntities[i];

            const Position     *p = ecs_get(world, e, Position);
            const TextureIndex *t = ecs_get(world, e, TextureIndex);

            DrawTextureRec
            (
                textureAtlas,
                textureRects[t->ID],
                (Vector2){p->x, p->y},
                WHITE
            );
        }  
        
        EndMode2D();
        
        DrawFPS(10, 10);
        
        const Level *flv = ecs_get(world, activeTester, Level);
        //DrawText(TextFormat("%i", flv->lvl), 100, 100, 20, BLACK);
        
        const Experience *fexp = ecs_get(world, activeTester, Experience);
        //DrawText(TextFormat("%i", fexp->xp), 100, 122, 20, BLACK);
        
        const HP *fhp = ecs_get(world, activeTester, HP);
        //DrawText(TextFormat("%i", fhp->points), 100, 144, 20, BLACK);

        EndDrawing();
        
    }
    
    UnloadTexture(textureAtlas);
    ecs_fini(world);
    CloseWindow();   
    return 0;
}