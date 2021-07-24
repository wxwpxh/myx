 /*编译指令 
 
 gcc clock.c -o clock -lGL -lGLU -lglut -lm 

 */

#include <GL/glut.h>
 /*本来OpenGL程序一般还要包含<GL/gl.h>和<GL/glu.h>，
    但GLUT的头文件中已经自动将这两个文件包含了，不必再次包含。 */
#include <unistd.h>
#include <math.h>
#include<time.h>


const GLfloat Pi = 3.1415926536;
const GLfloat R=0.8f;
const int n=200;
static GLfloat angle;

float Mysecond(struct tm *ptr)
{
    return ((Pi/2)-(((float)ptr->tm_sec)/60)*2*Pi);    
}

float Mymin(struct tm *ptr)
{
    //return (Pi/2)-(((ptr->tm_min+(((float)ptr->tm_sec)/60))/60)*2*Pi);
    return ((Pi/2)-((ptr->tm_min+ptr->tm_sec/60.0)/60)*2*Pi);
};

float Myhour(struct tm *ptr)
{
    if(0<ptr->tm_hour&&ptr->tm_hour<12)
    {
        return ((Pi/2)-((float)ptr->tm_hour+ptr->tm_min/60.0)/12*2*Pi);
    }else{
        return ((Pi/2)-((ptr->tm_hour-12.0+ptr->tm_min/60.0)/12)*2*Pi);
    }
};
void myDisplay(void)

{
    //glShadeModel(GL_SMOOTH);
    //获取系统时间
    struct tm *ptr;
    time_t it;
    it=time(NULL);
    ptr=localtime(&it);

    angle=2*Pi;
    glClear(GL_COLOR_BUFFER_BIT);//清除，GL_COLOR_BUFFER_BIT表示清除颜色
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     //钟盘
     glColor3f(0.5,0.5,0.5);
     glBegin(GL_POLYGON);
     for(int i=0;i<n;i++){
         glVertex2f(R*cos(2*Pi/n*i), R*sin(2*Pi/n*i));
     }
     glEnd();

     //刻度
     glColor3f(1.0,1.0,1.0);
      glPointSize(5.0f);
     glBegin(GL_POINTS);
     for(int j=0;j<12;j++)
     {
         glVertex2f(0.75*cos(2*Pi/12*j),0.75*sin(2*Pi/12*j));
     }
     glEnd();

     //时针
     glLineWidth(5.0f);
     glColor3f(0.0,0.0,0.0);
     
     glBegin(GL_LINES);
     glRotatef((angle/3600.0),0.0,0.0,1.0);
     glVertex2f(0.0,0.0);
     glVertex2f(cos(Myhour(ptr))*R*0.55,sin(Myhour(ptr))*R*0.55);
     glEnd();
     
     //分针
     glLineWidth(4.0f);
     glColor3f(0.0,0.0,0.0);
     
     glBegin(GL_LINES);
     glRotatef((angle/60.0),0.0,0.0,1.0);
     glVertex2f(0.0,0.0);
     glVertex2f(cos(Mymin(ptr))*R*0.65,sin(Mymin(ptr))*R*0.65);
     glEnd();

     //秒针
     glLineWidth(3.0f);
     glColor3f(0.0,0.0,0.0);
     
     glBegin(GL_LINES);
     glRotatef(angle,0.0,0.0,1.0);
     glVertex2f(0.0,0.0);
     glVertex2f(cos(Mysecond(ptr))*R*0.85,sin(Mysecond(ptr))*R*0.85);
     glEnd();
     glFlush();//glFlush，保证前面的OpenGL命令立即执行（而不是让它们在缓冲区中等待）。

}

void myIdle(void)
{
    angle-=((2*Pi)/60);
    sleep(1000);
    if(angle<0.0f){
        angle=2*Pi;
    }
    myDisplay();
}

int main(int argc, char *argv[])

{

     glutInit(&argc, argv);//glutInit，对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次。其格式比较死板，一般照抄这句glutInit(&argc, argv)就可以了。

     glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//设置显示方式,其中GLUT_RGB表示使用RGB颜色,与之对应的GLUT_INDEX表示使用索引颜色。GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）

     glutInitWindowPosition(300,100);//设置窗口在屏幕中的位置.

     glutInitWindowSize(400, 400);//设置窗口的大小

     glutCreateWindow("时钟");//根据前面设置的信息创建窗口。参数将被作为窗口的标题。注意：窗口被创建后，并不立即显示到屏幕上。需要调用glutMainLoop才能看到窗口。

     glutDisplayFunc(&myDisplay);//调用画图函数
     glutIdleFunc(&myIdle);

     glutMainLoop();//进行一个消息循环。这个函数可以显示窗口，并且等待窗口关闭后才会返回

     return 0;

}
