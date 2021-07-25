/*
 *    gcc -o study1 study1.c  -lGL -lGLU -lglut
 *
 * */

#include<GL/glut.h>
void init(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
}

void display(void)
{
	GLdouble eqn[4] = {0.0,1.0,0.0,0.0};
	GLdouble eqn2[4] = {1.0,0.0,0.0,0.0};

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
	glTranslatef(0.0,0.0,-5.0);

	/* 截掉下半部分（y坐标小于0的部分）*/
	glClipPlane(GL_CLIP_PLANE0,eqn);
	glEnable(GL_CLIP_PLANE0);
	/* 截掉下半部分（y坐标小于0的部分）*/
	glClipPlane(GL_CLIP_PLANE1,eqn2);
	glEnable(GL_CLIP_PLANE1);

	glRotatef(90.0,1.0,0.0,0.0);
	glutWireSphere(1.0,20,16);
	glPopMatrix();
	glFlush();
	
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400,400);
	glutInitWindowPosition(200,200);
	glutCreateWindow("study");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
