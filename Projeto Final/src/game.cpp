#include "game.h"
#include "player.h"
#include "globals.h"

#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <cstdlib> // Para rand(), srand()

// Posição da laranja
float laranjaX = 370, laranjaY = 550;

// Prototipação
float getRandomLadderXPosition(const std::vector<float> &existingXs, float minDistance);

// =========================
// Inicialização do cenário
// =========================
void initScenario()
{
    std::srand(static_cast<unsigned int>(time(0)));

    float platformWidth = 800.0f;
    float gapY = 80;
    float minDistance = 100.0f;
    std::vector<float> usedLadderXs;

    // Cria plataformas de ponta a ponta
    for (int i = 0; i < PLATFORM_COUNT; ++i)
    {
        float x = 0;
        float y = 100 + i * gapY;
        platforms[i] = {x, y, platformWidth, 20};
    }

    // Cria escadas
    for (int i = 0; i < LADDER_COUNT; ++i)
    {
        Platform lower = (i == 0) ? Platform{0, 0, 0, 0} : platforms[i - 1];
        Platform upper = platforms[i];

        float ladderX = getRandomLadderXPosition(usedLadderXs, minDistance);
        usedLadderXs.push_back(ladderX);

        float ladderY = (i == 0) ? 0 : lower.y + lower.height;
        float ladderHeight = upper.y - ladderY + PLAYER_HEIGHT;

        ladders[i] = {ladderX, ladderY, 20, ladderHeight};
    }
}

float getRandomLadderXPosition(const std::vector<float> &existingXs, float minDistance)
{
    int margin = 40;
    int minX = margin;
    int maxX = 800 - margin - 20;

    for (int attempt = 0; attempt < 100; ++attempt)
    {
        float candidateX = minX + std::rand() % (maxX - minX + 1);
        bool tooClose = false;

        for (float x : existingXs)
        {
            if (std::fabs(candidateX - x) < minDistance)
            {
                tooClose = true;
                break;
            }
        }

        if (!tooClose)
            return candidateX;
    }

    // Fallback se não encontrar posição válida
    return minX + std::rand() % (maxX - minX + 1);
}

// ============================
// Atualização da física do jogador
// ============================
void updatePlayerPhysics()
{
    playerVelocityY -= gravity;
    playerY += playerVelocityY;
    playerOnGround = false;

    for (int i = 0; i < PLATFORM_COUNT; ++i)
    {
        if (ignorePlatform)
            continue;

        Platform p = platforms[i];

        float playerBottom = playerY;
        float playerTop = playerY + PLAYER_HEIGHT;
        float playerLeft = playerX;
        float playerRight = playerX + PLAYER_WIDTH;

        float platTop = p.y + p.height;
        float platLeft = p.x;
        float platRight = p.x + p.width;

        bool horizontalOverlap = playerRight > platLeft && playerLeft < platRight;
        bool verticalCollision = playerBottom <= platTop && playerBottom >= platTop - std::fabs(playerVelocityY);

        if (horizontalOverlap && verticalCollision && playerVelocityY <= 0)
        {
            playerY = platTop;
            playerVelocityY = 0;
            playerOnGround = true;
        }
    }

    // Colisão com o chão
    if (playerY < 0)
    {
        playerY = 0;
        playerVelocityY = 0;
        playerOnGround = true;
    }
}

void checkLadderCollision()
{
    playerOnLadder = false;

    float playerCenterX = playerX + PLAYER_WIDTH / 2;
    float playerBottom = playerY;
    float playerTop = playerY + PLAYER_HEIGHT;

    for (int i = 0; i < LADDER_COUNT; ++i)
    {
        Ladder l = ladders[i];

        bool inX = playerCenterX >= l.x && playerCenterX <= l.x + l.width;
        bool inY = playerBottom <= l.y + l.height && playerTop >= l.y;

        if (inX && inY)
        {
            playerOnLadder = true;
            break;
        }
    }
}

// ======================
// Inicialização e update
// ======================
void initGame()
{
    initScenario();
    initPlayer();
    loadPlayerTexture();
}

void updateGame()
{
    checkLadderCollision();

    if (!playerOnLadder)
        updatePlayerPhysics();

    updatePlayer();
}

// ===============
// Renderização
// ===============
void renderScenario()
{
    // Plataformas
    glColor3f(0.6f, 0.3f, 0.1f);
    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        Platform p = platforms[i];
        glBegin(GL_QUADS);
        glVertex2f(p.x, p.y);
        glVertex2f(p.x + p.width, p.y);
        glVertex2f(p.x + p.width, p.y + p.height);
        glVertex2f(p.x, p.y + p.height);
        glEnd();
    }

    // Escadas
    glColor3f(0.7f, 0.7f, 1.0f);
    for (int i = 0; i < LADDER_COUNT; ++i)
    {
        Ladder l = ladders[i];
        glBegin(GL_QUADS);
        glVertex2f(l.x, l.y);
        glVertex2f(l.x + l.width, l.y);
        glVertex2f(l.x + l.width, l.y + l.height);
        glVertex2f(l.x, l.y + l.height);
        glEnd();
    }

    // Laranja
    glColor3f(1.0f, 0.5f, 0.0f);
    float cx = laranjaX + 16;
    float cy = laranjaY + 16;
    float radius = 16;
    int seg = 30;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= seg; ++i)
    {
        float ang = 2.0f * 3.1415926f * i / seg;
        glVertex2f(cx + cos(ang) * radius, cy + sin(ang) * radius);
    }
    glEnd();
}

void renderGame()
{
    renderScenario();
    renderPlayer();
}

// ===============
// Entrada de Teclado
// ===============
void handleKeyPress(unsigned char key, int x, int y)
{
    playerKeyPress(key, x, y);
}

void handleKeyRelease(unsigned char key, int x, int y)
{
    playerKeyRelease(key, x, y);
}
