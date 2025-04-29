/*
 * Computacao Grafica
 * Codigo Exemplo: Braco Robotico com Mão e Garra usando GLUT/OpenGL
 * Autor: Prof. Laurindo de Sousa Britto Neto
 * Modificações: [Seu Nome]
 */

// Bibliotecas utilizadas pelo OpenGL
#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#define ESC 27

/*
 * Declaracao de constantes e variaveis
 */
static int ombro = 0, cotovelo = 0, mao = 0;
static int dedo_indicador = 0, dedo_polegar = 0, dedo_anular = 0;
static int rotacao_y = 0;

/*
 * Declaracoes antecipadas (forward) das funcoes (assinaturas)
 */
void init(void);
void keyboard(unsigned char key, int x, int y);
void display(void);
void reshape(int w, int h);
void desenhaBraco();
void desenhaMao();

// Funcao principal
int main(int argc, char** argv) {
    // inicia o GLUT
    glutInit(&argc, argv);

    // inicia o display usando RGB, double-buffering e z-buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Computacao Grafica: Braco Robotico com Mao e Garra");

    // Funcao com alguns comandos para a inicializacao do OpenGL
    init();

    // define as funcoes de callback
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}

// Funcao com alguns comandos para a inicializacao do OpenGL
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Limpa a tela com a cor branca
    glEnable(GL_DEPTH_TEST); // Habilita o algoritmo Z-Buffer
    glShadeModel(GL_SMOOTH); // Habilita o sombreamento suave
}
void reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    gluPerspective(60, (float)w / (float)h, 1.0, 20.0); // (angulo, aspecto, ponto_proximo, ponto distante)
    gluLookAt(0.0, 2.0, 10.0,  // posicao da camera (olho)
              0.0, 0.0, 0.0,   // direcao da camera (geralmente para centro da cena)
              0.0, 1.0, 0.0);  // sentido ou orientacao da camera (de cabeca para cima)
    
    glMatrixMode(GL_MODELVIEW);
}

// CONTROLES *********************************
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'o': ombro = (ombro - 5) % 360; break; // sentido horario
        case 'O': ombro = (ombro + 5) % 360; break; // sentido anti-horario
        case 'c': cotovelo = (cotovelo - 5) % 360; break; // sentido horario
        case 'C': cotovelo = (cotovelo + 5) % 360; break; // sentido anti-horario
        case 'm': mao = (mao - 5) % 360; break; // sentido horario
        case 'M': mao = (mao + 5) % 360; break; // sentido anti-horario
        case 'i': dedo_indicador = (dedo_indicador - 5) % 360; break; // fecha indicador
        case 'I': dedo_indicador = (dedo_indicador + 5) % 360; break; // abre indicador
        case 'p': dedo_polegar = (dedo_polegar - 5) % 360; break; // fecha polegar
        case 'P': dedo_polegar = (dedo_polegar + 5) % 360; break; // abre polegar
        case 'a': dedo_anular = (dedo_anular - 5) % 360; break; // fecha anular
        case 'A': dedo_anular = (dedo_anular + 5) % 360; break; // abre anular
        case 'y': rotacao_y = (rotacao_y - 5) % 360; break; // rotação em torno do eixo Y
        case 'Y': rotacao_y = (rotacao_y + 5) % 360; break; // rotação em torno do eixo Y
        case ESC: exit(EXIT_SUCCESS); break; // sai do programa
    }
    glutPostRedisplay();
}


void desenhaBraco() {
    // Base
    glPushMatrix();
        glColor3f(0.8, 0.8, 0.8); // Cinza claro
        glTranslatef(0.0, -2.5, 0.0);
        glScalef(1.5, 0.5, 1.5);
        glutSolidCube(1.0);
    glPopMatrix();

    // Movimento do braco completo (rotação em Y)
    glPushMatrix();
        glRotatef((GLfloat)rotacao_y, 0.0, 1.0, 0.0);
        
        // Ombro
        glPushMatrix();
            glColor3f(0.2, 0.2, 0.8); // Azul
            glTranslatef(0.0, -1.5, 0.0);
            glutSolidSphere(0.5, 30, 30);
        glPopMatrix();

        // Braco superior
        glPushMatrix();
            glRotatef((GLfloat)ombro, 0.0, 0.0, 1.0);
            
            // Braco
            glPushMatrix();
                glColor3f(0.8, 0.2, 0.2); // Vermelho
                glTranslatef(0.0, -0.5, 0.0);
                glScalef(0.5, 2.0, 0.5);
                glutSolidCube(1.0);
            glPopMatrix();

            // Cotovelo
            glPushMatrix();
                glColor3f(0.2, 0.8, 0.2); // Verde
                glTranslatef(0.0, 0.5, 0.0);
                glutSolidSphere(0.4, 30, 30);
            glPopMatrix();

            // Antebraco
            glPushMatrix();
                glTranslatef(0.0, 0.5, 0.0);
                glRotatef((GLfloat)cotovelo, 0.0, 0.0, 1.0);
                
                // Antebraco
                glPushMatrix();
                    glColor3f(0.8, 0.8, 0.2); // Amarelo
                    glTranslatef(0.0, 0.5, 0.0);
                    glScalef(0.4, 2.0, 0.4);
                    glutSolidCube(1.0);
                glPopMatrix();

                // Punho
                glPushMatrix();
                    glColor3f(0.8, 0.2, 0.8); // Magenta
                    glTranslatef(0.0, 1.5, 0.0);
                    glutSolidSphere(0.3, 30, 30);
                glPopMatrix();

                // Mão
                glPushMatrix();
                    glTranslatef(0.0, 1.5, 0.0);
                    glRotatef((GLfloat)mao, 0.0, 0.0, 1.0);
                    desenhaMao();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void desenhaMao() {
    // Palma da mão
    glPushMatrix();
        glColor3f(0.5, 0.5, 0.0); // Amarelo escuro
        glScalef(0.6, 0.3, 1.0);
        glutSolidCube(1.0);
    glPopMatrix();

    // Dedo Indicador
    glPushMatrix();
        glTranslatef(0.2, 0.0, 0.3);
        glRotatef((GLfloat)dedo_indicador, 0.0, 0.0, 1.0);
        
        // Primeira falange
        glPushMatrix();
            glColor3f(0.7, 0.7, 0.7);
            glTranslatef(0.15, 0.0, 0.0);
            glScalef(0.3, 0.1, 0.1);
            glutSolidCube(1.0);
        glPopMatrix();
        
        // Segunda falange
        glPushMatrix();
            glTranslatef(0.3, 0.0, 0.0);
            glRotatef((GLfloat)dedo_indicador/2, 0.0, 0.0, 1.0);
            glColor3f(0.6, 0.6, 0.6);
            glTranslatef(0.15, 0.0, 0.0);
            glScalef(0.3, 0.1, 0.1);
            glutSolidCube(1.0);
        glPopMatrix();
    glPopMatrix();

    // Dedo Anular
    glPushMatrix();
        glTranslatef(0.2, 0.0, -0.3);
        glRotatef((GLfloat)dedo_anular, 0.0, 0.0, 1.0);
        
        // Primeira falange
        glPushMatrix();
            glColor3f(0.7, 0.7, 0.7);
            glTranslatef(0.15, 0.0, 0.0);
            glScalef(0.3, 0.1, 0.1);
            glutSolidCube(1.0);
        glPopMatrix();
        
        // Segunda falange
        glPushMatrix();
            glTranslatef(0.3, 0.0, 0.0);
            glRotatef((GLfloat)dedo_anular/2, 0.0, 0.0, 1.0);
            glColor3f(0.6, 0.6, 0.6);
            glTranslatef(0.15, 0.0, 0.0);
            glScalef(0.3, 0.1, 0.1);
            glutSolidCube(1.0);
        glPopMatrix();
    glPopMatrix();

    // Dedo Polegar
    glPushMatrix();
        glTranslatef(-0.2, 0.0, 0.5);
        glRotatef((GLfloat)dedo_polegar, 1.0, 0.0, 0.0);
        
        // Falange do polegar
        glPushMatrix();
            glColor3f(0.7, 0.7, 0.7);
            glTranslatef(0.0, 0.0, -0.15);
            glScalef(0.1, 0.1, 0.3);
            glutSolidCube(1.0);
        glPopMatrix();
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o Buffer de Cores e Profundidade
    glLoadIdentity();
    
    desenhaBraco();

    glutSwapBuffers();
}