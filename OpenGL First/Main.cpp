#include <glut.h>
#include <math.h>
#include <stdio.h>
#define RED 0
#define GREEN 1
#define BLUE 2
#define ORANGE 3
#define YELLOW 4
#define PINK 5


GLfloat angle = 0.0f;

GLfloat increment = 0.0f;
bool expanding = true;
GLfloat r = 0.0f;GLfloat g = 0.0f;GLfloat b = 1.0f;
void initGL() {
	// Set "clearing" or background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}
void idle() {
	glutPostRedisplay();   // Post a re-paint request to activate display()
}
void pickRGB(int rgbPicker) {
	switch (rgbPicker)
	{
		case RED:
			r = 1.0f; g = 0.0f; b = 0.0f;
			break;
		case GREEN:
			r = 0.0f; g = 1.0f; b = 0.0f;
			break;
		case BLUE:
			r = 0.0f; g = 0.0f; b = 1.0f;
			break;
		case ORANGE:
			r = 1.0f; g = 0.5f; b = 0.0f;
			break;
		case YELLOW:
			r = 1.0f; g = 1.0f; b = 0.0f;
			break;
		case PINK:
			r = 1.0f; g = 0.0f; b = 1.0f;
			break;
	}
}
void baseTriangle(GLfloat height) {
	//base triangle
	glBegin(GL_TRIANGLES);	
		glColor3f(r,g,b);  // Red
		glVertex2f(0.0f, 0.0f);     // Define vertices in counter-clockwise (CCW) order
		glVertex2f(height, 0.0f);     //  so that the normal (front-face) is facing you
		glVertex2f(0.0f, height);
	glEnd();
}

void applyOperations(GLfloat transX, GLfloat transY, int dir) {
	
	GLfloat height = 0.2f;
	GLfloat radius = (2.0f*height) / (2.0f+sqrt(2.0f));
	GLfloat radiusProjection = radius / sqrt(2.0f);
	glPushMatrix();
		glTranslatef(transX+ radiusProjection, transY+ radiusProjection, 0.0f);
		glRotatef((dir * angle), 0.0f, 0.0f, 1.0f);
		glTranslatef(-radiusProjection, -radiusProjection, 0.0f);
		baseTriangle(height);	
	glPopMatrix();
	
}
void setSection() {
	GLfloat height = 0.2f;
	pickRGB(BLUE);
	glPushMatrix();
		glRotatef((-angle), 0.0f, 0.0f, 1.0f);
		applyOperations(0.0f + increment, 0.0f + increment,-1);
	glPopMatrix();
	pickRGB(ORANGE);
	glPushMatrix();
		glRotatef((angle), 0.0f, 0.0f, 1.0f);
		applyOperations(height + increment,0.0f + increment,1);
	glPopMatrix();
	pickRGB(GREEN);
	glPushMatrix();
		glRotatef((angle), 0.0f, 0.0f, 1.0f);
		applyOperations(0.0f + increment, height + increment, 1);
	glPopMatrix();
	pickRGB(RED);
	glPushMatrix();
	glRotatef((-2*angle), 0.0f, 0.0f, 1.0f);
	applyOperations(0.0f + increment, 2 * height + increment, 1);
	glPopMatrix();
	pickRGB(PINK);
	glPushMatrix();
	glRotatef((-2*angle), 0.0f, 0.0f, 1.0f);
	applyOperations(2 * height + increment,increment, 1);
	glPopMatrix();
	pickRGB(YELLOW);
	glPushMatrix();
	glRotatef((-2*angle), 0.0f, 0.0f, 1.0f);
	applyOperations(height + increment, height + increment, 1);
	glPopMatrix();
	//applyOperations(0.0f + increment, 2 * height + increment);
	//applyOperations(height + increment, height + increment);
	//applyOperations(height + increment, 0.0f + increment);
	//applyOperations(2 * height + increment, 0.0f + increment);
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);    // Clear the color buffer
	glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
	glLoadIdentity();                // Reset the model-view matrix
	glPushMatrix();
	//glRotatef(angle, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 4; i++) {
		glRotatef(90.0f*i, 0.0f, 0.0f, 1.0f);
		setSection();
	}
	glPopMatrix();
	glFlush();   // Render now
	glutSwapBuffers();
	angle += 0.01f;
	if (angle >= 0.0f && angle <= 180.f) increment += 0.00001f;
	else if (angle > 180.0f && angle < 360.0f) increment -= 0.00001f;
	else angle = 0.0f;
	
}
void reshape(GLsizei width, GLsizei height) {  // GLsizei for unsigned integer
   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);
	
	// Set the aspect ratio of the clipping area to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset the projection matrix
	if (width >= height) {
		// aspect >= 1, set the height from -1 to 1, with larger width
		gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
	}
	else {
		// aspect < 1, set the width to -1 to 1, with larger height
		gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);//aspect is < 1
	}
}
/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE);
	glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);
	//glutInitWindowSize(320, 320);   // Set the window's initial width & height
	//glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);             // Register callback handler if no other event
	initGL();
	glutMainLoop();           // Enter the event-processing loop
	return 0;
}