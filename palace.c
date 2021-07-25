/* 
 *  gcc -std=c99 -o palace palace.c -lGL -lGLU -lglut -lm
 *
 * */

#include <GL/glut.h>
#include <math.h>

const float r = 100.0;

void arctestInit() {
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-200,640,-150,480);
}

void arctestDisplay() {

 	float h = r*0.618;
 	float r2 = 0.8*r;
 	float y2 = sqrt(pow((r+r2),2)-pow(r2,2));
 	float x2 = r2*r/(r+r2);

	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINE_STRIP);
	//先画下面的长方形
	glVertex2f(-r,0);
	glVertex2f(-r,-h);
	glVertex2f(r,-h);
	glVertex2f(r,0);
	//再画圆弧
	for(float x = r;x > x2;x -= 0.01) {
		glVertex2f(x,sqrt(pow(r,2)-pow(x,2)));
	}
	
	for(float x = x2;x > 0;x -= 0.01) {
		glVertex2f(x,y2-sqrt(pow(r2,2)-pow((r2-x),2)));
	}
	
	for(float x = 0;x > -x2;x -= 0.01) {
		glVertex2f(x,y2-sqrt(pow(r2,2)-pow((r2+x),2)));
	}
	
	for(float x = -x2;x > -r;x -= 0.01) {
		glVertex2f(x,sqrt(pow(r,2)-pow(x,2)));
	}
	
	glEnd();
	glFlush();
}


 


int main(int argc,char** argv) {

	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("绘制圆弧");
	glutDisplayFunc(arctestDisplay);
	arctestInit();
	glutMainLoop();

	return 0;
}
