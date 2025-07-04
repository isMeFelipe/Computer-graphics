#include <GL/glut.h>
#include "game.h"
#include "player.h"

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Câmera segue o jogador verticalmente
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float cameraY = playerY - 300; // Mantém jogador no centro vertical da tela
    if (cameraY < 0)
        cameraY = 0;

    gluOrtho2D(0, 800, cameraY, cameraY + 600); // Janela de visualização móvel

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderGame();
    glutSwapBuffers();
}

void update(int value)
{
    updateGame();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Donkey Kong Laranja");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // REGISTROS DE INPUT
    glutKeyboardFunc(playerKeyPress);
    glutKeyboardUpFunc(playerKeyRelease);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1600, 0, 1200);

    initGame();

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
