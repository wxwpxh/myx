/*编译指令   
   gcc test1.c -o test1 -lGL -lglut
*
* 画点
*/

#include <GL/glut.h>

void myDisplay(void)
{
     glClear(GL_COLOR_BUFFER_BIT);
     
	 glPointSize(5.0f);
     glBegin(GL_POINTS);
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
