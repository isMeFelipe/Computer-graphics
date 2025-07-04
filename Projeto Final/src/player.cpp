#include "player.h"
#include "globals.h"
#include "game.h"
#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

// --- Variáveis Globais ---

float playerX = 100;
float playerY = 100;
float playerVelocityX = 0;
float playerVelocityY = 0;
const float moveSpeed = 5.0f;

bool playerOnGround = false;
bool ignorePlatform = false;
int ignoreTimer = 0;

bool keyLeftPressed = false;
bool keyRightPressed = false;
bool facingRight = true;

// --- Texturas ---
GLuint idleTexture;
GLuint walkTexture;
GLuint jumpTexture;

// --- Estados do Jogador ---
enum PlayerState
{
    Idle,
    Walking,
    Jumping,
    Falling
};
PlayerState playerState = Idle;

// --- Animação ---
int currentFrame = 0;
const int frameCountIdle = 1;
const int frameCountWalk = 3;
const int frameCountJump = 1;

int animationCounter = 0;
const int animationSpeed = 5; // Frames para troca do frame

// --- Protótipos ---
void playerKeyPress(unsigned char key, int x, int y);
void playerKeyRelease(unsigned char key, int x, int y);
void loadPlayerTexture();
GLuint loadTexture(const char *filename);

// --- Inicializa jogador ---
void initPlayer()
{
    playerX = 100;
    playerY = 100;
    playerVelocityX = 0;
    playerVelocityY = 0;
    playerOnGround = false;
    playerState = Idle;
    facingRight = true;
    currentFrame = 0;
    animationCounter = 0;
}

// --- Atualiza lógica do jogador ---
void updatePlayer()
{
    // Atualiza velocidade horizontal e direção
    if (keyLeftPressed)
    {
        playerVelocityX = -moveSpeed;
        facingRight = false;
    }
    else if (keyRightPressed)
    {
        playerVelocityX = moveSpeed;
        facingRight = true;
    }
    else
    {
        playerVelocityX = 0;
    }

    playerX += playerVelocityX;

    // Limita jogador na tela (considerando largura da tela e do player)
    if (playerX < 0)
        playerX = 0;
    if (playerX > 768) // 800 - 32 (PLAYER_WIDTH)
        playerX = 768;

    // Aplica gravidade e movimenta verticalmente
    playerVelocityY -= gravity;
    playerY += playerVelocityY;

    playerOnGround = false;

    // Timer para ignorar plataforma
    if (ignoreTimer > 0)
    {
        ignoreTimer--;
        if (ignoreTimer == 0)
            ignorePlatform = false;
    }

    // Checa colisão com plataformas se não estiver ignorando
    if (!ignorePlatform)
    {
        for (int i = 0; i < 5; i++)
        {
            Platform p = platforms[i];

            bool insideX = (playerX + 32 > p.x) && (playerX < p.x + p.width);
            bool hitFromAbove = (playerY <= p.y + p.height) && (playerY >= p.y + p.height - 10);
            if (insideX && hitFromAbove && playerVelocityY <= 0)
            {
                playerY = p.y + p.height;
                playerVelocityY = 0;
                playerOnGround = true;
            }
        }
    }

    // Limita chão (y = 0)
    if (playerY < 0)
    {
        playerY = 0;
        playerVelocityY = 0;
        playerOnGround = true;
    }

    // Define estado do jogador
    if (!playerOnGround)
    {
        playerState = (playerVelocityY > 0) ? Jumping : Falling;
    }
    else
    {
        if (keyLeftPressed || keyRightPressed)
            playerState = Walking;
        else
            playerState = Idle;
    }

    // Atualiza frame de animação
    animationCounter++;
    if (animationCounter > animationSpeed)
    {
        animationCounter = 0;
        currentFrame++;

        int maxFrames = 1;
        switch (playerState)
        {
        case Idle:
            maxFrames = frameCountIdle;
            break;
        case Walking:
            maxFrames = frameCountWalk;
            break;
        case Jumping:
        case Falling:
            maxFrames = frameCountJump;
            break;
        }

        if (currentFrame >= maxFrames)
            currentFrame = 0;
    }
}

// --- Renderiza jogador com espelhamento baseado na direção ---
void renderPlayer()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    GLuint currentTexture;
    int frameCount;

    switch (playerState)
    {
    case Idle:
        currentTexture = idleTexture;
        frameCount = frameCountIdle;
        break;
    case Walking:
        currentTexture = walkTexture;
        frameCount = frameCountWalk;
        break;
    case Jumping:
    case Falling:
        currentTexture = jumpTexture;
        frameCount = frameCountJump;
        break;
    }

    float frameWidth = 1.0f / frameCount;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, currentTexture);

    float u1 = currentFrame * frameWidth;
    float u2 = u1 + frameWidth;

    glBegin(GL_QUADS);
    if (facingRight)
    {
        glTexCoord2f(u1, 0.0f);
        glVertex2f(playerX, playerY);
        glTexCoord2f(u2, 0.0f);
        glVertex2f(playerX + PLAYER_WIDTH, playerY);
        glTexCoord2f(u2, 1.0f);
        glVertex2f(playerX + PLAYER_WIDTH, playerY + PLAYER_HEIGHT);
        glTexCoord2f(u1, 1.0f);
        glVertex2f(playerX, playerY + PLAYER_HEIGHT);
    }
    else
    {
        // Espelhamento horizontal
        glTexCoord2f(u2, 0.0f);
        glVertex2f(playerX, playerY);
        glTexCoord2f(u1, 0.0f);
        glVertex2f(playerX + PLAYER_WIDTH, playerY);
        glTexCoord2f(u1, 1.0f);
        glVertex2f(playerX + PLAYER_WIDTH, playerY + PLAYER_HEIGHT);
        glTexCoord2f(u2, 1.0f);
        glVertex2f(playerX, playerY + PLAYER_HEIGHT);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// --- Entrada do teclado ---
void playerKeyPress(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A')
        keyLeftPressed = true;
    else if (key == 'd' || key == 'D')
        keyRightPressed = true;
    else if (key == 'w' || key == 'W')
    {
        if (playerOnGround)
        {
            playerVelocityY = 16.0f; // Pulo
            playerOnGround = false;
        }
    }
    else if (key == 's' || key == 'S')
    {
        // Descer da plataforma
        ignorePlatform = true;
        playerOnGround = false;
    }
}

void playerKeyRelease(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A')
        keyLeftPressed = false;
    else if (key == 'd' || key == 'D')
        keyRightPressed = false;
    else if (key == 's' || key == 'S')
    {
        ignorePlatform = false;
        ignoreTimer = 30; // Tempo para ignorar colisão com plataforma
    }
}

// --- Função vazia para controle de teclas especiais, se quiser usar ---
void playerSpecialPress(int key)
{
    // vazio por enquanto
}

// --- Carrega texturas do jogador ---
void loadPlayerTexture()
{
    idleTexture = loadTexture("../assets/textures/player_idle.png");
    walkTexture = loadTexture("../assets/textures/player_walk.png");
    jumpTexture = loadTexture("../assets/textures/player_jump.png");
}

// --- Função auxiliar para carregar textura ---
GLuint loadTexture(const char *filename)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 4);

    if (!data)
    {
        std::cerr << "Erro ao carregar imagem: " << filename << "\n";
        return 0;
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    return texID;
}
