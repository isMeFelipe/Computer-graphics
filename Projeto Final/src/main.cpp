#include <GL/glut.h>
#include "game.h"
#include "player.h"
#include "vilao.h"
#include "globals.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Dimensões atuais da janela, usadas para manter a proporção e posicionar a mini câmera corretamente
int windowWidth = 800;
int windowHeight = 600;

GLuint backgroundTextureID;

Mix_Chunk *hitSound = nullptr;
Mix_Music *bgMusic = nullptr;

void renderBackground(int x, int y, int width, int height)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundTextureID);
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + width, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + width, y + height);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Função principal de renderização
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha o fundo texturizado
    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundTextureID);
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);

    glTexCoord2f(1, 0);
    glVertex2f(windowWidth, 0);

    glTexCoord2f(1, 1);
    glVertex2f(windowWidth, windowHeight);

    glTexCoord2f(0, 1);
    glVertex2f(0, windowHeight);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Agora configura a câmera principal (como antes)
    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float cameraY = playerY - 300;
    if (cameraY < 0)
        cameraY = 0;

    gluOrtho2D(0, 800, cameraY, cameraY + 600);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderGame();

    // -------------------------------
    // MINI CÂMERA DO VILÃO (canto inferior direito)
    // -------------------------------

    int miniWidth = 200;
    int miniHeight = 150;

    // Posiciona a mini câmera no canto inferior direito da janela
    int miniX = 0;
    int miniY = windowHeight - miniHeight;

    glViewport(miniX, miniY, miniWidth, miniHeight);

    // Limpa apenas a área da mini viewport
    glEnable(GL_SCISSOR_TEST);
    glScissor(miniX, miniY, miniWidth, miniHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    // Salva a projeção principal e define uma nova para a mini câmera
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Define o que a mini câmera vai mostrar (região ao redor do vilão)
    gluOrtho2D(vilaoX - 150, vilaoX + 150, vilaoY - 50, vilaoY + 250);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderBackground(vilaoX - 150, vilaoY - 50, 300, 300);

    // Renderiza o vilão, projéteis e indicadores na mini câmera
    renderVilao();
    renderProjectiles();
    renderPreview();

    // Restaura a projeção e viewport da câmera principal
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, windowWidth, windowHeight);

    // Troca os buffers para exibir o frame renderizado
    glutSwapBuffers();
}

// Atualiza o estado do jogo e solicita novo redraw
void update(int value)
{
    updateGame();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Chama novamente em ~60 FPS
}

// Trata teclas especiais (usadas pelo vilão)
void specialKeyPress(int key, int x, int y)
{
    handleVilaoSpecialKeyPress(key);
}

// Ajusta a viewport e guarda novas dimensões da janela
void reshape(int width, int height)
{
    if (height == 0)
        height = 1;

    // Atualiza as dimensões globais
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);

    // Não definimos projeção fixa aqui — ela será atualizada dinamicamente no display()
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // Projeção neutra temporária

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void loadBackgroundTexture()
{
    backgroundTextureID = loadTexture("../assets/textures/background_1.png");
}

void initAudio()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf("SDL_Init erro: %s\n", SDL_GetError());
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("Mix_OpenAudio erro: %s\n", Mix_GetError());
        return;
    }

    Mix_Chunk *hitSound = Mix_LoadWAV("../assets/sounds/hit.wav");
    Mix_Music *bgMusic = Mix_LoadMUS("../assets/sounds/music_loop.mp3");

    if (!hitSound || !bgMusic)
    {
        printf("Erro ao carregar sons: %s\n", Mix_GetError());
    }

    Mix_PlayMusic(bgMusic, -1);
}

void cleanupAudio()
{
    Mix_FreeChunk(hitSound);
    Mix_FreeMusic(bgMusic);
    Mix_CloseAudio();
    SDL_Quit();
}

// Função principal
int main(int argc, char **argv)
{
    initAudio();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Donkey Kong Laranja");

    // Habilita transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Registra callbacks de entrada
    glutKeyboardFunc(playerKeyPress);
    glutKeyboardUpFunc(playerKeyRelease);
    glutSpecialFunc(specialKeyPress);
    glutReshapeFunc(reshape);

    // Inicializa lógica do jogo e texturas
    initGame();

    loadBackgroundTexture();
    loadProjectileTextures();
    loadVilaoTextures();

    // Registra funções principais do loop
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);

    // Inicia o loop principal
    glutMainLoop();


    return 0;
}
