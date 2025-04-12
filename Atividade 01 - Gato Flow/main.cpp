#ifdef __APPLE__ // MacOS
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else // Windows e Linux
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <cmath>
#include <GL/glut.h>


void catHead(){
    // Bola da cabe�a
    glTranslatef(-0.9f, -0.5f, -5.0f);
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
    glutSolidSphere(0.8, 50, 50);

    // Orelhas ( base)
    glTranslatef(-1.5f, 1.0f, -5.0f);
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);
        glVertex3f(-1.0f, -2.0f, 0.0f);  
        glVertex3f( 1.0f, -1.0f, 0.0f);  
        glVertex3f( -1.0f,  2.0f, 0.0f); 
    glEnd();    

    glTranslatef(0.9f, 1.0f, -5.0f);
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);
        glVertex3f(-1.0f, -1.0f, 0.0f);  
        glVertex3f(1.0f, -2.0f, 0.0f);  
        glVertex3f( 1.0f,  2.0f, 0.0f); 
    glEnd();    

    // Orelhas ( contorno interno)
    glTranslatef(-0.3f, -1.0f, 1.0f);
    glColor4f(0.4f, 0.2f, 0.2f, 1.0f);

    glBegin(GL_TRIANGLES);
        glVertex3f(-0.8f, -1.0f, 0.0f);  
        glVertex3f(1.0f, -2.0f, 0.0f);  
        glVertex3f( 1.2f,  2.3f, 0.0f); 
    glEnd();   
    
    glTranslatef(-1.5f, 1.0f, 5.0f);
    glColor4f(0.4f, 0.2f, 0.2f, 1.0f);

    glBegin(GL_TRIANGLES);
        glVertex3f(0.8f, -2.0f, 1.0f);  
        glVertex3f(2.0f, -2.0f, 1.0f);  
        glVertex3f(0.7f,  0.1f, 1.0f);  
    glEnd();
    
    // Olhos
	glTranslatef(2.5f, -1.8f, 6.0f);
    glColor4f(0.796f, 0.608f, 0.278f, 1.0f);
    glutSolidSphere(0.15, 50, 50);
    
    glTranslatef(0.5f, 0.0f, 0.0f); 
	glutSolidSphere(0.15, 50, 50);
	
	// Pupilas (olho esquerdo)
    glTranslatef(-0.016f, 0.01f, 0.2f); 
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.1, 50, 50);
	
    glTranslatef(0.09f, 0.08f, 0.4f); 
    glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glutSolidSphere(0.01, 50, 50);
	
    glTranslatef(0.01f, 0.01f, 0.4f); 
    glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glutSolidSphere(0.01, 50, 50);
	
	// Pupilas (olho direito)
    glTranslatef(-0.25f, 0.01f, 0.2f); 
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.07, 50, 50);
	
    glTranslatef(0.075f, 0.065f, 0.4f); 
    glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glutSolidSphere(0.008, 50, 50);
	
    glTranslatef(0.13f, 0.022f, 0.4f); 
    glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glutSolidSphere(0.006, 50, 50);
	
	
	glPushMatrix();
	
	// Nariz
	glTranslatef(-0.012, -0.08f, -0.5f); 
    glColor4f(0.4f, 0.2f, 0.2f, 1.0f);
	
	// Parte horizontal do T do nariz
	glBegin(GL_QUADS);
	    glVertex3f(-0.05f, -0.08f, 0.0f); 
	    glVertex3f( 0.05f, -0.08f, 0.0f); 
	    glVertex3f( 0.05f,  -0.05f, 0.0f);
	    glVertex3f(-0.05f,  -0.05f, 0.0f);
	glEnd();

    glPopMatrix();
	
	// Parte Vertical
    glTranslatef(-0.005, -0.12, -0.5f); 
    glColor4f(0.4f, 0.2f, 0.2f, 1.0f);
	
	glBegin(GL_QUADS);
	    glVertex3f(-0.025f, -0.06f, 0.0f);  
	    glVertex3f( 0.015f, -0.06f, 0.0f);  
	    glVertex3f( 0.015f,  -0.025f, 0.0f);
	    glVertex3f(-0.025f,  -0.025f, 0.0f);
	glEnd();
	
	glPushMatrix();
	// Narinas
    glTranslatef(0.05f, 0.028f, 0.4f); 
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.006, 50, 50);
	
	glPopMatrix();
	
	    glTranslatef(0.085f, 0.028f, 0.4f); 
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.006, 50, 50);
	
	glPopMatrix();
}

void title() {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.3f, -0.2f, -5.0f);
    glScalef(0.1f, 0.1f, 1.0f);

    glColor3f(0.0f, 0.0f, 0.0f);

    // ----- Letra F -----
    glPushMatrix();

    glBegin(GL_QUADS);
        glVertex2f(-0.2f, 0.0f);
        glVertex2f(5.0f, 0.0f);
        glVertex2f(5.0f, 1.0f);
        glVertex2f(-0.2f, 1.0f);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex2f(0.8f, -4.0f);
        glVertex2f(2.0f, -4.0f);
        glVertex2f(2.0f, 0.0f);
        glVertex2f(0.8f, 1.0f);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex2f(-0.2f, -2.0f);
        glVertex2f(4.0f, -2.0f);
        glVertex2f(4.0f, -1.0f);
        glVertex2f(-0.2f, -1.0f);
    glEnd();

    glPopMatrix();

    // ----- Letra L -----
    
    glPushMatrix();
    glTranslatef(15.0f, -1.0f, -5.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.2f, -9.0f);
        glVertex2f(2.0f, -9.0f);
        glVertex2f(2.0f, 1.0f);
        glVertex2f(-0.2f, 1.0f);
    glEnd();

    glPopMatrix();
    
    // ----- Letra O -----
    
    glPushMatrix();
	glLoadIdentity();
	glTranslatef(1.3f, -0.4f, -5.0f);
	glutSolidSphere(0.25, 50, 50);
	
	glTranslatef(-0.08f, 0.01f, 0.2f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.125, 50, 50);		
	glPopMatrix();
    
	glPushMatrix();
	glLoadIdentity();		    
    // ----- Letra W -----
	glTranslatef(1.72f, -0.7f, -5.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.1f, 0.1f);
        glVertex2f(0.1f, 0.1f);
        glVertex2f(0.1f, 0.5f);
        glVertex2f(-0.1f, 0.5f);
    glEnd();
    
    glLoadIdentity();
 	glTranslatef(2.1f, -0.7f, -5.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.1f, 0.1f);
        glVertex2f(0.1f, 0.1f);
        glVertex2f(0.1f, 0.5f);
        glVertex2f(-0.1f, 0.5f);
    glEnd();
    
    
    glLoadIdentity();
 	glTranslatef(1.90f, -0.35f, -5.0f);
    
	glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, -0.3f, 0.0f);
	
	glBegin(GL_QUADS);
	    glVertex2f(-0.05f, 0.05f);
	    glVertex2f( 0.05f, 0.05f);
	    glVertex2f( 0.05f, 0.2f);
	    glVertex2f(-0.05f, 0.4f);
	glEnd();
	
	
    glLoadIdentity();
 	glTranslatef(1.90f, -0.36f, -5.0f);
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, -0.3f, 0.0f);
	
	glBegin(GL_QUADS);
	    glVertex2f(-0.05f, 0.05f);
	    glVertex2f( 0.05f, 0.05f);
	    glVertex2f( 0.05f, 0.4f);
	    glVertex2f(-0.07f, 0.2f);
	glEnd();


glPopMatrix();

	
	glPopMatrix();
	
    glPopMatrix();
}



void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

	catHead();
	title();

    glutSwapBuffers();
}


void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0); // Fundo branco
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Atividade 01 - Gato Flow");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
