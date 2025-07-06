#include "vilao.h"
#include "player.h"
#include "platform.h"

#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include "stb_image.h"

float vilaoX = 400;
float vilaoY = 1100; // topo da última plataforma
int vilaoDirectionX = 1;

float projectileOffsetY = 0;
const float maxOffset = 50.0f;
const float minOffset = 5.0f;

float previewX = vilaoX;
float previewY = vilaoY + projectileOffsetY;

GLuint vilaoTextureID;

int lastLaunchTime = 0;
const int cooldownMs = 500;

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
    vilaoY = 1100;
    vilaoDirectionX = 1;
    projectiles.clear();
}

void updateVilao()
{
    // Vilão parado, apenas direção muda via input
}

void renderVilao()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, vilaoTextureID);
    glColor3f(1, 1, 1);

    float scale = 0.3f;
    float w = 500 * scale / 2.0f;
    float h = 500 * scale / 2.0f;

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(vilaoX - w, vilaoY);
    glTexCoord2f(1, 0);
    glVertex2f(vilaoX + w, vilaoY);
    glTexCoord2f(1, 1);
    glVertex2f(vilaoX + w, vilaoY + 2 * h);
    glTexCoord2f(0, 1);
    glVertex2f(vilaoX - w, vilaoY + 2 * h);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void launchProjectile()
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    if (now - lastLaunchTime < cooldownMs)
        return;
    lastLaunchTime = now;

    Projectile p;
    p.x = vilaoX;
    p.y = vilaoY + projectileOffsetY;
    p.vx = vilaoDirectionX * 5.0f;
    p.vy = 0;
    p.active = true;
    projectiles.push_back(p);
}

void updatePreview()
{
    previewX = vilaoX + vilaoDirectionX * 20;
    previewY = vilaoY + projectileOffsetY;
}

void renderPreview()
{
    glColor4f(0, 0, 1, 1);
    float size = 5.0f;
    glBegin(GL_QUADS);
    glVertex2f(previewX - size, previewY - size);
    glVertex2f(previewX + size, previewY - size);
    glVertex2f(previewX + size, previewY + size);
    glVertex2f(previewX - size, previewY + size);
    glEnd();
}

void updateProjectiles()
{
    for (auto &p : projectiles)
    {
        if (!p.active)
            continue;

        p.x += p.vx;

        // Rebater nas paredes e descer plataforma
        if (p.x < 0 || p.x > 800)
        {
            p.vx *= -1;
            p.y -= 60;
        }

        if (p.y < 0)
            p.active = false;
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const Projectile &p)
                       { return !p.active; }),
        projectiles.end());
}

void renderProjectiles()
{
    for (const auto &p : projectiles)
    {
        if (!p.active)
            continue;

        glColor3f(1, 0.3f, 0.3f);
        glBegin(GL_QUADS);
        glVertex2f(p.x, p.y);
        glVertex2f(p.x + 10, p.y);
        glVertex2f(p.x + 10, p.y + 10);
        glVertex2f(p.x, p.y + 10);
        glEnd();
    }
}

void handleVilaoSpecialKeyPress(int key)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        vilaoDirectionX = -1;
        break;
    case GLUT_KEY_RIGHT:
        vilaoDirectionX = 1;
        break;
    case GLUT_KEY_UP:
        projectileOffsetY += 5.0f;
        if (projectileOffsetY > maxOffset)
            projectileOffsetY = maxOffset;
        break;
    case GLUT_KEY_DOWN:
        projectileOffsetY -= 5.0f;
        if (projectileOffsetY < minOffset)
            projectileOffsetY = minOffset;
        break;
    case GLUT_KEY_F1:
        launchProjectile();
        break;
    }
    updatePreview();
}

void loadVilaoTexture(const char *filename)
{
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 4);

    if (!data)
    {
        printf("Falha ao carregar textura %s\n", filename);
        return;
    }

    glGenTextures(1, &vilaoTextureID);
    glBindTexture(GL_TEXTURE_2D, vilaoTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}
