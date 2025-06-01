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
    CIR
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
};

// Lista encadeada de formas geometricas
forward_list<forma> formas;

// Funcao para armazenar uma forma geometrica na lista de formas
// Armazena sempre no inicio da lista
void pushForma(int tipo)
{
    forma f;
    f.tipo = tipo;
    formas.push_front(f);
}

// Funcao para armazenar um vertice na forma do inicio da lista de formas geometricas
// Armazena sempre no inicio da lista
void pushVertice(int x, int y)
{
    vertice v;
    v.x = x;
    v.y = y;
    formas.front().v.push_back(v);
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
void menu_popup(int value)
{
    if (value == 0)
        exit(EXIT_SUCCESS);
    modo = value;
}

/*
 * Controle das teclas comuns do teclado
 */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    { // key - variavel que possui valor ASCII da tecla precionada
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
void mouse(int button, int state, int x, int y)
{
    int mouseX = x;
    int mouseY = height - y - 1;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (modo == LIN)
        {
            if (click1)
            {
                x_2 = x;
                y_2 = height - y - 1;
                printf("Clique 2 (%d, %d)\n", x_2, y_2);
                pushLinha(x_1, y_1, x_2, y_2);
                click1 = false;
                glutPostRedisplay();
            }
            else
            {
                click1 = true;
                x_1 = x;
                y_1 = height - y - 1;
                printf("Clique 1 (%d, %d)\n", x_1, y_1);
            }
        }
        else if (modo == RET)
        {
            if (click1)
            {
                x_2 = x;
                y_2 = height - y - 1;
                printf("Clique 2 (%d, %d)\n", x_2, y_2);
                pushRetangulo(x_1, y_1, x_2, y_2);
                click1 = false;
                glutPostRedisplay();
            }
            else
            {
                click1 = true;
                x_1 = x;
                y_1 = height - y - 1;
                printf("Clique 1 (%d, %d)\n", x_1, y_1);
            }
        }
        else if (modo == TRI)
        {
            x_clicks[clickCount] = x;
            y_clicks[clickCount] = height - y - 1;
            printf("Clique %d (%d, %d)\n", clickCount + 1, x_clicks[clickCount], y_clicks[clickCount]);
            clickCount++;

            if (clickCount == 3)
            {
                pushTriangulo(x_clicks[0], y_clicks[0], x_clicks[1], y_clicks[1], x_clicks[2], y_clicks[2]);
                clickCount = 0;
                glutPostRedisplay();
            }
        }
        else if (modo == POL)
        {
            // Começa o desenho de polígono
            if (!drawingPolygon)
            {
                drawingPolygon = true;
                tempVertices.clear();
            }
            // adiciona o vértice atual
            tempVertices.push_back(vertice{mouseX, mouseY});
            glutPostRedisplay();
        }
        else if (modo == CIR)
        {
            if (click1)
            {
                x_2 = x;
                y_2 = height - y - 1;
                // Calcula o raio como a distância entre os dois pontos
                int radius = (int)sqrt(pow(x_2 - x_1, 2) + pow(y_2 - y_1, 2));
                printf("Clique 2 (%d, %d), raio: %d\n", x_2, y_2, radius);
                pushCirculo(x_1, y_1, radius);
                click1 = false;
                glutPostRedisplay();
            }
            else
            {
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
void drawFormas()
{
    // Apos o primeiro clique, desenha a reta com a posicao atual do mouse
    if (click1)
    {
        if (modo == CIR)
        {
            int radius = (int)sqrt(pow(m_x - x_1, 2) + pow(m_y - y_1, 2));
            drawCircleBresenham(x_1, y_1, radius);
        }
        else
        {
            retaBresenham(x_1, y_1, m_x, m_y);
        }
    }

    if (drawingPolygon && modo == POL && tempVertices.size() > 0)
    {
        for (size_t i = 0; i < tempVertices.size() - 1; i++)
        {
            retaBresenham(tempVertices[i].x, tempVertices[i].y, tempVertices[i + 1].x, tempVertices[i + 1].y);
        }
        // Linha provisória até o mouse atual
        retaBresenham(tempVertices.back().x, tempVertices.back().y, m_x, m_y);
    }
    // Percorre a lista de formas geometricas para desenhar
    for (forward_list<forma>::iterator f = formas.begin(); f != formas.end(); f++)
    {
        switch (f->tipo)
        {
        case LIN:
        {
            int i = 0, x[2], y[2];
            // Percorre a lista de vertices da forma linha para desenhar
            for (auto v = f->v.begin(); v != f->v.end(); v++, i++)
            {
                x[i] = v->x;
                y[i] = v->y;
            }
            // Desenha o segmento de reta apos dois cliques
            retaBresenham(x[0], y[0], x[1], y[1]);
            break;
        }
        case RET:
        {
            int i = 0, x[4], y[4];
            // Como push_front inverte a ordem, primeiro armazena os vértices em vetor temporário
            vertice verts[4];
            for (auto v = f->v.begin(); v != f->v.end() && i < 4; v++, i++)
            {
                verts[i] = *v;
            }
            // Agora copia invertido para x e y
            for (int j = 0; j < i; j++)
            {
                x[j] = verts[i - 1 - j].x;
                y[j] = verts[i - 1 - j].y;
            }
            retaRetanguloBresenham(x[0], y[0], x[2], y[2]);
            break;
        }
        case TRI:
        {
            int i = 0, x[3], y[3];
            for (auto v = f->v.begin(); v != f->v.end() && i < 3; v++, i++)
            {
                x[i] = v->x;
                y[i] = v->y;
            }
            // Desenha as 3 arestas do triângulo
            retaBresenham(x[0], y[0], x[1], y[1]);
            retaBresenham(x[1], y[1], x[2], y[2]);
            retaBresenham(x[2], y[2], x[0], y[0]);
            break;
        }
        case POL:
        {
            drawPoligono(f->v);
            break;
        }
        case CIR:
        {
            if (f->v.size() >= 2)
            {
                auto it = f->v.begin();
                int xc = it->x; // Centro x
                int yc = it->y; // Centro y
                it++;
                int xr = it->x;                                            // Ponto no raio (x)
                int yr = it->y;                                            // Ponto no raio (y)
                int radius = (int)sqrt(pow(xr - xc, 2) + pow(yr - yc, 2)); // Calcula raio corretamente
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

void pushCirculo(int xc, int yc, int radius)
{
    pushForma(CIR);
    pushVertice(xc, yc);          // Centro do círculo
    pushVertice(xc + radius, yc); // Ponto para armazenar o raio (pode ser qualquer ponto na circunferência)
}
