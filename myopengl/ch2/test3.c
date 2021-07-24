/*编译指令   
   gcc test3.c -o test3 -lGL -lglut -lm
 */

#include <GL/glut.h>

#include <math.h>
const GLfloat Pi = 3.1415926536f;
void myDisplay(void)
{
     GLfloat a = 1 / (2-2*cos(72*Pi/180));
     GLfloat bx = a * cos(18 * Pi/180);
     GLfloat by = a * sin(18 * Pi/180);
     GLfloat cy = -a * cos(18 * Pi/180);
     GLfloat
         PointA[2] = { 0, a },
         PointB[2] = { bx, by },
         PointC[2] = { 0.5, cy },
         PointD[2] = { -0.5, cy },
         PointE[2] = { -bx, by };

     glClear(GL_COLOR_BUFFER_BIT);
     // 按照A->C->E->B->D->A的顺序，可以一笔将五角星画出
     glBegin(GL_LINE_LOOP);
         glVertex2fv(PointA);
         glVertex2fv(PointC);
         glVertex2fv(PointE);
         glVertex2fv(PointB);
         glVertex2fv(PointD);
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
