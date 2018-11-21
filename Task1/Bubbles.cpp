#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;


double px,py;
double vx,vy;
double angle1;


double qx,qy;
double ux,uy;
double angle2;


double rotateF;
struct point
{
	double x,y,z;
};


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


void drawPointer(int rad)
{

		glColor3f(1.0, 0, 0);
		glBegin(GL_LINES);{
			glVertex3f( 0,0,0);
			glVertex3f( rad,0,0);

			}glEnd();

		glBegin(GL_TRIANGLES);
        {
            glVertex3f(rad,0,0);
			glVertex3f(rad-3,2,0);
			glVertex3f(rad-3,-2,0);
        }
        glEnd();
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


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawball(double radius,int segments,int ax,int ay,int num)
{
    int i;
    struct point points[100];
    if(num ==1 )glColor3f(0,0,1);
    else glColor3f(0,1,0);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=ax+radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=ay+radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}









void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			rotateF+=.005;
			double rot, vTx, vTy;
			rot =.5;//rotateF;

			 vTx=vy;
			 vTy=-vx;
			vx= vx* cos(rot) + vTx *sin(rot);
			vy= vy*cos(rot) + vTy * sin(rot);
            //px=px+0.5*vx;
            //py=py+0.5*vy;
            angle1=atan2((vy),(vx));
            angle1=(angle1*180)/pi;

			break;

		case GLUT_KEY_LEFT:
			rotateF+=0.005;
			rot=0.5;//rotateF;

			 vTx=-vy;
			 vTy=vx;
			vx= vx* cos(rot) + vTx *sin(rot);
			vy= vy*cos(rot) + vTy * sin(rot);
			//px=px+0.05*vx;
            //py=py+0.05*vy;
            angle1=atan2((vy),(vx));
            angle1=(angle1*180)/pi;

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
	gluLookAt(0,0,150,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    drawCircle(80,80);

    drawball(10,30,px,py,1);
    glPushMatrix();
    {
    glTranslatef(px,0,0);
    glTranslatef(0,py,0);
    glRotatef(angle1,0,0,1);
    drawPointer(10);
    }
    glPopMatrix();


    drawball(10,30,qx,qy,2);

    glPushMatrix();
    {
    glTranslatef(qx,0,0);
    glTranslatef(0,qy,0);
    glRotatef(angle2,0,0,1);
    drawPointer(10);
    }
    glPopMatrix();










    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
    double c1c2,sx,sy,dots1,dots2,magn;
    c1c2=sqrt(((qx-px)*(qx-px) )+ ((qy-py)*(qy-py)));
    if(c1c2<=20){
        sx=qx-px;
        sy=qy-py;
        magn=sqrt((sx*sx)+(sy*sy));
        sx= sx / magn;
        sy= sy /magn;
        dots1= sx*vx + sy*vy;
        dots2= sx*ux + sy*uy;
        vx = vx - 2 * dots1 *sx;
        vy = vy - 2 * dots1 *sy;
        ux = ux - 2 * dots2 *sx;
        uy = uy - 2 * dots2 *sy;
        //px=px+0.5*vx;
        //py=py+0.5*vy;
        //qx=qx + 0.5*ux;
        //qy=qy + 0.5*uy;
        //return;
    }



	double ans,nx,ny,magnitude,dot;
	px=px+0.5*vx;
    py=py+0.5*vy;

    ans=sqrt(((px)*(px))+((py)*(py)));
    if(ans >= 70){
        nx=px-0;
        ny=py-0;
        magnitude=ans;
        nx =(nx / magnitude);
        ny =(ny / magnitude);
        dot = vx*nx + vy*ny;
        nx = 2 * dot* nx;
        ny = 2 * dot* ny;
        vx=vx - nx;
        vy=vy - ny;
    }
    else{
	   vx=vx;
	   vy=vy;

    }
    angle1=atan2((vy),(vx));
    angle1=(angle1*180)/pi;

    qx=qx + 0.5*ux;
    qy=qy + 0.5*uy;
    ans = sqrt(((qx)*(qx))+((qy)*(qy)));

    if(ans >= 70){
        nx= qx- 0;
        ny= qy -0;
        magnitude = ans;
        nx =(nx / magnitude);
        ny =(ny / magnitude);
        dot = ux*nx + uy*ny;
        nx = 2 * dot* nx;
        ny = 2 * dot* ny;
        ux = ux - nx;
        uy = uy - ny;
    }
    else{
        ux=ux;
        uy=uy;
    }

    angle2 = atan2(uy,ux);
    angle2 = (angle2*180)/pi;






	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/

	px=20;
	py=25;

    vx=sqrt(3);
    vy=1;


    qx=-20;
    qy=25;
    ux=1;
    uy=1;

    rotateF = 0;
    //double res;
    //res=vy/vx;

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
