/*编译指令   
   gcc test4.c -o test4 -lGL -lglut
*
* 
*/

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
void myDisplay(void)
{
     static GLubyte Mask[128];
     FILE *fp;

     fp = fopen("1.bmp", "rb");
     if( !fp )
         exit(0);

     if( fseek(fp, -(int)sizeof(Mask), SEEK_END) )
         exit(0);

     if( !fread(Mask, sizeof(Mask), 1, fp) )
         exit(0);

     fclose(fp);

     glClear(GL_COLOR_BUFFER_BIT);
     glEnable(GL_POLYGON_STIPPLE);
     glPolygonStipple(Mask);
     glRectf(-0.5f, -0.5f, 0.0f, 0.0f);   // 在左下方绘制一个有镂空效果的正方形
     glDisable(GL_POLYGON_STIPPLE);
     glRectf(0.0f, 0.0f, 0.5f, 0.5f);     // 在右上方绘制一个无镂空效果的正方形
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
