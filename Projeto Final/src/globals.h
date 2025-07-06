#include "platform.h"
#include "ladder.h"

#define PLATFORM_COUNT 30
extern Platform platforms[PLATFORM_COUNT];

#define LADDER_COUNT (PLATFORM_COUNT)

extern Ladder ladders[LADDER_COUNT];
extern bool playerOnLadder;

extern const float gravity;
const float PLAYER_WIDTH = 32.0f;
const float PLAYER_HEIGHT = 32.0f;

extern float vilaoX, vilaoY;
extern float vilaoDirectionX; // Direção: -1 (esquerda), 1 (direita)

struct Projectile
{
    float x, y;
    float vx, vy;
    bool active;
};

const int MAX_PROJECTILES = 10;
extern Projectile projectiles[MAX_PROJECTILES];
