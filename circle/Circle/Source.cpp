#include <gl/glut.h>
#include <math.h>

class Circle {
public: float x, y;

public:
	void createCircle(int r);

};

void Circle::createCircle(int r) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 180; i++){
		float angle = 2.0f * 3.1415926f * float(i) / float(180);
		float x = r * cosf(angle);
		float y = r * sinf(angle);

		glVertex2f(x, y);

	}
	glEnd();
}

void display(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -20);
	glColor3f(1, 1, 1);
	Circle c;
	c.createCircle(10);

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Circle");

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}