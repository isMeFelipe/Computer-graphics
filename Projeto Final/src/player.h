void initPlayer();
void updatePlayer();
void renderPlayer();
void playerKeyPress(unsigned char key, int x, int y);
void playerKeyRelease(unsigned char key, int x, int y);
void playerSpecialPress(int key);

extern float playerX, playerY;
extern float playerVelocityY;
extern float playerVelocityX;
extern bool playerOnGround;
extern bool ignorePlatform;
extern int ignoreTimer;