/*
 * gcc -o reshape reshape.c -lGL -lGLU -lglut
 *
 * */
#include <GL/glut.h>

#include <stdlib.h>

void init(void)

{

glClearColor (0.0, 0.0, 0.0, 0.0);

glShadeModel (GL_FLAT);

glEnable(GL_DEPTH_TEST);

}

void display(void)

{

glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

glColor3f (0.5, 0.5, 0.5);

glRotatef (30, 0.0, 1.0, 0.0);

glutSolidCube(1);

glColor3f (1.0, 0.0, 1.0);

glutWireCube(1);

glutSwapBuffers();

}

void reshape (int w, int h)

{

glViewport (0, 0, (GLsizei) w, (GLsizei) h);

glMatrixMode (GL_PROJECTION);

glLoadIdentity ();

gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);

glMatrixMode(GL_MODELVIEW);

glLoadIdentity();

gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

int main(int argc, char** argv)

{

glutInit(&argc, argv);

glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

glutInitWindowSize (500, 500);

glutInitWindowPosition (100, 100);

glutCreateWindow (argv[0]);

init ();

glutDisplayFunc(display);

glutReshapeFunc(reshape);

glutMainLoop();

return 0;

}
