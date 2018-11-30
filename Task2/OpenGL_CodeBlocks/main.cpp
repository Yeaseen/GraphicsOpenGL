#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

using namespace std;
struct point2d
{
    double x, y;
};




struct point2d cp[200];


int cpidx;

int drawaxes;

int initialState;

int drawCurveState;

int upStateOne;
int changedCounter;
double changedX;
double changedY;
int upStateOneEffect;
int upStateTwo;




void drawSquare()
{
    glBegin(GL_QUADS);
    {
        glVertex3d( 3,  3, 0);
        glVertex3d( 3, -3, 0);
        glVertex3d(-3, -3, 0);
        glVertex3d(-3,  3, 0);
    }
    glEnd();
}

void drawBigSquare()
{
    glBegin(GL_QUADS);
    {
        glVertex3d( 6,  6, 0);
        glVertex3d( 6, -6, 0);
        glVertex3d(-6, -6, 0);
        glVertex3d(-6,  6, 0);
    }
    glEnd();
}


void drawPointer(int rad)
{

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 0,0,0);
			glVertex3f( rad-(rad/4),0,0);

			}glEnd();

		glBegin(GL_TRIANGLES);
        {
            glColor3f(1.0, 0, 0);
            glVertex3f(rad,0,0);
			glVertex3f(rad-(rad/4),4,0);
			glVertex3f(rad-(rad/4),-4,0);
        }
        glEnd();
}


void drawAxes(double startX,double startY,double endX,double endY)
{
	if(drawaxes==1)
	{

		double adj,angle1;
		adj = sqrt(pow((startX-endX),2) + pow((startY-endY),2));

		angle1=atan2((endY-startY),(endX-startX));
        angle1=(angle1*180)/pi;

    glPushMatrix();
    {
    glTranslatef(startX,0,0);
    glTranslatef(0,startY,0);
    glRotatef(angle1,0,0,1);
    drawPointer(adj);
    }
    glPopMatrix();


	}
}

void drawHermitCurve(int p0x,int p0y,int p1x,int p1y,int p2x,int p2y,int p3x,int p3y){

    double geoMat[4][2]={
        {p0x,p0y},
        {p2x,p2y},
        {p1x-p0x, p1y-p0y},
        {p3x-p2x, p3y-p2y}
        };
    double invHerMat[4][4] = {
       { 2, -2, 1, 1 },
       { -3, 3, -2, -1 },
       { 0, 0, 1, 0 },
       { 1, 0, 0, 0 },
    };

    double ansMat[4][2] = {};

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 2; j++){
                for(int k = 0; k < 4; ++k)
            {
                ansMat[i][j] += invHerMat[i][k] * geoMat[k][j];
            }
        }
    }


    int counter=0;
    struct point2d pointsOn[30];


    /*

    //Curve rendering: BRUTE FORCE METHOD
    double t=0;
    while(t<=1){
        double xT=0,yT=0;
        for(int i=0; i<4; i++){
            if(i==3){
                xT=xT+ansMat[i][0];
                yT=yT+ansMat[i][1];
            }
            else{
                xT=(xT + ansMat[i][0])*t;
                yT=(yT + ansMat[i][1])*t;
            }
        }
        pointsOn[counter].x = (double)xT;
        pointsOn[counter].y = (double)yT;
        counter++;
        xT=0;
        yT=0;
        t=t+0.1;
    }
    */
     //forward differencing method for rendering curve points
     int t=20;
     double space=0.05;
     double fx,fy,f1x,f2x,f3x,f1y,f2y,f3y;
     fx=ansMat[3][0];
     fy=ansMat[3][1];

     f1x= ansMat[0][0]*pow(space,3)+ ansMat[1][0]*pow(space,2)+ansMat[2][0]*space;
     f2x=6*ansMat[0][0]*pow(space,3)+ 2*ansMat[1][0]*pow(space,2);
     f3x=6*ansMat[0][0]*pow(space,3);

     f1y= ansMat[0][1]*pow(space,3)+ ansMat[1][1]*pow(space,2)+ansMat[2][1]*space;
     f2y=6*ansMat[0][1]*pow(space,3)+ 2*ansMat[1][1]*pow(space,2);
     f3y=6*ansMat[0][1]*pow(space,3);

     pointsOn[counter].x = (double)fx;
     pointsOn[counter].y = (double)fy;
     counter++;

     for(int i=0; i<t; i++){
        fx+=f1x;
        f1x+=f2x;
        f2x+=f3x;

        fy+=f1y;
        f1y+=f2y;
        f2y+=f3y;
        pointsOn[counter].x = (double)fx;
        pointsOn[counter].y = (double)fy;
        counter++;

     }
    for(int i=0; i< counter-1 ; i++){
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);{
			glVertex3f( pointsOn[i].x, pointsOn[i].y, 0);
			glVertex3f( pointsOn[i+1].x, pointsOn[i+1].y, 0);
		}glEnd();
    }

}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

        case 'g':
            drawaxes=1-drawaxes;
			break;
        case 'u':
            upStateOne=1;
            break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			break;
		case GLUT_KEY_UP:		// up arrow key
			break;

		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_LEFT:
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP

                if(initialState==1){
                        std::cout << x << " " << y << std::endl;

                cp[cpidx].x = (double)x;
                cp[cpidx].y = (double)(600 - y);
                cpidx++;
                }

                if(upStateOne == 1){
                    double newx,newy,ans;
                    int counter;
                    newx=(double)x;
                    newy=(double)(600 - y);
                    counter=0,ans=10000;

                    for(int i=0; i<cpidx ; i++){
                        double length;
                        length= sqrt(pow(abs(cp[i].x-newx),2)+pow(abs(cp[i].y-newy),2));

                        if(length < ans){
                            ans = length;
                            counter = i;
                        }
                    }

                    upStateOneEffect=1;
                    changedCounter=counter;
                    changedX=newx;
                    changedY=newy;
                    upStateOne=0;
                    upStateTwo=1;
                }
                else if(upStateTwo == 1){
                    upStateOneEffect=0;
                    cp[changedCounter].x=changedX;
                    cp[changedCounter].y=changedY;
                    upStateTwo=0;
                }
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			initialState=0;
			drawCurveState=1;
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
	//gluLookAt(150*cos(cameraAngle), 150*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,0,	0,0,-1,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects


	int i;

    for (i = 0; i < cpidx; i++)
    {
        glPushMatrix();
        {   if(i%2 !=0){
              drawAxes(cp[i-1].x, cp[i-1].y, cp[i].x, cp[i].y);
              if(drawCurveState ==1){
               if(i+1>=cpidx){
                drawHermitCurve(cp[i-1].x, cp[i-1].y, cp[i].x, cp[i].y,cp[0].x,cp[0].y,cp[1].x,cp[1].y);
               }
               else{
                drawHermitCurve(cp[i-1].x, cp[i-1].y, cp[i].x, cp[i].y,cp[i+1].x,cp[i+1].y,cp[i+2].x,cp[i+2].y);
               }

              }
            }
            if(drawaxes==1){
            glColor3f(1, 1, 0);
            glTranslatef(cp[i].x, cp[i].y, 0);
            drawSquare();
            }

        }
        glPopMatrix();

    }

    if(upStateOneEffect==1){
            glPushMatrix();
            {
            glColor3f(1, 1, 0);
            glTranslatef(cp[changedCounter].x, cp[changedCounter].y, 0);
            drawBigSquare();

            }
            glPopMatrix();

    }

    //cout<<"Hello"<<endl;

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){


	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	cpidx = 0;

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
	gluOrtho2D(0, 800, 0, 600);
	//gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance



	drawaxes=1;
	initialState=1;
	drawCurveState=0;
	upStateOne=0;
	upStateTwo=0;
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(800, 600);
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
