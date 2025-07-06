#include "vilao.h"
#include "player.h"
#include "platform.h"

#include <GL/glut.h>
#include <vector>
#include <algorithm>

float vilaoX = 400;
float vilaoY = 1100; // topo da tela
int vilaoDirectionX = 1;

float projectileOffsetY = 0; // Variação da altura do lançamento
const float maxOffset = 100.0f;
const float minOffset = -100.0f;

struct Projectile
{
    float x, y;
    float vx, vy;
    bool active;
};

std::vector<Projectile> projectiles;

void initVilao()
{
    vilaoX = 400;
    vilaoY = 1100; // última plataforma
    vilaoDirectionX = 1;
    projectiles.clear();
}

void updateVilao()
{
    // O vilão não se move, só muda direção
}

void renderVilao()
{
    glColor3f(1.0f, 0.3f, 0.3f); // vermelho
    glBegin(GL_QUADS);
    glVertex2f(vilaoX - 25, vilaoY);
    glVertex2f(vilaoX + 25, vilaoY);
    glVertex2f(vilaoX + 25, vilaoY + 50);
    glVertex2f(vilaoX - 25, vilaoY + 50);
    glEnd();
}

void launchProjectile() {
    Projectile p;
    p.x = vilaoX;
    p.y = vilaoY + projectileOffsetY; // usa altura ajustável
    p.vx = vilaoDirectionX * 5.0f;
    p.vy = 0;
    p.active = true;
    projectiles.push_back(p);
}


void updateProjectiles()
{
    for (int i = 0; i < projectiles.size(); ++i)
    {
        if (!projectiles[i].active)
            continue;

        projectiles[i].x += projectiles[i].vx;

        // Rebater nas paredes
        if (projectiles[i].x < 0 || projectiles[i].x > 800)
        {
            projectiles[i].vx *= -1;
            projectiles[i].y -= 60; // Desce uma plataforma
        }

        // Verifica se saiu da tela (ou foi muito para baixo)
        if (projectiles[i].y < 0)
        {
            projectiles[i].active = false;
        }
    }

    // Remove projéteis inativos
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile &p)
                       { return !p.active; }),
        projectiles.end());
}

void renderProjectiles()
{
    for (const auto &p : projectiles)
    {
        if (!p.active)
            continue;

        glColor3f(1.0f, 0.3f, 0.3f); // Vermelho claro
        glBegin(GL_QUADS);
        glVertex2f(p.x, p.y);
        glVertex2f(p.x + 10, p.y);
        glVertex2f(p.x + 10, p.y + 10);
        glVertex2f(p.x, p.y + 10);
        glEnd();
    }
}

void handleVilaoSpecialKeyPress(int key) {
    switch (key) {
        case GLUT_KEY_LEFT:
            vilaoDirectionX = -1;
            break;
        case GLUT_KEY_RIGHT:
            vilaoDirectionX = 1;
            break;
        case GLUT_KEY_UP:
            projectileOffsetY += 5.0f;
            if (projectileOffsetY > maxOffset) projectileOffsetY = maxOffset;
            break;
        case GLUT_KEY_DOWN:
            projectileOffsetY -= 5.0f;
            if (projectileOffsetY < minOffset) projectileOffsetY = minOffset;
            break;
        case GLUT_KEY_F1:
            launchProjectile();
            break;
    }
}
