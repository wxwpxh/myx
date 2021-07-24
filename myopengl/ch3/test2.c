/*编译指令   
   gcc test2.c -o test2 -lGL -lglut
*
*  画虚线，并且指定线的宽度
*/

#include <GL/glut.h>

void myDisplay(void)
{
     glClear(GL_COLOR_BUFFER_BIT);

     glEnable(GL_LINE_STIPPLE);

     glLineStipple(2, 0x0F0F);

     glLineWidth(10.0f);

     glBegin(GL_LINES);

         glVertex2f(0.0f, 0.0f);

         glVertex2f(0.5f, 0.5f);

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
