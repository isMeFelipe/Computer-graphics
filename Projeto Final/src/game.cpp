#include "game.h"
#include "player.h"
#include "globals.h"
#include <GL/glut.h>
#include <cmath>

float laranjaX = 370, laranjaY = 550;

void initScenario() {
    platforms[0] = {50, 100, 700, 20};
    platforms[1] = {100, 200, 600, 20};
    platforms[2] = {150, 300, 500, 20};
    platforms[3] = {200, 400, 400, 20};
    platforms[4] = {250, 500, 300, 20};
}

void updatePlayerPhysics() {
    playerVelocityY -= gravity;
    playerY += playerVelocityY;
    playerOnGround = false;
    
    for (int i = 0; i < 5; ++i) {
        Platform p = platforms[i];

        if (ignorePlatform) continue; // Ignora colisões se o jogador estiver ignorando plataformas (descendo)

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

        if (horizontalOverlap && verticalCollision && playerVelocityY <= 0) {
            // Corrige posição
            playerY = platTop;
            playerVelocityY = 0;
            playerOnGround = true;
        }
    }

    // Colisão com o chão
    if (playerY < 0) {
        playerY = 0;
        playerVelocityY = 0;
        playerOnGround = true;
    }
}

void initGame() {
    initScenario();
    initPlayer();
}

void updateGame() {
    updatePlayerPhysics();
    updatePlayer();  // Atualiza posição lateral e entrada do teclado
}

void renderScenario() {
    glColor3f(0.6f, 0.3f, 0.1f);
    for (int i = 0; i < 5; i++) {
        Platform p = platforms[i];
        glBegin(GL_QUADS);
            glVertex2f(p.x, p.y);
            glVertex2f(p.x + p.width, p.y);
            glVertex2f(p.x + p.width, p.y + p.height);
            glVertex2f(p.x, p.y + p.height);
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
    for (int i = 0; i <= seg; ++i) {
        float ang = 2.0f * 3.1415926f * i / seg;
        glVertex2f(cx + cos(ang) * radius, cy + sin(ang) * radius);
    }
    glEnd();
}

void renderGame() {
    renderScenario();
    renderPlayer();
}

void handleKeyPress(unsigned char key, int x, int y) {
    playerKeyPress(key, x, y);
}

void handleKeyRelease(unsigned char key, int x, int y) {
    playerKeyRelease(key, x, y);
}

