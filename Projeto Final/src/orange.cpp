#include "globals.h"
#include "player.h"

void initOrange()
{
    orangeX = vilaoX + 50; // ajusta conforme necessidade
    orangeY = vilaoY;
}

void loadOrangeTexture()
{
    orangeTextureID = loadTexture("./assets/textures/orange.png");
}

void renderOrange()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, orangeTextureID);
    glColor3f(1, 1, 1);

    float size = 96;

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(orangeX, orangeY);
    glTexCoord2f(1, 0); glVertex2f(orangeX + size, orangeY);
    glTexCoord2f(1, 1); glVertex2f(orangeX + size, orangeY + size);
    glTexCoord2f(0, 1); glVertex2f(orangeX, orangeY + size);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void checkVictory()
{
    float playerSize = 32.0f;
    float orangeSize = 96.0f;

    if (playerX < orangeX + orangeSize &&
        playerX + playerSize > orangeX &&
        playerY < orangeY + orangeSize &&
        playerY + playerSize > orangeY)
    {
        gameState = 2; // Vitória!
    }
}
