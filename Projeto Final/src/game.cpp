#include "game.h"
#include "player.h"
#include "globals.h"
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <vector>

float laranjaX = 370, laranjaY = 550;
float getRandomLadderXPosition(const std::vector<float> &existingXs, float minDistance);

void initScenario()
{
    std::srand(static_cast<unsigned int>(time(0)));

    float platformWidth = 800.0f;
    float gapY = 80;

    // Cria plataformas de ponta a ponta
    for (int i = 0; i < PLATFORM_COUNT; ++i)
    {
        float x = 0;
        float y = 100 + i * gapY;
        platforms[i] = {x, y, platformWidth, 20};
    }

    std::vector<float> usedLadderXs;
    float minDistance = 100.0f;

    // Escada do chão para a primeira plataforma
    {
        Platform first = platforms[0];
        float ladderX = getRandomLadderXPosition(usedLadderXs, minDistance);
        usedLadderXs.push_back(ladderX);

        float ladderY = 0;
        float ladderHeight = first.y - ladderY + PLAYER_HEIGHT;

        ladders[0] = {ladderX, ladderY, 20, ladderHeight};
    }

    // Escadas entre plataformas
    for (int i = 1; i < LADDER_COUNT; ++i)
    {
        Platform lower = platforms[i - 1];
        Platform upper = platforms[i];

        float ladderX = getRandomLadderXPosition(usedLadderXs, minDistance);
        usedLadderXs.push_back(ladderX);

        float ladderY = lower.y + lower.height;
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

    // Se não conseguir encontrar posição válida após 100 tentativas, retorna mesmo assim
    return minX + std::rand() % (maxX - minX + 1);
}

void updatePlayerPhysics()
{
    playerVelocityY -= gravity;
    playerY += playerVelocityY;
    playerOnGround = false;

    for (int i = 0; i < 5; ++i)
    {
        Platform p = platforms[i];

        if (ignorePlatform)
            continue; // Ignora colisões se o jogador estiver ignorando plataformas (descendo)

        // Coordenadas do jogador
        float playerBottom = playerY;
        float playerTop = playerY + PLAYER_HEIGHT;
        float playerLeft = playerX;
        float playerRight = playerX + PLAYER_WIDTH;

        // Coordenadas da plataforma
        float platTop = p.y + p.height;
        float platLeft = p.x;
        float platRight = p.x + p.width;

        // Verifica se o jogador caiu sobre a plataforma
        bool horizontalOverlap = playerRight > platLeft && playerLeft < platRight;
        bool verticalCollision = playerBottom <= platTop && playerBottom >= platTop - fabs(playerVelocityY);

        if (horizontalOverlap && verticalCollision && playerVelocityY <= 0)
        {
            // Corrige posição
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

    updatePlayer(); // Atualiza posição lateral e entrada do teclado
}

void renderScenario()
{
    glColor3f(0.6f, 0.3f, 0.1f);
    for (int i = 0; i < 5; i++)
    {
        Platform p = platforms[i];
        glBegin(GL_QUADS);
        glVertex2f(p.x, p.y);
        glVertex2f(p.x + p.width, p.y);
        glVertex2f(p.x + p.width, p.y + p.height);
        glVertex2f(p.x, p.y + p.height);
        glEnd();
    }

    // Desenhar escadas
    glColor3f(0.7f, 0.7f, 1.0f); // cor azul clara
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

void handleKeyPress(unsigned char key, int x, int y)
{
    playerKeyPress(key, x, y);
}

void handleKeyRelease(unsigned char key, int x, int y)
{
    playerKeyRelease(key, x, y);
}
