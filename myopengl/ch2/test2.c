/*编译指令   
   gcc test2.c -o test2 -lGL -lglut -lm
 */

#include <GL/glut.h>

#include <math.h>
const int n = 20;
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;
void myDisplay(void)
{
     int i;
     glClear(GL_COLOR_BUFFER_BIT);
     glBegin(GL_POLYGON);
     for(i=0; i<n; ++i)
         glVertex2f(R*cos(2*Pi/n*i), R*sin(2*Pi/n*i));
     glEnd();
     glFlush();
}


int main(int argc, char *argv[])

{

     glutInit(&argc, argv);

     glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

     glutInitWindowPosition(100, 100);

     glutInitWindowSize(400, 400);

     glutCreateWindow("第一个OpenGL程序");

     glutDisplayFunc(&myDisplay);

     glutMainLoop();

     return 0;

}
