// g++ -o maze maze.C  -lGL -lGLU -lglut

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <list>
using namespace std;

#define mazeRow 50
#define mazeColum 75

const int UP = 1;
const int DOWN = 2;
const int LEFT = 4;
const int RIGHT = 8;
typedef struct coor {
	int i;
	int j;
}coor;

typedef list<coor> coorlist;
int mazeWidth = 600;
int mazeHeight = 400;
int pointSize = mazeWidth/mazeColum>mazeHeight/mazeRow?mazeHeight/mazeRow:mazeWidth/mazeColum;
char northWall[mazeRow][mazeColum],eastWall[mazeRow][mazeColum];

void mazeInit(void) {
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	glPointSize(4.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,640.0,0.0,480.0);
}

bool mazeIsPass(int i,int j) {
	if (i > 0) {
		if (northWall[i-1][j] == 1&&northWall[i][j] == 1) {
			if ( j > 0) {
				if (eastWall[i][j] == 1&&eastWall[i][j-1] == 1) {
					return false;
				}
			}
			else {
				if (northWall[i][j] == 1)
				  return false;
			}
		}
	}
	else {
		if (northWall[i][j] == 1) {
			if ( j > 0) {
				if (eastWall[i][j] == 1&&eastWall[i][j-1] == 1) {
					return false;
				}
			}
			else {
				if (eastWall[i][j] == 1)
				  return false;
			}
		}
	}
	return true;
}

int surround(int i,int j) {
	int res = 0;
	if(i > 0) {
		if(!mazeIsPass(i-1,j))
		  res += DOWN;
	}
	if(i < mazeRow-1) {
		if(!mazeIsPass(i+1,j))
		  res += UP;
	}
	if(j < mazeColum-1) {
		if(!mazeIsPass(i,j+1))
		  res += RIGHT;
	}
	if(j > 0) {
		if(!mazeIsPass(i,j-1))
		  res += LEFT;
	}
	return res;
}




void eatWall() {
	coorlist mylist;//用于存储候选的单元格
	int i = rand()%mazeRow;
	int j = rand()%mazeColum;
	coor c;
	int res;
	do {
		do {
			res = surround(i,j);
			switch(res) {
				case UP:
					northWall[i][j] = 0;
					i = i+1;
					break;
				case DOWN:
					northWall[i-1][j] = 0;
					i = i-1;
					break;
				case LEFT:
					eastWall[i][j-1] = 0;
					j = j-1;
					break;
				case RIGHT:
					eastWall[i][j] = 0;
					j = j+1;
					break;
				case UP+DOWN:
					switch(rand()%2) {
						case 0:
							northWall[i][j] = 0;
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							i = i+1;
							break;
						case 1:
							northWall[i-1][j] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							i = i-1;
							break;
					}
					break;
				case UP+RIGHT:
					switch(rand()%2) {
						case 0:
							northWall[i][j] = 0;
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							i = i+1;
							break;
						case 1:
							eastWall[i][j] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							j = j+1;
							break;
					}
					break;
				case UP+LEFT:
					switch(rand()%2) {
						case 0:
							northWall[i][j] = 0;
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							i = i+1;
							break;
						case 1:
							eastWall[i][j-1] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							j = j-1;
							break;
					}
					break;
				case DOWN+LEFT:
					switch(rand()%2) {
						case 0:
							northWall[i-1][j] = 0;
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							i = i-1;
							break;
						case 1:
							eastWall[i][j-1] = 0;
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							j = j-1;
							break;
					}
					break;
				case DOWN+RIGHT:
					switch(rand()%2) {
						case 0:
							northWall[i-1][j] = 0;
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							i = i-1;
							break;
						case 1:
							eastWall[i][j] = 0;
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							j = j+1;
							break;
					}
					break;
				case LEFT+RIGHT:
					switch(rand()%2) {
						case 0:
							eastWall[i][j-1] = 0;
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							j = j-1;
							break;
						case 1:
							eastWall[i][j] = 0;
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							j = j+1;
							break;
					}
					break;
				case DOWN+LEFT+RIGHT:
					switch(rand()%3) {
						case 0:
							northWall[i-1][j] = 0;
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							i = i-1;
							break;
						case 1:
							eastWall[i][j-1] = 0;
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							j = j-1;
							break;
						case 2:
							eastWall[i][j] = 0;
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							j = j+1;
							break;
					}
					break;
				case UP+LEFT+RIGHT:
					switch(rand()%3) {
						case 0:
							northWall[i][j] = 0;
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							i = i+1;
							break;
						case 1:
							eastWall[i][j-1] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							j = j-1;
							break;
						case 2:
							eastWall[i][j] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							j = j+1;
							break;
					}
					break;
				case UP+DOWN+RIGHT:
					switch(rand()%3) {
						case 0:
							northWall[i][j] = 0;
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							i = i+1;
							break;
						case 1:
							northWall[i-1][j] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							i = i-1;
							break;
						case 2:
							eastWall[i][j] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							j = j+1;
							break;
					}
					break;
				case UP+DOWN+LEFT:
					switch(rand()%3) {
						case 0:
							northWall[i][j] = 0;
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							i = i+1;
							break;
						case 1:
							northWall[i-1][j] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							i = i-1;
							break;
						case 2:
							eastWall[i][j-1] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							j = j-1;
							break;
					}
					break;
				case UP+DOWN+LEFT+RIGHT:
					switch(rand()%4) {
						case 0:
							northWall[i][j] = 0;
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							i = i+1;
							break;
						case 1:
							northWall[i-1][j] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							i = i-1;
							break;
						case 2:
							eastWall[i][j-1] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j+1;
							mylist.push_front(c);
							j = j-1;
							break;
						case 3:
							eastWall[i][j] = 0;
							c.i = i+1;c.j = j;
							mylist.push_front(c);
							c.i = i-1;c.j = j;
							mylist.push_front(c);
							c.i = i;c.j = j-1;
							mylist.push_front(c);
							j = j+1;
							break;
					}
					break;
			}
		}while (surround(i,j) != 0);
		do{
			c = mylist.front();
			mylist.pop_front();
		}while(mazeIsPass(c.i,c.j)&&mylist.size() != 0);
		i = c.i;j = c.j;
	}while(mylist.size() != 0);
}

void mazeDisplay() {
	int i,j;
	int xBegin = 20,yBegin = 20;
	for(i = 0;i < mazeRow;i++) {
		for(j = 0;j < mazeColum;j++){
			northWall[i][j] = 1;
			eastWall[i][j] = 1;
		}
	}
	eatWall();
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINES);
	for(i = 0;i < mazeRow;i++) {
		for(j = 0;j < mazeColum;j++){
			if (northWall[i][j] == 1) {
				glVertex2i((int)xBegin+j*1.0/mazeColum*mazeWidth-pointSize/2,(int)yBegin+i*1.0/mazeRow*mazeHeight+pointSize/2);
				glVertex2i((int)xBegin+j*1.0/mazeColum*mazeWidth+pointSize/2,(int)yBegin+i*1.0/mazeRow*mazeHeight+pointSize/2);
			}
			if (eastWall[i][j] == 1) {
				glVertex2i((int)xBegin+j*1.0/mazeColum*mazeWidth+pointSize/2,(int)yBegin+i*1.0/mazeRow*mazeHeight-pointSize/2);
				glVertex2i((int)xBegin+j*1.0/mazeColum*mazeWidth+pointSize/2,(int)yBegin+i*1.0/mazeRow*mazeHeight+pointSize/2);
			}
		}
	}
	glVertex2i(xBegin-pointSize/2,yBegin-pointSize/2);
	glVertex2i(xBegin+mazeWidth-pointSize/2,yBegin-pointSize/2);
	glVertex2i(xBegin-pointSize/2,yBegin-pointSize/2);
	glVertex2i(xBegin-pointSize/2,yBegin+mazeHeight-pointSize/2);
	glEnd();
	glFlush();
}



int main(int argc,char** argv) {

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("迷宫");
	glutDisplayFunc(mazeDisplay);
	mazeInit();
	glutMainLoop();

	return 0;

}
