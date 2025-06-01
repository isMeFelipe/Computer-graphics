/*
 * Computacao Grafica
 * Codigo Exemplo: Rasterizacao de Segmentos de Reta com GLUT/OpenGL
 * Autor: Prof. Laurindo de Sousa Britto Neto
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

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <forward_list>
#include "glut_text.h"
#include <list>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Variaveis Globais
#define ESC 27
#define ENTER 13
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Enumeracao com os tipos de formas geometricas
enum tipo_forma
{
    LIN = 1,
    TRI,
    RET,
    POL,
    CIR,
    PINTAR
}; // Linha, Triangulo, Retangulo Poligono, Circulo

// Verifica se foi realizado o primeiro clique do mouse
bool click1 = false;

// Coordenadas da posicao atual do mouse
int m_x, m_y;

// Coordenadas do primeiro clique e do segundo clique do mouse
int x_1, y_1, x_2, y_2;

// Indica o tipo de forma geometrica ativa para desenhar
int modo = LIN;

// Largura e altura da janela
int width = 512, height = 512;

int clickCount = 0;
int x_clicks[3], y_clicks[3];

bool preenchimentoAtivo = false; // definido pela tecla p/P

// Definicao de vertice
struct vertice
{
    int x;
    int y;
};

std::vector<vertice> tempVertices;
bool drawingPolygon = false;

// Definicao das formas geometricas

struct forma
{
    int tipo;
    std::list<vertice> v;
    float cor[3];
};

// Lista encadeada de formas geometricas
forward_list<forma> formas;

// Funcao para armazenar um vertice na forma do inicio da lista de formas geometricas
// Armazena sempre no inicio da lista
void pushVertice(int x, int y)
{
    vertice v;
    v.x = x;
    v.y = y;
    formas.front().v.push_back(v);
}

// Funcao para armazenar uma forma geometrica na lista de formas
// Armazena sempre no inicio da lista
void pushForma(int tipo) {
    forma f;
    f.tipo = tipo;
    f.cor[0] = 0.0; // R
    f.cor[1] = 0.0; // G
    f.cor[2] = 0.0; // B
    formas.push_front(f);
}

void pushCirculo(int xc, int yc, int radius) {
    pushForma(CIR);
    pushVertice(xc, yc);          // Centro do círculo
    pushVertice(xc + radius, yc); // Ponto para armazenar o raio
    formas.front().cor[0] = 0.0;  // Inicializa com preto
    formas.front().cor[1] = 0.0;
    formas.front().cor[2] = 0.0;
}

// Fucao para armazenar uma Linha na lista de formas geometricas
void pushLinha(int x1, int y1, int x2, int y2)
{
    pushForma(LIN);
    pushVertice(x1, y1);
    pushVertice(x2, y2);
}

/*
 * Declaracoes antecipadas (forward) das funcoes (assinaturas das funcoes)
 */
void init(void);
void reshape(int w, int h);
void display(void);
void menu_popup(int value);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mousePassiveMotion(int x, int y);
void drawPixel(int x, int y);
// Funcao que percorre a lista de formas geometricas, desenhando-as na tela
void drawFormas();
// Funcao que implementa o Algoritmo Imediato para rasterizacao de segmentos de retas
void retaImediata(double x1, double y1, double x2, double y2);
void retaBresenham(int x1, int y1, int x2, int y2);
void pushRetangulo(int x1, int y1, int x2, int y2);
void retaRetanguloBresenham(int x1, int y1, int x2, int y2);
void pushTriangulo(int x1, int y1, int x2, int y2, int x3, int y3);
void drawPoligono(const std::list<vertice> &vertices);
void transladarForma(forma &f, int dx, int dy);
void escalarForma(forma &f, float sx, float sy);
void cisalharForma(forma &f, float shx, float shy);
void refletirForma(forma &f, bool refletirX, bool refletirY);
void rotacionarForma(forma &f, float angulo_graus);
void drawCircleBresenham(int xc, int yc, int radius);
void pushCirculo(int xc, int yc, int radius);
void fillPolygon(const std::list<vertice> &vertices);
void fillRectangle(int x1, int y1, int x2, int y2);
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
bool pontoNaLinha(int px, int py, int x1, int y1, int x2, int y2, int tolerancia = 5);
bool pontoNoRetangulo(int px, int py, int x1, int y1, int x2, int y2);
bool pontoNoTriangulo(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3);
bool pontoNoPoligono(int px, int py, const std::list<vertice> &vertices);
bool pontoNoCirculo(int px, int py, int xc, int yc, int radius);
/*
 * Funcao principal
 */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                         // Passagens de parametro C para o glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   // Selecao do Modo do Display e do Sistema de cor
    glutInitWindowSize(width, height);             // Tamanho da janela do OpenGL
    glutInitWindowPosition(100, 100);              // Posicao inicial da janela do OpenGL
    glutCreateWindow("Computacao Grafica: Paint"); // Da nome para uma janela OpenGL
    init();                                        // Chama funcao init();
    glutReshapeFunc(reshape);                      // funcao callback para redesenhar a tela
    glutKeyboardFunc(keyboard);                    // funcao callback do teclado
    glutMouseFunc(mouse);                          // funcao callback do mouse
    glutPassiveMotionFunc(mousePassiveMotion);     // fucao callback do movimento passivo do mouse
    glutDisplayFunc(display);                      // funcao callback de desenho

    // Define o menu pop-up
    glutCreateMenu(menu_popup);
    glutAddMenuEntry("Linha", LIN);
    glutAddMenuEntry("Retângulo", RET);
    glutAddMenuEntry("Triângulo", TRI);
    glutAddMenuEntry("Polígono", POL);
    glutAddMenuEntry("Círculo", CIR);
    glutAddMenuEntry("Pintar", PINTAR);
    glutAddMenuEntry("Sair", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();      // executa o loop do OpenGL
    return EXIT_SUCCESS; // retorna 0 para o tipo inteiro da funcao main();
}

/*
 * Inicializa alguns parametros do GLUT
 */
void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // Limpa a tela com a cor branca;
}

/*
 * Ajusta a projecao para o redesenho da janela
 */
void reshape(int w, int h)
{
    // Muda para o modo de projecao e reinicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Definindo o Viewport para o tamanho da janela
    glViewport(0, 0, w, h);

    width = w;
    height = h;
    glOrtho(0, w, 0, h, -1, 1);

    // muda para o modo de desenho
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
 * Controla os desenhos na tela
 */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer de cores e reinicia a matriz
    glColor3f(0.0, 0.0, 0.0);     // Seleciona a cor default como preto
    drawFormas();                 // Desenha as formas geometricas da lista
    // Desenha texto com as coordenadas da posicao do mouse
    draw_text_stroke(0, 0, "(" + to_string(m_x) + "," + to_string(m_y) + ")", 0.2);
    glutSwapBuffers(); // manda o OpenGl renderizar as primitivas
}

/*
 * Controla o menu pop-up
 */
void menu_popup(int value) {
    if (value == 0)
        exit(EXIT_SUCCESS);
    modo = value;
    if (modo == PINTAR) {
        click1 = false;
        clickCount = 0;
        tempVertices.clear();
        drawingPolygon = false;
        printf("Modo Pintar ativado. Pontos temporários limpos.\n");
    }
    glutPostRedisplay();
}

/*
 * Controle das teclas comuns do teclado
 */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ESC:
        exit(EXIT_SUCCESS);
        break;
    case ENTER:
    {
        if (modo == POL && drawingPolygon)
        {
            if (tempVertices.size() >= 4)
            {
                pushForma(POL);
                for (size_t i = 0; i < tempVertices.size(); i++)
                {
                    pushVertice(tempVertices[i].x, tempVertices[i].y);
                }
            }
            tempVertices.clear();
            drawingPolygon = false;
            glutPostRedisplay();
        }
        break;
    }
    case 'p':
    case 'P':
    {
        preenchimentoAtivo = !preenchimentoAtivo;
        printf("Preenchimento %s\n", preenchimentoAtivo ? "Ativado" : "Desativado");
        glutPostRedisplay();
        break;
    }
    case 't':
    { // Translação
        if (!formas.empty())
            transladarForma(formas.front(), 20, 20); // desloca a última forma desenhada
        glutPostRedisplay();
        break;
    }
    case 'T':
    { // Translação inversa
        if (!formas.empty())
            transladarForma(formas.front(), -20, -20);
        glutPostRedisplay();
        break;
    }
    case 'e':
    { // Escala
        if (!formas.empty())
            escalarForma(formas.front(), 1.5, 1.5); // aumenta 50%
        glutPostRedisplay();
        break;
    }
    case 'E':
    { // Escala inversa
        if (!formas.empty())
            escalarForma(formas.front(), 1.0 / 1.5, 1.0 / 1.5);
        glutPostRedisplay();
        break;
    }
    case 'c':
    { // Cisalhamento
        if (!formas.empty())
            cisalharForma(formas.front(), 0.5, 0.0); // cisalha na direção x
        glutPostRedisplay();
        break;
    }
    case 'C':
    { // Cisalhamento inverso
        if (!formas.empty())
            cisalharForma(formas.front(), -0.5, 0.0);
        glutPostRedisplay();
        break;
    }
    case 'r':
    { // Rotação
        if (!formas.empty())
            rotacionarForma(formas.front(), 45); // gira 45 graus
        glutPostRedisplay();
        break;
    }
    case 'R':
    { // Rotação inversa
        if (!formas.empty())
            rotacionarForma(formas.front(), -45);
        glutPostRedisplay();
        break;
    }
    case 'x':
    { // Reflexão em X
        if (!formas.empty())
            refletirForma(formas.front(), true, false);
        glutPostRedisplay();
        break;
    }
    case 'X':
    { // Reflexão em X novamente (mesmo que aplicar novamente)
        if (!formas.empty())
            refletirForma(formas.front(), true, false);
        glutPostRedisplay();
        break;
    }
    case 'y':
    { // Reflexão em Y
        if (!formas.empty())
            refletirForma(formas.front(), false, true);
        glutPostRedisplay();
        break;
    }
    case 'Y':
    { // Reflexão em Y novamente
        if (!formas.empty())
            refletirForma(formas.front(), false, true);
        glutPostRedisplay();
        break;
    }
    }
}

/*
 * Controle dos botoes do mouse
 */
void mouse(int button, int state, int x, int y) {
    int mouseX = x;
    int mouseY = height - y - 1;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (modo == PINTAR) {
            bool formaEncontrada = false;
            for (forward_list<forma>::iterator f = formas.begin(); f != formas.end(); ++f) {
                switch (f->tipo) {
                    case LIN: {
                        int i = 0, x[2], y[2];
                        for (auto v = f->v.begin(); v != f->v.end(); ++v, ++i) {
                            x[i] = v->x;
                            y[i] = v->y;
                        }
                        if (pontoNaLinha(mouseX, mouseY, x[0], y[0], x[1], y[1])) {
                            f->cor[0] = 1.0; // Vermelho
                            f->cor[1] = 0.0;
                            f->cor[2] = 0.0;
                            formaEncontrada = true;
                        }
                        break;
                    }
                    case RET: {
                        int i = 0, x[4], y[4];
                        vertice verts[4];
                        for (auto v = f->v.begin(); v != f->v.end() && i < 4; ++v, ++i) {
                            verts[i] = *v;
                        }
                        x[0] = verts[3].x; y[0] = verts[3].y; // inf. esquerdo
                        x[2] = verts[1].x; y[2] = verts[1].y; // sup. direito
                        if (pontoNoRetangulo(mouseX, mouseY, x[0], y[0], x[2], y[2])) {
                            f->cor[0] = 1.0; // Vermelho
                            f->cor[1] = 0.0;
                            f->cor[2] = 0.0;
                            formaEncontrada = true;
                        }
                        break;
                    }
                    case TRI: {
                        int i = 0, x[3], y[3];
                        for (auto v = f->v.begin(); v != f->v.end() && i < 3; ++v, ++i) {
                            x[i] = v->x;
                            y[i] = v->y;
                        }
                        if (pontoNoTriangulo(mouseX, mouseY, x[0], y[0], x[1], y[1], x[2], y[2])) {
                            f->cor[0] = 1.0; // Vermelho
                            f->cor[1] = 0.0;
                            f->cor[2] = 0.0;
                            formaEncontrada = true;
                        }
                        break;
                    }
                    case POL: {
                        if (pontoNoPoligono(mouseX, mouseY, f->v)) {
                            f->cor[0] = 1.0; // Vermelho
                            f->cor[1] = 0.0;
                            f->cor[2] = 0.0;
                            formaEncontrada = true;
                        }
                        break;
                    }
                    case CIR: {
                        auto it = f->v.begin();
                        int xc = it->x;
                        int yc = it->y;
                        ++it;
                        int xr = it->x;
                        int yr = it->y;
                        int radius = (int)sqrt(pow(xr - xc, 2) + pow(yr - yc, 2));
                        if (pontoNoCirculo(mouseX, mouseY, xc, yc, radius)) {
                            f->cor[0] = 1.0; // Vermelho
                            f->cor[1] = 0.0;
                            f->cor[2] = 0.0;
                            formaEncontrada = true;
                        }
                        break;
                    }
                }
                if (formaEncontrada)
                    break; // Para após encontrar a primeira forma
            }
            if (!formaEncontrada) {
                printf("Nenhuma forma encontrada no ponto (%d, %d)\n", mouseX, mouseY);
            }
            glutPostRedisplay();
        }
        else if (modo == LIN) {
            // Código existente para LIN
            if (click1) {
                x_2 = x;
                y_2 = height - y - 1;
                printf("Clique 2 (%d, %d)\n", x_2, y_2);
                pushLinha(x_1, y_1, x_2, y_2);
                click1 = false;
                glutPostRedisplay();
            } else {
                click1 = true;
                x_1 = x;
                y_1 = height - y - 1;
                printf("Clique 1 (%d, %d)\n", x_1, y_1);
            }
        }
        else if (modo == RET) {
            // Código existente para RET
            if (click1) {
                x_2 = x;
                y_2 = height - y - 1;
                printf("Clique 2 (%d, %d)\n", x_2, y_2);
                pushRetangulo(x_1, y_1, x_2, y_2);
                click1 = false;
                glutPostRedisplay();
            } else {
                click1 = true;
                x_1 = x;
                y_1 = height - y - 1;
                printf("Clique 1 (%d, %d)\n", x_1, y_1);
            }
        }
        else if (modo == TRI) {
            // Código existente para TRI
            x_clicks[clickCount] = x;
            y_clicks[clickCount] = height - y - 1;
            printf("Clique %d (%d, %d)\n", clickCount + 1, x_clicks[clickCount], y_clicks[clickCount]);
            clickCount++;
            if (clickCount == 3) {
                pushTriangulo(x_clicks[0], y_clicks[0], x_clicks[1], y_clicks[1], x_clicks[2], y_clicks[2]);
                clickCount = 0;
                glutPostRedisplay();
            }
        }
        else if (modo == POL) {
            // Código existente para POL
            if (!drawingPolygon) {
                drawingPolygon = true;
                tempVertices.clear();
            }
            tempVertices.push_back(vertice{mouseX, mouseY});
            glutPostRedisplay();
        }
        else if (modo == CIR) {
            // Código existente para CIR
            if (click1) {
                x_2 = x;
                y_2 = height - y - 1;
                int radius = (int)sqrt(pow(x_2 - x_1, 2) + pow(y_2 - y_1, 2));
                printf("Clique 2 (%d, %d), raio: %d\n", x_2, y_2, radius);
                pushCirculo(x_1, y_1, radius);
                click1 = false;
                glutPostRedisplay();
            } else {
                click1 = true;
                x_1 = x;
                y_1 = height - y - 1;
                printf("Clique 1 (centro) (%d, %d)\n", x_1, y_1);
            }
        }
    }
}

/*
 * Controle da posicao do cursor do mouse
 */
void mousePassiveMotion(int x, int y)
{
    m_x = x;
    m_y = height - y - 1;
    glutPostRedisplay();
}

/*
 * Funcao para desenhar apenas um pixel na tela
 */
void drawPixel(int x, int y)
{
    glBegin(GL_POINTS); // Seleciona a primitiva GL_POINTS para desenhar
    glVertex2i(x, y);
    glEnd(); // indica o fim do ponto
}

/*
 *Funcao que desenha a lista de formas geometricas
 */
void drawFormas() {
    // Apos o primeiro clique, desenha a reta com a posicao atual do mouse
    if (click1 && modo != PINTAR) {
        glColor3f(0.0, 0.0, 0.0); // Cor padrão (preto) para formas temporárias
        if (modo == CIR) {
            int radius = (int)sqrt(pow(m_x - x_1, 2) + pow(m_y - y_1, 2));
            drawCircleBresenham(x_1, y_1, radius);
        } else {
            retaBresenham(x_1, y_1, m_x, m_y);
        }
    }

    if (drawingPolygon && modo == POL && tempVertices.size() > 0) {
        glColor3f(0.0, 0.0, 0.0); // Cor padrão (preto)
        for (size_t i = 0; i < tempVertices.size() - 1; i++) {
            retaBresenham(tempVertices[i].x, tempVertices[i].y, tempVertices[i + 1].x, tempVertices[i + 1].y);
        }
        retaBresenham(tempVertices.back().x, tempVertices.back().y, m_x, m_y);
    }

    // Percorre a lista de formas geometricas para desenhar
    for (forward_list<forma>::iterator f = formas.begin(); f != formas.end(); ++f) {
        glColor3f(f->cor[0], f->cor[1], f->cor[2]); // Aplica a cor da forma
        switch (f->tipo) {
            case LIN: {
                int i = 0, x[2], y[2];
                for (auto v = f->v.begin(); v != f->v.end(); ++v, ++i) {
                    x[i] = v->x;
                    y[i] = v->y;
                }
                retaBresenham(x[0], y[0], x[1], y[1]);
                break;
            }
            case RET: {
                int i = 0, x[4], y[4];
                vertice verts[4];
                for (auto v = f->v.begin(); v != f->v.end() && i < 4; ++v, ++i) {
                    verts[i] = *v;
                }
                x[0] = verts[3].x; y[0] = verts[3].y; // inf. esquerdo
                x[1] = verts[2].x; y[1] = verts[2].y; // sup. esquerdo
                x[2] = verts[1].x; y[2] = verts[1].y; // sup. direito
                x[3] = verts[0].x; y[3] = verts[0].y; // inf. direito
                retaRetanguloBresenham(x[0], y[0], x[2], y[2]);
                if (preenchimentoAtivo || f->cor[0] == 1.0) { // Preenche se preenchimentoAtivo ou se a forma é vermelha
                    fillRectangle(x[0], y[0], x[2], y[2]);
                }
                break;
            }
            case TRI: {
                int i = 0, x[3], y[3];
                for (auto v = f->v.begin(); v != f->v.end() && i < 3; ++v, ++i) {
                    x[i] = v->x;
                    y[i] = v->y;
                }
                retaBresenham(x[0], y[0], x[1], y[1]);
                retaBresenham(x[1], y[1], x[2], y[2]);
                retaBresenham(x[2], y[2], x[0], y[0]);
                if (preenchimentoAtivo || f->cor[0] == 1.0) { // Preenche se preenchimentoAtivo ou se a forma é vermelha
                    fillTriangle(x[0], y[0], x[1], y[1], x[2], y[2]);
                }
                break;
            }
            case POL: {
                drawPoligono(f->v);
                if (preenchimentoAtivo || f->cor[0] == 1.0) { // Preenche se preenchimentoAtivo ou se a forma é vermelha
                    fillPolygon(f->v);
                }
                break;
            }
            case CIR: {
                if (f->v.size() >= 2) {
                    auto it = f->v.begin();
                    int xc = it->x;
                    int yc = it->y;
                    ++it;
                    int xr = it->x;
                    int yr = it->y;
                    int radius = (int)sqrt(pow(xr - xc, 2) + pow(yr - yc, 2));
                    drawCircleBresenham(xc, yc, radius);
                }
                break;
            }
        }
    }
}

/*
 * Fucao que implementa o Algoritmo de Rasterizacao da Reta Imediata
 */
void retaImediata(double x1, double y1, double x2, double y2)
{
    double m, b, yd, xd;
    double xmin, xmax, ymin, ymax;

    drawPixel((int)x1, (int)y1);
    if (x2 - x1 != 0)
    { // Evita a divisao por zero
        m = (y2 - y1) / (x2 - x1);
        b = y1 - (m * x1);

        if (m >= -1 && m <= 1)
        { // Verifica se o declive da reta tem tg de -1 a 1, se verdadeira calcula incrementando x
            xmin = (x1 < x2) ? x1 : x2;
            xmax = (x1 > x2) ? x1 : x2;

            for (int x = (int)xmin + 1; x < xmax; x++)
            {
                yd = (m * x) + b;
                yd = floor(0.5 + yd);
                drawPixel(x, (int)yd);
            }
        }
        else
        { // Se tg menor que -1 ou maior que 1, calcula incrementado os valores de y
            ymin = (y1 < y2) ? y1 : y2;
            ymax = (y1 > y2) ? y1 : y2;

            for (int y = (int)ymin + 1; y < ymax; y++)
            {
                xd = (y - b) / m;
                xd = floor(0.5 + xd);
                drawPixel((int)xd, y);
            }
        }
    }
    else
    { // se x2-x1 == 0, reta perpendicular ao eixo x
        ymin = (y1 < y2) ? y1 : y2;
        ymax = (y1 > y2) ? y1 : y2;
        for (int y = (int)ymin + 1; y < ymax; y++)
        {
            drawPixel((int)x1, y);
        }
    }
    drawPixel((int)x2, (int)y2);
}

/*
 * Fucao que implementa o Algoritmo de Bresenham
 */

void retaBresenham(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1); // delta x
    int dy = abs(y2 - y1); // delta y

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int aux = dx - dy; // isso é equivalente a p = 2*dy - dx, mas normalizado para lidar com qualquer octante

    while (true)
    {
        drawPixel(x1, y1);

        if (x1 == x2 && y1 == y2)
            break;

        int aux2 = 2 * aux; // acumulado

        if (aux2 > -dy)
        { // Verifica se o acumulado está pode avançar no eixo x
            aux -= dy;
            x1 += sx;
        }
        if (aux2 < dx)
        { // Verifica se o acumulado está pode avançar no eixo y
            aux += dx;
            y1 += sy;
        }
    }
}

void pushRetangulo(int x1, int y1, int x2, int y2)
{
    pushForma(RET);
    pushVertice(x1, y1); // sup. esquerdo
    pushVertice(x2, y1); // sup. direito
    pushVertice(x2, y2); // inf. direito
    pushVertice(x1, y2); // inf. esquerdo
}

void retaRetanguloBresenham(int x1, int y1, int x2, int y2)
{
    // Desenha os 4 lados do retângulo
    retaBresenham(x1, y1, x2, y1); // lado superior
    retaBresenham(x2, y1, x2, y2); // lado direito
    retaBresenham(x2, y2, x1, y2); // lado inferior
    retaBresenham(x1, y2, x1, y1); // lado esquerdo
}

void pushTriangulo(int x1, int y1, int x2, int y2, int x3, int y3)
{
    pushForma(TRI);
    pushVertice(x1, y1);
    pushVertice(x2, y2);
    pushVertice(x3, y3);
}

void pushPoligono(const int x[], const int y[], int n)
{
    if (n < 4)
        return;

    pushForma(POL);
    for (int i = 0; i < n; i++)
    {
        pushVertice(x[i], y[i]);
    }
}

void drawPoligono(const std::list<vertice> &vertices)
{
    int n = (int)vertices.size();
    if (n < 3)
        return;

    std::vector<vertice> verts(vertices.begin(), vertices.end());
    for (int i = 0; i < n - 1; i++)
    {
        retaBresenham(verts[i].x, verts[i].y, verts[i + 1].x, verts[i + 1].y);
    }
    // Fecha o polígono
    retaBresenham(verts[n - 1].x, verts[n - 1].y, verts[0].x, verts[0].y);
}

void transladarForma(forma &f, int dx, int dy)
{
    for (std::list<vertice>::iterator it = f.v.begin(); it != f.v.end(); ++it)
    {
        it->x += dx;
        it->y += dy;
    }
}

void escalarForma(forma &f, float sx, float sy)
{
    if (f.v.empty())
        return;

    int centro_x = 0, centro_y = 0, n = 0;
    for (std::list<vertice>::iterator it = f.v.begin(); it != f.v.end(); ++it)
    {
        centro_x += it->x;
        centro_y += it->y;
        n++;
    }
    centro_x /= n;
    centro_y /= n;

    for (std::list<vertice>::iterator it = f.v.begin(); it != f.v.end(); ++it)
    {
        int x_rel = it->x - centro_x;
        int y_rel = it->y - centro_y;

        it->x = static_cast<int>(x_rel * sx) + centro_x;
        it->y = static_cast<int>(y_rel * sy) + centro_y;
    }
}

void cisalharForma(forma &f, float shx, float shy)
{
    if (f.v.empty())
        return;

    int centro_x = 0, centro_y = 0, n = 0;
    for (std::list<vertice>::iterator it = f.v.begin(); it != f.v.end(); ++it)
    {
        centro_x += it->x;
        centro_y += it->y;
        n++;
    }
    centro_x /= n;
    centro_y /= n;

    for (std::list<vertice>::iterator it = f.v.begin(); it != f.v.end(); ++it)
    {
        int x_rel = it->x - centro_x;
        int y_rel = it->y - centro_y;

        int x_novo = static_cast<int>(x_rel + shx * y_rel);
        int y_novo = static_cast<int>(y_rel + shy * x_rel);

        it->x = x_novo + centro_x;
        it->y = y_novo + centro_y;
    }
}

void refletirForma(forma &f, bool refletirX, bool refletirY)
{
    if (f.v.empty())
        return;

    int centro_x = 0, centro_y = 0, n = 0;
    for (std::list<vertice>::iterator it = f.v.begin(); it != f.v.end(); ++it)
    {
        centro_x += it->x;
        centro_y += it->y;
        n++;
    }
    centro_x /= n;
    centro_y /= n;

    for (std::list<vertice>::iterator it = f.v.begin(); it != f.v.end(); ++it)
    {
        int x_rel = it->x - centro_x;
        int y_rel = it->y - centro_y;

        if (refletirX)
            y_rel = -y_rel;
        if (refletirY)
            x_rel = -x_rel;

        it->x = x_rel + centro_x;
        it->y = y_rel + centro_y;
    }
}

void rotacionarForma(forma &f, float angulo_graus)
{
    if (f.v.empty())
        return;

    // Calcula o centro geométrico da forma
    int centro_x = 0, centro_y = 0;
    int num_vertices = 0;

    for (std::list<vertice>::iterator it = f.v.begin(); it != f.v.end(); ++it)
    {
        centro_x += it->x;
        centro_y += it->y;
        num_vertices++;
    }

    centro_x /= num_vertices;
    centro_y /= num_vertices;

    // Converte ângulo para radianos
    float ang = angulo_graus * M_PI / 180.0f;
    float cos_ang = cos(ang);
    float sin_ang = sin(ang);

    // Aplica rotação em torno do centro
    for (std::list<vertice>::iterator it = f.v.begin(); it != f.v.end(); ++it)
    {
        // Translada para a origem
        int x_rel = it->x - centro_x;
        int y_rel = it->y - centro_y;

        // Rotaciona
        int x_rot = static_cast<int>(x_rel * cos_ang - y_rel * sin_ang);
        int y_rot = static_cast<int>(x_rel * sin_ang + y_rel * cos_ang);

        // Translada de volta
        it->x = x_rot + centro_x;
        it->y = y_rot + centro_y;
    }
}

void drawCircleBresenham(int xc, int yc, int radius)
{
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (x <= y)
    {
        // Desenha os 8 pontos simétricos
        drawPixel(xc + x, yc + y);
        drawPixel(xc - x, yc + y);
        drawPixel(xc + x, yc - y);
        drawPixel(xc - x, yc - y);
        drawPixel(xc + y, yc + x);
        drawPixel(xc - y, yc + x);
        drawPixel(xc + y, yc - x);
        drawPixel(xc - y, yc - x);

        if (d < 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}
/*
 * Função para preencher polígonos usando o algoritmo scanline
 */
/*
 * Função para preencher polígonos usando o algoritmo scanline (sintaxe antiga)
 */
void fillPolygon(const std::list<vertice> &vertices)
{
    if (vertices.size() < 3)
        return; // Precisa de pelo menos 3 vértices para formar um polígono

    // Encontra os limites mínimo e máximo em y
    int ymin = height, ymax = 0;
    for (std::list<vertice>::const_iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        const vertice &v = *it;
        if (v.y < ymin)
            ymin = v.y;
        if (v.y > ymax)
            ymax = v.y;
    }

    // Converte a lista de vértices para um vetor para facilitar o acesso
    std::vector<vertice> verts;
    for (std::list<vertice>::const_iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        verts.push_back(*it);
    }
    int n = verts.size();

    // Para cada linha de varredura (scanline)
    for (int y = ymin; y <= ymax; y++)
    {
        std::vector<int> intersections;

        // Encontra todas as interseções da scanline com as arestas do polígono
        for (int i = 0; i < n; i++)
        {
            int j = (i + 1) % n;
            int y1 = verts[i].y;
            int y2 = verts[j].y;

            // Verifica se a aresta cruza a scanline
            if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y))
            {
                // Calcula a interseção usando interpolação
                float x = verts[i].x + (float)(y - y1) / (float)(y2 - y1) * (verts[j].x - verts[i].x);
                intersections.push_back((int)x);
            }
        }

        // Ordena as interseções em ordem crescente de x
        std::sort(intersections.begin(), intersections.end());

        // Preenche os pixels entre pares de interseções
        for (size_t i = 0; i < intersections.size(); i += 2)
        {
            if (i + 1 >= intersections.size())
                break;
            int x1 = intersections[i];
            int x2 = intersections[i + 1];

            // Desenha a linha horizontal entre x1 e x2
            for (int x = x1; x <= x2; x++)
            {
                drawPixel(x, y);
            }
        }
    }
}

/*
 * Função para preencher um retângulo
 */
void fillRectangle(int x1, int y1, int x2, int y2)
{
    // Garante que x1 < x2 e y1 < y2
    if (x1 > x2)
        std::swap(x1, x2);
    if (y1 > y2)
        std::swap(y1, y2);

    // Preenche linha por linha
    for (int y = y1; y <= y2; y++)
    {
        for (int x = x1; x <= x2; x++)
        {
            drawPixel(x, y);
        }
    }
}

/*
 * Função para preencher um triângulo
 */
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    // Cria uma lista temporária de vértices
    std::list<vertice> vertices;

    // Cria objetos vertice explicitamente
    vertice v1;
    v1.x = x1;
    v1.y = y1;

    vertice v2;
    v2.x = x2;
    v2.y = y2;

    vertice v3;
    v3.x = x3;
    v3.y = y3;

    // Adiciona os vértices à lista
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    // Usa o mesmo algoritmo de preenchimento de polígonos
    fillPolygon(vertices);
}

// Verifica se um ponto (px, py) está próximo de uma linha (x1, y1) a (x2, y2)
bool pontoNaLinha(int px, int py, int x1, int y1, int x2, int y2, int tolerancia) {
    // Calcula a distância do ponto à reta usando a fórmula de distância ponto-reta
    float num = abs((y2 - y1) * px - (x2 - x1) * py + x2 * y1 - y2 * x1);
    float den = sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
    float distancia = num / den;

    // Verifica se o ponto está dentro da tolerância e dentro do retângulo delimitador da linha
    int minX = std::min(x1, x2) - tolerancia;
    int maxX = std::max(x1, x2) + tolerancia;
    int minY = std::min(y1, y2) - tolerancia;
    int maxY = std::max(y1, y2) + tolerancia;

    return distancia <= tolerancia && px >= minX && px <= maxX && py >= minY && py <= maxY;
}

// Verifica se um ponto (px, py) está dentro de um retângulo definido por (x1, y1) e (x2, y2)
bool pontoNoRetangulo(int px, int py, int x1, int y1, int x2, int y2) {
    int minX = std::min(x1, x2);
    int maxX = std::max(x1, x2);
    int minY = std::min(y1, y2);
    int maxY = std::max(y1, y2);
    return px >= minX && px <= maxX && py >= minY && py <= maxY;
}

// Verifica se um ponto (px, py) está dentro de um triângulo definido por (x1, y1), (x2, y2), (x3, y3)
bool pontoNoTriangulo(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3) {
    // Calcula a área do triângulo original
    float areaOrig = abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0f);

    // Calcula as áreas dos três triângulos formados pelo ponto (px, py)
    float area1 = abs((px * (y2 - y3) + x2 * (y3 - py) + x3 * (py - y2)) / 2.0f);
    float area2 = abs((x1 * (py - y3) + px * (y3 - y1) + x3 * (y1 - py)) / 2.0f);
    float area3 = abs((x1 * (y2 - py) + x2 * (py - y1) + px * (y1 - y2)) / 2.0f);

    // O ponto está dentro se a soma das áreas for aproximadamente igual à área original
    return abs(areaOrig - (area1 + area2 + area3)) < 0.0001f;
}

// Verifica se um ponto (px, py) está dentro de um polígono usando o algoritmo de ray-casting
bool pontoNoPoligono(int px, int py, const std::list<vertice> &vertices) {
    int n = vertices.size();
    if (n < 3)
        return false;

    std::vector<vertice> verts(vertices.begin(), vertices.end());
    int intersections = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int y1 = verts[i].y;
        int y2 = verts[j].y;
        int x1 = verts[i].x;
        int x2 = verts[j].x;

        if ((y1 <= py && py < y2) || (y2 <= py && py < y1)) {
            if (x1 + (float)(py - y1) / (y2 - y1) * (x2 - x1) > px) {
                intersections++;
            }
        }
    }
    return (intersections % 2) == 1; // Ímpar = dentro, par = fora
}

// Verifica se um ponto (px, py) está dentro ou na borda de um círculo
bool pontoNoCirculo(int px, int py, int xc, int yc, int radius) {
    float distancia = sqrt(pow(px - xc, 2) + pow(py - yc, 2));
    return distancia <= radius;
}