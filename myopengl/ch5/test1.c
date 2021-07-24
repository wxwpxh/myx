/* 
	gcc test1.c -o test1 -lGL -lglut
*/
#include <GL/glut.h>

/* 太阳、地球和月亮
   假设每个月都是30天
   一年12个月，共是360天
*/
static int day = 200; /* day的变化：从0到359 */
void myDisplay(void)
{
     /****************************************************
      这里的内容照搬上一课的，只因为使用了双缓冲，补上最后这句
     *****************************************************/
     glutSwapBuffers();
}

void myIdle(void)
{
     /* 新的函数，在空闲时调用，作用是把日期往后移动一天并重新绘制，达到动画效果 */
     ++day;
     if( day >= 360 )
         day = 0;
     myDisplay();
}

int main(int argc, char *argv[])
{
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); /* 修改了参数为GLUT_DOUBLE */
     glutInitWindowPosition(100, 100);
     glutInitWindowSize(400, 400);
     glutCreateWindow("太阳，地球和月亮");    /* 改了窗口标题 */
     glutDisplayFunc(&myDisplay);
     glutIdleFunc(&myIdle);                /* 新加入了这句 */
     glutMainLoop();

     return 0;
}
