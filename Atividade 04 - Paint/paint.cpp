/*
*** ENGLISH
* Computer Graphics
* Example Code: Line Segment Rasterization with GLUT/OpenGL
* Author: Prof. Laurindo de Sousa Britto Neto
*
* Usage Instructions:
* - Right-click to open the pop-up menu and select a drawing mode:
*   - Line: Draw a line with two mouse clicks.
*   - Rectangle: Draw a rectangle with two mouse clicks (opposite corners).
*   - Triangle: Draw a triangle with three mouse clicks.
*   - Polygon: Click multiple points to define vertices; press Enter to close the polygon (minimum of 4 vertices).
*   - Circle: The first click sets the center, the second click sets the radius.
*   - Paint: Click on a shape to fill it with red color.
*   - Exit: Closes the application.
* - Keyboard Controls:
*   - ESC: Exits the application.
*   - Enter: Finalizes the drawing of a polygon (in Polygon mode).
*   - p/P: Toggles fill mode for rectangles, triangles, and polygons.
*   - t: Translates the last drawn shape by (20, 20).
*   - T: Translates the last drawn shape by (-20, -20).
*   - e: Scales the last drawn shape by 1.5x.
*   - E: Scales down the last drawn shape by 1/1.5x.
*   - c: Shears the last drawn shape in the X direction (0.5).
*   - C: Reverses the shear in the X direction (-0.5) for the last drawn shape.
*   - r: Rotates the last drawn shape 45 degrees clockwise.
*   - R: Rotates the last drawn shape 45 degrees counterclockwise.
*   - x/X: Reflects the last drawn shape across the X axis.
*   - y/Y: Reflects the last drawn shape across the Y axis.
* - Mouse:
*   - Left click: Defines points for drawing shapes or selects shapes in Paint mode.
*   - Mouse movement: Displays the current coordinates in the bottom-left corner.
*/
/*
***** PORTUGUÊS
* Computação Gráfica
* Código Exemplo: Rasterização de Segmentos de Reta com GLUT/OpenGL
* Autor: Prof. Laurindo de Sousa Britto Neto
*
* Instruções de Uso:
* - Clique com o botão direito para abrir o menu pop-up e selecionar um modo de desenho:
*   - Linha: Desenhe uma linha com dois cliques do mouse.
*   - Retângulo: Desenhe um retângulo com dois cliques do mouse (cantos opostos).
*   - Triângulo: Desenhe um triângulo com três cliques do mouse.
*   - Polígono: Clique em vários pontos para definir vértices; pressione Enter para fechar o polígono (mínimo de 4 vértices).
*   - Círculo: O primeiro clique define o centro, o segundo clique define o raio.
*   - Pintar: Clique em uma forma para colori-la de vermelho.
*   - Sair: Fecha o aplicativo.
* - Controles do teclado:
*   - ESC: Sai do aplicativo.
*   - Enter: Finaliza o desenho de um polígono (no modo Polígono).
*   - p/P: Alterna o modo de preenchimento para retângulos, triângulos e polígonos.
*   - t: Translada a última forma desenhada por (20, 20).
*   - T: Translada a última forma desenhada por (-20, -20).
*   - e: Escala a última forma desenhada em 1.5x.
*   - E: Reduz a escala da última forma desenhada em 1/1.5x.
*   - c: Cisaia a última forma desenhada na direção X (0.5).
*   - C: Reverte o cisalhamento da última forma desenhada na direção X (-0.5).
*   - r: Rotaciona a última forma desenhada em 45 graus no sentido horário.
*   - R: Rotaciona a última forma desenhada em 45 graus no sentido anti-horário.
*   - x/X: Reflete a última forma desenhada sobre o eixo X.
*   - y/Y: Reflete a última forma desenhada sobre o eixo Y.
* - Mouse:
*   - Clique esquerdo: Define pontos para desenhar formas ou seleciona formas no modo Pintar.
*   - Movimento do mouse: Exibe as coordenadas atuais no canto inferior esquerdo.
*/
// OpenGL libraries
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
#include <algorithm>

using namespace std;

// Global variables
#define ESC 27
#define ENTER 13
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Enum for geometric shape types
enum ShapeType
{
    LINE = 1,
    TRIANGLE,
    RECTANGLE,
    POLYGON,
    CIRCLE,
    PAINT
};

// Flag for first mouse click
bool firstClick = false;

// Current mouse position coordinates
int mouseX, mouseY;

// Coordinates of first and second mouse clicks
int x1, y1, x2, y2;

// Active geometric shape type for drawing
int mode = LINE;

// Window dimensions
int width = 512, height = 512;

int clickCount = 0;
int xClicks[3], yClicks[3];

bool fillActive = false; // Toggled by 'p'/'P' key

// Vertex structure
struct Vertex
{
    int x;
    int y;
};

std::vector<Vertex> tempVertices;
bool drawingPolygon = false;

// Geometric shape structure
struct Shape
{
    int type;
    std::list<Vertex> vertices;
    float color[3];
};

// Linked list of geometric shapes
forward_list<Shape> shapes;

// Function to add a vertex to the shape at the front of the shapes list
void pushVertex(int x, int y)
{
    Vertex v;
    v.x = x;
    v.y = y;
    shapes.front().vertices.push_back(v);
}

// Function to add a shape to the shapes list
void pushShape(int type)
{
    Shape s;
    s.type = type;
    s.color[0] = 0.0; // R
    s.color[1] = 0.0; // G
    s.color[2] = 0.0; // B
    shapes.push_front(s);
}

void pushCircle(int xc, int yc, int radius)
{
    pushShape(CIRCLE);
    pushVertex(xc, yc);            // Circle center
    pushVertex(xc + radius, yc);   // Point to store radius
    shapes.front().color[0] = 0.0; // Initialize with black
    shapes.front().color[1] = 0.0;
    shapes.front().color[2] = 0.0;
}

// Function to add a line to the shapes list
void pushLine(int x1, int y1, int x2, int y2)
{
    pushShape(LINE);
    pushVertex(x1, y1);
    pushVertex(x2, y2);
}

/*
 * Function declarations
 */
void init(void);
void reshape(int w, int h);
void display(void);
void menuPopup(int value);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mousePassiveMotion(int x, int y);
void drawPixel(int x, int y);
void drawShapes();
void immediateLine(double x1, double y1, double x2, double y2);
void bresenhamLine(int x1, int y1, int x2, int y2);
void pushRectangle(int x1, int y1, int x2, int y2);
void bresenhamRectangle(int x1, int y1, int x2, int y2);
void pushTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
void drawPolygon(const std::list<Vertex> &vertices);
void translateShape(Shape &s, int dx, int dy);
void scaleShape(Shape &s, float sx, float sy);
void shearShape(Shape &s, float shx, float shy);
void reflectShape(Shape &s, bool reflectX, bool reflectY);
void rotateShape(Shape &s, float angleDegrees);
void drawCircleBresenham(int xc, int yc, int radius);
void fillPolygon(const std::list<Vertex> &vertices);
void fillRectangle(int x1, int y1, int x2, int y2);
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
bool pointOnLine(int px, int py, int x1, int y1, int x2, int y2, int tolerance = 5);
bool pointInRectangle(int px, int py, int x1, int y1, int x2, int y2);
bool pointInTriangle(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3);
bool pointInPolygon(int px, int py, const std::list<Vertex> &vertices);
bool pointInCircle(int px, int py, int xc, int yc, int radius);

/*
 * Main function
 */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Computer Graphics: Paint");
    init();
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mousePassiveMotion);
    glutDisplayFunc(display);

    // Define the pop-up menu
    glutCreateMenu(menuPopup);
    glutAddMenuEntry("Line", LINE);
    glutAddMenuEntry("Rectangle", RECTANGLE);
    glutAddMenuEntry("Triangle", TRIANGLE);
    glutAddMenuEntry("Polygon", POLYGON);
    glutAddMenuEntry("Circle", CIRCLE);
    glutAddMenuEntry("Paint", PAINT);
    glutAddMenuEntry("Exit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return EXIT_SUCCESS;
}

/*
 * Initializes GLUT parameters
 */
void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // Clear screen with white color
}

/*
 * Adjusts projection for window resizing
 */
void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    glOrtho(0, w, 0, h, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
 * Controls screen drawing
 */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0); // Default color: black
    drawShapes();
    draw_text_stroke(0, 0, "(" + to_string(mouseX) + "," + to_string(mouseY) + ")", 0.2);
    glutSwapBuffers();
}

/*
 * Controls the pop-up menu
 */
void menuPopup(int value)
{
    if (value == 0)
        exit(EXIT_SUCCESS);
    mode = value;
    if (mode == PAINT)
    {
        firstClick = false;
        clickCount = 0;
        tempVertices.clear();
        drawingPolygon = false;
        printf("Paint mode activated. Temporary points cleared.\n");
    }
    glutPostRedisplay();
}

/*
 * Handles keyboard input
 */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ESC:
        exit(EXIT_SUCCESS);
        break;
    case ENTER:
        if (mode == POLYGON && drawingPolygon)
        {
            if (tempVertices.size() >= 4)
            {
                pushShape(POLYGON);
                std::vector<Vertex>::const_iterator it;
                for (it = tempVertices.begin(); it != tempVertices.end(); ++it)
                {
                    pushVertex(it->x, it->y);
                }
            }
            tempVertices.clear();
            drawingPolygon = false;
            glutPostRedisplay();
        }
        break;
    case 'p':
    case 'P':
        fillActive = !fillActive;
        printf("Fill %s\n", fillActive ? "Enabled" : "Disabled");
        glutPostRedisplay();
        break;
    case 't':
        if (!shapes.empty())
            translateShape(shapes.front(), 20, 20);
        glutPostRedisplay();
        break;
    case 'T':
        if (!shapes.empty())
            translateShape(shapes.front(), -20, -20);
        glutPostRedisplay();
        break;
    case 'e':
        if (!shapes.empty())
            scaleShape(shapes.front(), 1.5, 1.5);
        glutPostRedisplay();
        break;
    case 'E':
        if (!shapes.empty())
            scaleShape(shapes.front(), 1.0 / 1.5, 1.0 / 1.5);
        glutPostRedisplay();
        break;
    case 'c':
        if (!shapes.empty())
            shearShape(shapes.front(), 0.5, 0.0);
        glutPostRedisplay();
        break;
    case 'C':
        if (!shapes.empty())
            shearShape(shapes.front(), -0.5, 0.0);
        glutPostRedisplay();
        break;
    case 'r':
        if (!shapes.empty())
            rotateShape(shapes.front(), 45);
        glutPostRedisplay();
        break;
    case 'R':
        if (!shapes.empty())
            rotateShape(shapes.front(), -45);
        glutPostRedisplay();
        break;
    case 'x':
        if (!shapes.empty())
            reflectShape(shapes.front(), true, false);
        glutPostRedisplay();
        break;
    case 'X':
        if (!shapes.empty())
            reflectShape(shapes.front(), true, false);
        glutPostRedisplay();
        break;
    case 'y':
        if (!shapes.empty())
            reflectShape(shapes.front(), false, true);
        glutPostRedisplay();
        break;
    case 'Y':
        if (!shapes.empty())
            reflectShape(shapes.front(), false, true);
        glutPostRedisplay();
        break;
    }
}

/*
 * Handles mouse button events
 */
void mouse(int button, int state, int x, int y)
{
    int mouseX = x;
    int mouseY = height - y - 1;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (mode == PAINT)
        {
            bool shapeFound = false;
            for (auto f = shapes.begin(); f != shapes.end(); ++f)
            {
                switch (f->type)
                {
                case LINE:
                {
                    int i = 0, x[2], y[2];
                    for (auto v = f->vertices.begin(); v != f->vertices.end(); ++v, ++i)
                    {
                        x[i] = v->x;
                        y[i] = v->y;
                    }
                    if (pointOnLine(mouseX, mouseY, x[0], y[0], x[1], y[1]))
                    {
                        f->color[0] = 1.0; // Red
                        f->color[1] = 0.0;
                        f->color[2] = 0.0;
                        shapeFound = true;
                    }
                    break;
                }
                case RECTANGLE:
                {
                    int i = 0, x[4], y[4];
                    Vertex verts[4];
                    for (auto v = f->vertices.begin(); v != f->vertices.end() && i < 4; ++v, ++i)
                    {
                        verts[i] = *v;
                    }
                    x[0] = verts[3].x;
                    y[0] = verts[3].y; // Bottom-left
                    x[2] = verts[1].x;
                    y[2] = verts[1].y; // Top-right
                    if (pointInRectangle(mouseX, mouseY, x[0], y[0], x[2], y[2]))
                    {
                        f->color[0] = 1.0; // Red
                        f->color[1] = 0.0;
                        f->color[2] = 0.0;
                        shapeFound = true;
                    }
                    break;
                }
                case TRIANGLE:
                {
                    int i = 0, x[3], y[3];
                    for (auto v = f->vertices.begin(); v != f->vertices.end() && i < 3; ++v, ++i)
                    {
                        x[i] = v->x;
                        y[i] = v->y;
                    }
                    if (pointInTriangle(mouseX, mouseY, x[0], y[0], x[1], y[1], x[2], y[2]))
                    {
                        f->color[0] = 1.0; // Red
                        f->color[1] = 0.0;
                        f->color[2] = 0.0;
                        shapeFound = true;
                    }
                    break;
                }
                case POLYGON:
                {
                    if (pointInPolygon(mouseX, mouseY, f->vertices))
                    {
                        f->color[0] = 1.0; // Red
                        f->color[1] = 0.0;
                        f->color[2] = 0.0;
                        shapeFound = true;
                    }
                    break;
                }
                case CIRCLE:
                {
                    auto it = f->vertices.begin();
                    int xc = it->x;
                    int yc = it->y;
                    ++it;
                    int xr = it->x;
                    int yr = it->y;
                    int radius = (int)sqrt(pow(xr - xc, 2) + pow(yr - yc, 2));
                    if (pointInCircle(mouseX, mouseY, xc, yc, radius))
                    {
                        f->color[0] = 1.0; // Red
                        f->color[1] = 0.0;
                        f->color[2] = 0.0;
                        shapeFound = true;
                    }
                    break;
                }
                }
                if (shapeFound)
                    break;
            }
            if (!shapeFound)
            {
                printf("No shape found at point (%d, %d)\n", mouseX, mouseY);
            }
            glutPostRedisplay();
        }
        else if (mode == LINE)
        {
            if (firstClick)
            {
                x2 = x;
                y2 = height - y - 1;
                printf("Click 2 (%d, %d)\n", x2, y2);
                pushLine(x1, y1, x2, y2);
                firstClick = false;
                glutPostRedisplay();
            }
            else
            {
                firstClick = true;
                x1 = x;
                y1 = height - y - 1;
                printf("Click 1 (%d, %d)\n", x1, y1);
            }
        }
        else if (mode == RECTANGLE)
        {
            if (firstClick)
            {
                x2 = x;
                y2 = height - y - 1;
                printf("Click 2 (%d, %d)\n", x2, y2);
                pushRectangle(x1, y1, x2, y2);
                firstClick = false;
                glutPostRedisplay();
            }
            else
            {
                firstClick = true;
                x1 = x;
                y1 = height - y - 1;
                printf("Click 1 (%d, %d)\n", x1, y1);
            }
        }
        else if (mode == TRIANGLE)
        {
            xClicks[clickCount] = x;
            yClicks[clickCount] = height - y - 1;
            printf("Click %d (%d, %d)\n", clickCount + 1, xClicks[clickCount], yClicks[clickCount]);
            clickCount++;
            if (clickCount == 3)
            {
                pushTriangle(xClicks[0], yClicks[0], xClicks[1], yClicks[1], xClicks[2], yClicks[2]);
                clickCount = 0;
                glutPostRedisplay();
            }
        }
        else if (mode == POLYGON)
        {
            if (!drawingPolygon)
            {
                drawingPolygon = true;
                tempVertices.clear();
            }
            tempVertices.push_back(Vertex{mouseX, mouseY});
            glutPostRedisplay();
        }
        else if (mode == CIRCLE)
        {
            if (firstClick)
            {
                x2 = x;
                y2 = height - y - 1;
                int radius = (int)sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
                printf("Click 2 (%d, %d), radius: %d\n", x2, y2, radius);
                pushCircle(x1, y1, radius);
                firstClick = false;
                glutPostRedisplay();
            }
            else
            {
                firstClick = true;
                x1 = x;
                y1 = height - y - 1;
                printf("Click 1 (center) (%d, %d)\n", x1, y1);
            }
        }
    }
}

/*
 * Tracks mouse cursor position
 */
void mousePassiveMotion(int x, int y)
{
    mouseX = x;
    mouseY = height - y - 1;
    glutPostRedisplay();
}

/*
 * Draws a single pixel on the screen
 */
void drawPixel(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

/*
 * Draws the list of geometric shapes
 */
void drawShapes()
{
    if (firstClick && mode != PAINT)
    {
        glColor3f(0.0, 0.0, 0.0);
        if (mode == CIRCLE)
        {
            int radius = (int)sqrt(pow(mouseX - x1, 2) + pow(mouseY - y1, 2));
            drawCircleBresenham(x1, y1, radius);
        }
        else
        {
            bresenhamLine(x1, y1, mouseX, mouseY);
        }
    }

    if (drawingPolygon && mode == POLYGON && !tempVertices.empty())
    {
        glColor3f(0.0, 0.0, 0.0);
        for (size_t i = 0; i < tempVertices.size() - 1; i++)
        {
            bresenhamLine(tempVertices[i].x, tempVertices[i].y, tempVertices[i + 1].x, tempVertices[i + 1].y);
        }
        bresenhamLine(tempVertices.back().x, tempVertices.back().y, mouseX, mouseY);
    }

    for (auto f = shapes.begin(); f != shapes.end(); ++f)
    {
        glColor3f(f->color[0], f->color[1], f->color[2]);
        switch (f->type)
        {
        case LINE:
        {
            int i = 0, x[2], y[2];
            for (auto v = f->vertices.begin(); v != f->vertices.end(); ++v, ++i)
            {
                x[i] = v->x;
                y[i] = v->y;
            }
            bresenhamLine(x[0], y[0], x[1], y[1]);
            break;
        }
        case RECTANGLE:
        {
            int i = 0, x[4], y[4];
            Vertex verts[4];
            for (auto v = f->vertices.begin(); v != f->vertices.end() && i < 4; ++v, ++i)
            {
                verts[i] = *v;
            }
            x[0] = verts[3].x;
            y[0] = verts[3].y; // Bottom-left
            x[1] = verts[2].x;
            y[1] = verts[2].y; // Top-left
            x[2] = verts[1].x;
            y[2] = verts[1].y; // Top-right
            x[3] = verts[0].x;
            y[3] = verts[0].y; // Bottom-right
            bresenhamRectangle(x[0], y[0], x[2], y[2]);
            if (fillActive || f->color[0] == 1.0)
            {
                fillRectangle(x[0], y[0], x[2], y[2]);
            }
            break;
        }
        case TRIANGLE:
        {
            int i = 0, x[3], y[3];
            for (auto v = f->vertices.begin(); v != f->vertices.end() && i < 3; ++v, ++i)
            {
                x[i] = v->x;
                y[i] = v->y;
            }
            bresenhamLine(x[0], y[0], x[1], y[1]);
            bresenhamLine(x[1], y[1], x[2], y[2]);
            bresenhamLine(x[2], y[2], x[0], y[0]);
            if (fillActive || f->color[0] == 1.0)
            {
                fillTriangle(x[0], y[0], x[1], y[1], x[2], y[2]);
            }
            break;
        }
        case POLYGON:
        {
            drawPolygon(f->vertices);
            if (fillActive || f->color[0] == 1.0)
            {
                fillPolygon(f->vertices);
            }
            break;
        }
        case CIRCLE:
        {
            if (f->vertices.size() >= 2)
            {
                auto it = f->vertices.begin();
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
 * Implements the Immediate Line Rasterization Algorithm
 */
void immediateLine(double x1, double y1, double x2, double y2)
{
    double m, b, yd, xd;
    double xmin, xmax, ymin, ymax;

    drawPixel((int)x1, (int)y1);
    if (x2 - x1 != 0)
    {
        m = (y2 - y1) / (x2 - x1);
        b = y1 - (m * x1);

        if (m >= -1 && m <= 1)
        {
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
        {
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
    {
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
 * Implements the Bresenham Line Algorithm
 */
void bresenhamLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int aux = dx - dy;

    while (true)
    {
        drawPixel(x1, y1);
        if (x1 == x2 && y1 == y2)
            break;
        int aux2 = 2 * aux;
        if (aux2 > -dy)
        {
            aux -= dy;
            x1 += sx;
        }
        if (aux2 < dx)
        {
            aux += dx;
            y1 += sy;
        }
    }
}

void pushRectangle(int x1, int y1, int x2, int y2)
{
    pushShape(RECTANGLE);
    pushVertex(x1, y1); // Top-left
    pushVertex(x2, y1); // Top-right
    pushVertex(x2, y2); // Bottom-right
    pushVertex(x1, y2); // Bottom-left
}

void bresenhamRectangle(int x1, int y1, int x2, int y2)
{
    bresenhamLine(x1, y1, x2, y1); // Top
    bresenhamLine(x2, y1, x2, y2); // Right
    bresenhamLine(x2, y2, x1, y2); // Bottom
    bresenhamLine(x1, y2, x1, y1); // Left
}

void pushTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    pushShape(TRIANGLE);
    pushVertex(x1, y1);
    pushVertex(x2, y2);
    pushVertex(x3, y3);
}

void pushPolygon(const int x[], const int y[], int n)
{
    if (n < 4)
        return;
    pushShape(POLYGON);
    for (int i = 0; i < n; i++)
    {
        pushVertex(x[i], y[i]);
    }
}

void drawPolygon(const std::list<Vertex> &vertices)
{
    int n = (int)vertices.size();
    if (n < 3)
        return;
    std::vector<Vertex> verts(vertices.begin(), vertices.end());
    for (int i = 0; i < n - 1; i++)
    {
        bresenhamLine(verts[i].x, verts[i].y, verts[i + 1].x, verts[i + 1].y);
    }
    bresenhamLine(verts[n - 1].x, verts[n - 1].y, verts[0].x, verts[0].y);
}

void translateShape(Shape &s, int dx, int dy)
{
    for (std::list<Vertex>::iterator it = s.vertices.begin(); it != s.vertices.end(); ++it)
    {
        it->x += dx;
        it->y += dy;
    }
}

void scaleShape(Shape &s, float sx, float sy)
{
    if (s.vertices.empty())
        return;
    int centerX = 0, centerY = 0, n = 0;
    std::list<Vertex>::const_iterator cit;
    for (cit = s.vertices.begin(); cit != s.vertices.end(); ++cit)
    {
        centerX += cit->x;
        centerY += cit->y;
        n++;
    }
    centerX /= n;
    centerY /= n;
    std::list<Vertex>::iterator it;
    for (it = s.vertices.begin(); it != s.vertices.end(); ++it)
    {
        int xRel = it->x - centerX;
        int yRel = it->y - centerY;
        it->x = static_cast<int>(xRel * sx) + centerX;
        it->y = static_cast<int>(yRel * sy) + centerY;
    }
}

void shearShape(Shape &s, float shx, float shy)
{
    if (s.vertices.empty())
        return;

    int centerX = 0, centerY = 0, n = 0;
    std::list<Vertex>::const_iterator cit;
    for (cit = s.vertices.begin(); cit != s.vertices.end(); ++cit)
    {
        centerX += cit->x;
        centerY += cit->y;
        n++;
    }
    centerX /= n;
    centerY /= n;

    std::list<Vertex>::iterator it;
    for (it = s.vertices.begin(); it != s.vertices.end(); ++it)
    {
        int xRel = it->x - centerX;
        int yRel = it->y - centerY;
        int xNew = static_cast<int>(xRel + shx * yRel);
        int yNew = static_cast<int>(yRel + shy * xRel);
        it->x = xNew + centerX;
        it->y = yNew + centerY;
    }
}

void reflectShape(Shape &s, bool reflectX, bool reflectY)
{
    if (s.vertices.empty())
        return;

    int centerX = 0, centerY = 0, n = 0;
    std::list<Vertex>::const_iterator cit;
    for (cit = s.vertices.begin(); cit != s.vertices.end(); ++cit)
    {
        centerX += cit->x;
        centerY += cit->y;
        n++;
    }
    centerX /= n;
    centerY /= n;

    std::list<Vertex>::iterator it;
    for (it = s.vertices.begin(); it != s.vertices.end(); ++it)
    {
        int xRel = it->x - centerX;
        int yRel = it->y - centerY;
        if (reflectX)
            yRel = -yRel;
        if (reflectY)
            xRel = -xRel;
        it->x = xRel + centerX;
        it->y = yRel + centerY;
    }
}

void rotateShape(Shape &s, float angleDegrees)
{
    if (s.vertices.empty())
        return;

    int centerX = 0, centerY = 0, n = 0;
    std::list<Vertex>::const_iterator cit;
    for (cit = s.vertices.begin(); cit != s.vertices.end(); ++cit)
    {
        centerX += cit->x;
        centerY += cit->y;
        n++;
    }
    centerX /= n;
    centerY /= n;

    float angleRad = angleDegrees * M_PI / 180.0f;
    float cosAngle = cos(angleRad);
    float sinAngle = sin(angleRad);

    std::list<Vertex>::iterator it;
    for (it = s.vertices.begin(); it != s.vertices.end(); ++it)
    {
        int xRel = it->x - centerX;
        int yRel = it->y - centerY;
        int xRot = static_cast<int>(xRel * cosAngle - yRel * sinAngle);
        int yRot = static_cast<int>(xRel * sinAngle + yRel * cosAngle);
        it->x = xRot + centerX;
        it->y = yRot + centerY;
    }
}

void drawCircleBresenham(int xc, int yc, int radius)
{
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    while (x <= y)
    {
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
 * Fills a polygon using the scanline algorithm
 */
void fillPolygon(const std::list<Vertex> &vertices)
{
    if (vertices.size() < 3)
        return;
    int ymin = height, ymax = 0;
    std::list<Vertex>::const_iterator it;
    for (it = vertices.begin(); it != vertices.end(); ++it)
    {
        if (it->y < ymin)
            ymin = it->y;
        if (it->y > ymax)
            ymax = it->y;
    }
    std::vector<Vertex> verts(vertices.begin(), vertices.end());
    int n = verts.size();
    for (int y = ymin; y <= ymax; y++)
    {
        std::vector<int> intersections;
        for (int i = 0; i < n; i++)
        {
            int j = (i + 1) % n;
            int y1 = verts[i].y;
            int y2 = verts[j].y;
            if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y))
            {
                float x = verts[i].x + (float)(y - y1) / (y2 - y1) * (verts[j].x - verts[i].x);
                intersections.push_back((int)x);
            }
        }
        std::sort(intersections.begin(), intersections.end());
        for (size_t i = 0; i < intersections.size(); i += 2)
        {
            if (i + 1 >= intersections.size())
                break;
            int x1 = intersections[i];
            int x2 = intersections[i + 1];
            for (int x = x1; x <= x2; x++)
            {
                drawPixel(x, y);
            }
        }
    }
}

/*
 * Fills a rectangle
 */
void fillRectangle(int x1, int y1, int x2, int y2)
{
    if (x1 > x2)
        std::swap(x1, x2);
    if (y1 > y2)
        std::swap(y1, y2);
    for (int y = y1; y <= y2; y++)
    {
        for (int x = x1; x <= x2; x++)
        {
            drawPixel(x, y);
        }
    }
}

/*
 * Fills a triangle
 */
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    std::list<Vertex> vertices;
    vertices.push_back(Vertex{x1, y1});
    vertices.push_back(Vertex{x2, y2});
    vertices.push_back(Vertex{x3, y3});
    fillPolygon(vertices);
}

/*
 * Checks if a point is on or near a line
 */
bool pointOnLine(int px, int py, int x1, int y1, int x2, int y2, int tolerance)
{
    float num = abs((y2 - y1) * px - (x2 - x1) * py + x2 * y1 - y2 * x1);
    float den = sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
    float distance = num / den;
    int minX = std::min(x1, x2) - tolerance;
    int maxX = std::max(x1, x2) + tolerance;
    int minY = std::min(y1, y2) - tolerance;
    int maxY = std::max(y1, y2) + tolerance;
    return distance <= tolerance && px >= minX && px <= maxX && py >= minY && py <= maxY;
}

/*
 * Checks if a point is inside a rectangle
 */
bool pointInRectangle(int px, int py, int x1, int y1, int x2, int y2)
{
    int minX = std::min(x1, x2);
    int maxX = std::max(x1, x2);
    int minY = std::min(y1, y2);
    int maxY = std::max(y1, y2);
    return px >= minX && px <= maxX && py >= minY && py <= maxY;
}

/*
 * Checks if a point is inside a triangle
 */
bool pointInTriangle(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3)
{
    float areaOrig = abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0f);
    float area1 = abs((px * (y2 - y3) + x2 * (y3 - py) + x3 * (py - y2)) / 2.0f);
    float area2 = abs((x1 * (py - y3) + px * (y3 - y1) + x3 * (y1 - py)) / 2.0f);
    float area3 = abs((x1 * (y2 - py) + x2 * (py - y1) + px * (y1 - y2)) / 2.0f);
    return abs(areaOrig - (area1 + area2 + area3)) < 0.0001f;
}

/*
 * Checks if a point is inside a polygon using ray-casting
 */
bool pointInPolygon(int px, int py, const std::list<Vertex> &vertices)
{
    int n = vertices.size();
    if (n < 3)
        return false;
    std::vector<Vertex> verts(vertices.begin(), vertices.end());
    int intersections = 0;
    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        int y1 = verts[i].y;
        int y2 = verts[j].y;
        int x1 = verts[i].x;
        int x2 = verts[j].x;
        if ((y1 <= py && py < y2) || (y2 <= py && py < y1))
        {
            if (x1 + (float)(py - y1) / (y2 - y1) * (x2 - x1) > px)
            {
                intersections++;
            }
        }
    }
    return (intersections % 2) == 1;
}

/*
 * Checks if a point is inside or on the boundary of a circle
 */
bool pointInCircle(int px, int py, int xc, int yc, int radius)
{
    float distance = sqrt(pow(px - xc, 2) + pow(py - yc, 2));
    return distance <= radius;
}