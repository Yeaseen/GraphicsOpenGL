#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

#define pCos cos(pi/90)
#define nCos cos(-pi/90)
#define pSin sin(pi/90)
#define nSin sin(-pi/90)
class Point
{
public:
	double x,y,z;

	Point(){
	}
	Point(double a, double b, double c){
        x = a;
        y = b;
        z = c;
	}
	~Point(){}

	double dot(Point v){
        return x*v.x + y*v.y + z*v.z;
	}

	Point operator+(Point pt) {
        return Point(x + pt.x, y + pt.y, z + pt.z);
    }

    Point operator-(Point pt) {
        return Point(x - pt.x, y - pt.y, z - pt.z);
    }

    Point operator*(double v) {

        return Point(x*v, y*v, z*v);
    }

    Point operator*(Point v){
        return Point(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
    }

    Point operator/(double pt) {

        return Point(x/pt, y/pt, z/pt);
    }


    Point normalize() {
        return *this / sqrt(x*x + y*y + z*z);
    }
};

Point pos(0, -100, 10);
Point u(0,0,1);
Point r(1, 0, 0);
Point l(0, 1, 0);
Point temp(0,0,0);
int drawgrid;
int drawaxes;
double tempVal;



void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
            temp.x = l.x*pCos - r.x*pSin;
            temp.y = l.y*pCos - r.y*pSin;
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;

            r.x = r.x*pCos + l.x*pSin;
            r.y = r.y*pCos + l.y*pSin;
            tempVal = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);
            r.x /= tempVal;
            r.y /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
			break;

        case '2':

            temp.x = l.x*nCos - r.x*nSin;
            temp.y = l.y*nCos - r.y*nSin;
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;

            r.x = r.x*nCos + l.x*nSin;
            r.y = r.y*nCos + l.y*nSin;
            tempVal = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);
            r.x /= tempVal;
            r.y /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
			break;
        case '3':
            temp.x = l.x*pCos + u.x*pSin;
            temp.y = l.y*pCos + u.y*pSin;
            temp.z = l.z*pCos + u.z*pSin;
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            u.x = u.x*pCos - l.x*pSin;
            u.y = u.y*pCos - l.y*pSin;
            u.z = u.z*pCos - l.z*pSin;
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
            l.z = temp.z;

			break;
        case '4':
            temp.x = l.x*nCos + u.x*nSin;
            temp.y = l.y*nCos + u.y*nSin;
            temp.z = l.z*nCos + u.z*nSin;
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            u.x = u.x*nCos - l.x*nSin;
            u.y = u.y*nCos - l.y*nSin;
            u.z = u.z*nCos - l.z*nSin;
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
            l.z = temp.z;

			break;
        case '5':
            temp.x = r.x*pCos - u.x*pSin;
            temp.y = r.y*pCos - u.y*pSin;
            temp.z = r.z*pCos - u.z*pSin;
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            u.x = u.x*pCos + r.x*pSin;
            u.y = u.y*pCos + r.y*pSin;
            u.z = u.z*pCos + r.z*pSin;
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            r.x = temp.x;
            r.y = temp.y;
            r.z = temp.z;
			break;
        case '6':
            temp.x = r.x*nCos - u.x*nSin;
            temp.y = r.y*nCos - u.y*nSin;
            temp.z = r.z*nCos - u.z*nSin;
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;


            u.x = u.x*nCos + r.x*nSin;
            u.y = u.y*nCos + r.y*nSin;
            u.z = u.z*nCos + r.z*nSin;
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            r.x = temp.x;
            r.y = temp.y;
            r.z = temp.z;

			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			pos.x -= 2.0*l.x;
			pos.y -= 2.0*l.y;

			break;
		case GLUT_KEY_UP:		// up arrow key
			pos.x += 2.0*l.x;
			pos.y += 2.0*l.y;
			break;

		case GLUT_KEY_RIGHT:
			pos.x += 2.0*r.x;
			pos.y += 2.0*r.y;
			break;
		case GLUT_KEY_LEFT:
			pos.x -= 2.0*r.x;
			pos.y -= 2.0*r.y;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.z += 2.0;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.z -= 2.0;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
           /* squareLength = (squareLength/2) > 0? squareLength-2:squareLength;
            circleRadius = circleRadius < 19? circleRadius+1:circleRadius;*/
			break;
		case GLUT_KEY_END:
		    /*squareLength = (squareLength/2) < 16? squareLength+2:squareLength;
            circleRadius = circleRadius > 0? circleRadius-1:circleRadius;*/
			break;


		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();






	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;



	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
