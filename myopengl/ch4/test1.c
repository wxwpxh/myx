/*编译指令   
   gcc test1.c -o test1 -lGL -lglut
*
*  OpenGL支持两种颜色模式：一种是RGBA，一种是颜色索引模式.这里是第一种
*/

#include <GL/glut.h>

void myDisplay(void)

{

     glClear(GL_COLOR_BUFFER_BIT);
     glColor3f(0.0f, 1.0f, 1.0f);
     glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
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
