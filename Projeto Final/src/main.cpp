#include <GL/glut.h>
#include "game.h"
#include "player.h"
#include "vilao.h"
#include "globals.h"

int windowWidth = 800;
int windowHeight = 600;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // --- Câmera principal (seguindo jogador dinamicamente) ---
    glViewport(0, 0, windowWidth, windowHeight); // use variáveis globais atualizadas no reshape

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float cameraY = playerY - 300;
    if (cameraY < 0)
        cameraY = 0;

    gluOrtho2D(0, 800, cameraY, cameraY + 600); // Área visível do jogo (ajustável)

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderGame();

    // --- Mini-câmera do vilão ---

    int miniWidth = 200;
    int miniHeight = 150;

    int miniX = windowWidth - miniWidth;
    int miniY = windowHeight - miniHeight;

    glViewport(miniX, miniY, miniWidth, miniHeight);

    glEnable(GL_SCISSOR_TEST);
    glScissor(miniX, miniY, miniWidth, miniHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // Salva a projeção principal
    glLoadIdentity();

    gluOrtho2D(vilaoX - 150, vilaoX + 150, vilaoY - 50, vilaoY + 250);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderVilao();
    renderProjectiles();
    renderPreview();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // Restaura a projeção principal
    glMatrixMode(GL_MODELVIEW);

    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    glutSwapBuffers();
}

void update(int value)
{
    updateGame();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void specialKeyPress(int key, int x, int y)
{
    handleVilaoSpecialKeyPress(key);
}

void reshape(int width, int height)
{
    if (height == 0)
        height = 1;

    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);

    // Não defina gluOrtho2D aqui — será feito dinamicamente no display()

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // apenas valor neutro temporário

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Donkey Kong Laranja");
    glutReshapeFunc(reshape);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // REGISTROS DE INPUT
    glutKeyboardFunc(playerKeyPress);
    glutKeyboardUpFunc(playerKeyRelease);

    glutSpecialFunc(specialKeyPress);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    initGame();
    loadVilaoTexture("../assets/textures/villian_idle1.png");

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
