/*
 *
 *  gcc -o bezier bezier.c  -lGL -lGLU -lglut
 *   libGL.so libGLU.so
 * */
#include <GL/glut.h>

GLfloat ctrlpoints[4][3] =

{{ -4.0, -4.0, 0.0}, { -2.0, 3.0, 0.0},

{2.0, 4.5, 0.0}, {3.0, -3.0, 0.0}};

void init(void)

{

glClearColor(1.0, 1.0, 1.0, 0.0);

glShadeModel(GL_FLAT);

//下行用于定义曲线函数

glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);

glEnable(GL_MAP1_VERTEX_3); //将当前曲线函数激活

}

void display(void)

{

int i;

glClear(GL_COLOR_BUFFER_BIT);

//下面用求值器按20等分计算Bezier曲线上的点

glColor3f(0.0, 0.0, 0.0);

glLineWidth(2);

glBegin(GL_LINE_STRIP);

for (i = 0; i <= 20; i++)

glEvalCoord1f((GLfloat) i/20.0); //相当于调用了glVertex*()

glEnd();

//下面绘制控制多边形

glLineWidth(1);

glColor3f(0.0, 0.0, 1.0);

glBegin(GL_LINE_STRIP);

for (i = 0; i < 4; i++)

glVertex3fv(&ctrlpoints[i][0]);

glEnd();

glFlush();

}

void reshape(int w, int h)

{

glViewport(0, 0, (GLsizei) w, (GLsizei) h);

glMatrixMode(GL_PROJECTION);

glLoadIdentity();

if (w <= h)

glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w, 5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);

else

glOrtho(-5.0*(GLfloat)w/(GLfloat)h, 5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);

glMatrixMode(GL_MODELVIEW);

glLoadIdentity();

}

int main(int argc, char** argv)

{

glutInit(&argc, argv);

glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

glutInitWindowSize (500, 500);

glutInitWindowPosition (100, 100);

glutCreateWindow (argv[0]);

init ();

glutDisplayFunc(display);

glutReshapeFunc(reshape);

glutMainLoop();

return 0;

}
