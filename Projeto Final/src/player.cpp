#include "player.h"
#include "globals.h"
#include "game.h"
#include <GL/glut.h>

float playerX = 100;
float playerY = 100;
float playerVelocityY = 0;
float playerVelocityX = 0;
float moveSpeed = 5.0f;

bool playerOnGround = false;

bool ignorePlatform = false;
int ignoreTimer = 0;

// keys
bool keyLeftPressed = false;
bool keyRightPressed = false;

void initPlayer()
{
    playerX = 100;
    playerY = 100;
    playerVelocityY = 0;
    playerOnGround = false;
}

void updatePlayer()
{
    // Aplica velocidade horizontal
    playerX += playerVelocityX;

    // Limita o jogador dentro da tela (supondo largura 800)
    if (playerX < 0)
        playerX = 0;
    if (playerX > 768)
        playerX = 768; // considerando player width 32

    // Aplica gravidade na velocidade vertical
    playerVelocityY -= gravity;
    playerY += playerVelocityY;

    playerOnGround = false;

    if (ignoreTimer > 0)
    {
        ignoreTimer--;
        if (ignoreTimer == 0)
        {
            ignorePlatform = false;
        }
    }

    // Colisão com plataformas (se não estiver ignorando colisão)
    if (!ignorePlatform)
    {
        for (int i = 0; i < 5; i++)
        {
            Platform p = platforms[i];

            bool insideX = (playerX + 32 > p.x) && (playerX < p.x + p.width);
            bool hitFromAbove = (playerY <= p.y + p.height) && (playerY >= p.y + p.height - 10);
            if (insideX && hitFromAbove && playerVelocityY <= 0)
            {
                playerY = p.y + p.height;
                playerVelocityY = 0;
                playerOnGround = true;
            }
        }
    }

    if (keyLeftPressed)
    {
        playerVelocityX = -moveSpeed;
    }
    else if (keyRightPressed)
    {
        playerVelocityX = moveSpeed;
    }
    else
    {
        playerVelocityX = 0;
    }

    playerX += playerVelocityX;

    // Limita o chão (y=0)
    if (playerY < 0)
    {
        playerY = 0;
        playerVelocityY = 0;
        playerOnGround = true;
    }
}

void renderPlayer()
{
    glColor3f(1, 1, 1); // branco
    glBegin(GL_QUADS);
    glVertex2f(playerX, playerY);
    glVertex2f(playerX + PLAYER_WIDTH, playerY);
    glVertex2f(playerX + PLAYER_WIDTH, playerY + PLAYER_HEIGHT);
    glVertex2f(playerX, playerY + PLAYER_HEIGHT);
    glEnd();
}

void playerKeyPress(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A')
        keyLeftPressed = true;
    if (key == 'd' || key == 'D')
        keyRightPressed = true;
    if (key == 'w' || key == 'W')
    {
        if (playerOnGround)
        {
            playerVelocityY = 16.0f; // Exemplo de pulo
            playerOnGround = false;
        }
    }
    if (key == 's' || key == 'S')
    {
        // Desce da plataforma
        ignorePlatform = true;
        playerOnGround = false;
    }
}

void playerKeyRelease(unsigned char key, int x, int y)
{

    if (key == 'a' || key == 'A')
    {
        keyLeftPressed = false;
    }
    if (key == 'd' || key == 'D')
    {
        keyRightPressed = false;
    }
    if (key == 's' || key == 'S')
    {
        ignorePlatform = false;
        ignoreTimer = 30; // Ignora colisão por 30 frames
    }
}

void playerSpecialPress(int key)
{
    // vazio por enquanto
}
