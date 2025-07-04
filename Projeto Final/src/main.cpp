#include <GL/glut.h>
#include "game.h"
#include "player.h"

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
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
    gluOrtho2D(0, 800, 0, 600);

    initGame();

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
