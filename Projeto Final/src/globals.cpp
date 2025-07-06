#include "globals.h"

Platform platforms[PLATFORM_COUNT];
const float gravity = 0.5f;

Ladder ladders[LADDER_COUNT];
bool playerOnLadder = false;

int playerHealth = 5;
GLuint heartFullTextureID = 0;
GLuint heartEmptyTextureID = 0;

int invulnerabilityFrames = 0;
