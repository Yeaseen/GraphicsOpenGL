#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))






double posx,posy,posz;

double ux,uy,uz;

double rx,ry,rz;

double lx,ly,lz;

double camRotateAngle;

int drawaxes;
double angle;

double pqangle;

bool pqboolp;
bool pqboolq;
double pqangle2;

bool pqbool2q,pqbool2p;


double pipangle;

bool pipp,pipq;

double rotation;



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
			glVertex3f( 500,0,0);
			glVertex3f(-500,0,0);

			glVertex3f(0,-500,0);
			glVertex3f(0, 500,0);

			glVertex3f(0,0, 500);
			glVertex3f(0,0,-500);
		}glEnd();
	}
}

/*
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
*/
void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,600);
		glVertex3f( a,-a,600);
		glVertex3f(-a,-a,600);
		glVertex3f(-a, a,600);
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

void drawCylinder(double radius,int slices,int stacks, int x)
{
    struct point points[100][100];
	int i,j;
	double h=x-5,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h+=radius / (stacks/2);//*sin(((double)i/(double)stacks)*(pi/2));
		r=radius; //*cos(((double)i/(double)stacks)*(pi/2));
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
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2 == 0) glColor3f(1,1,1);
            else glColor3f(0,0,0);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);



                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

}



void drawSphere2(double radius,int slices,int stacks, int p)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=-p +radius*sin(((double)i/(double)stacks)*(pi/2));
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
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2 == 0) glColor3f(1,1,1);
            else glColor3f(0,0,0);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				//glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                //glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);


                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}






void drawSphere(double radius,int slices,int stacks,int x)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=-x+radius*sin(((double)i/(double)stacks)*(pi/2));
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
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2 == 0) glColor3f(1,1,1);
            else glColor3f(0,0,0);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
            glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);


                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSphere3(double radius,int slices,int stacks,int x)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=-x+radius*sin(((double)i/(double)stacks)*(pi/2));
		r=2* radius-radius*cos(((double)i/(double)stacks)*(pi/2));
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
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2 == 0) glColor3f(1,1,1);
            else glColor3f(0,0,0);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
            glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);


                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSS()
{
    //glColor3f(1,0,0);
    //glRotated(90,0,0,0);



     glColor3f(1,1,1);
     //glTranslatef(100,0,0);
     //glTranslatef(0,-100,0);
     //glTranslatef(0,0,300);
     drawSquare(100);



    glPopMatrix();
    //glTranslated(xangle,0,zangle);

    glRotated(pqangle,1,0,0);
    glRotated(pqangle2,0,1,0);
    drawSphere2(30,24,20,30);
    drawCylinder(30,24,20,30);
    drawSphere(30,24,20,-85);



    glPushMatrix();
    {
    glTranslatef(0,0,+130);
    glRotatef(pipangle,0,1,0);

    glTranslatef(0,0,-130);

    glRotatef(rotation,0,0,1);
    drawSphere2(15,24,20,130);


    glPushMatrix();
    {
    glRotatef(2*angle,0,0,1);
    drawCylinder(15,24,20,130);
    drawCylinder(15,24,20,160);
    drawCylinder(15,24,20,190);
    }
    glPopMatrix();



    drawSphere3(15,24,20,-215);
    }
    glPopMatrix();



    //glTranslatef(0,15,0);



}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
		    double theta,crossx,crossy,crossz,magnitude1;
            theta=(camRotateAngle*pi)/180 ;
            crossx = uy * lz - uz*ly;
            crossy = uz * lx - lz * ux;
            crossz= ux * ly - lx *uy;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            lx = lx * cos(theta) + crossx* sin(theta);
            ly = ly * cos(theta) + crossy* sin(theta);
            lz = lz * cos(theta) + crossz* sin(theta);



            crossx = uy * rz - uz*ry;
            crossy = uz * rx - rz * ux;
            crossz= ux * ry - rx *uy;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            rx = rx * cos(theta) + crossx* sin(theta);
            ry = ry * cos(theta) + crossy* sin(theta);
            rz = rz * cos(theta) + crossz* sin(theta);

			break;
        case '2':
            //double theta,crossx,crossy,crossz,magnitude1;
            theta=(-camRotateAngle*pi)/180 ;
            crossx = uy * lz - uz*ly;
            crossy = uz * lx - lz * ux;
            crossz= ux * ly - lx *uy;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            lx = lx * cos(theta) + crossx* sin(theta);
            ly = ly * cos(theta) + crossy* sin(theta);
            lz = lz * cos(theta) + crossz* sin(theta);



            crossx = uy * rz - uz*ry;
            crossy = uz * rx - rz * ux;
            crossz= ux * ry - rx *uy;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            rx = rx * cos(theta) + crossx* sin(theta);
            ry = ry * cos(theta) + crossy* sin(theta);
            rz = rz * cos(theta) + crossz* sin(theta);
            break;
        case '3':
            theta=(+camRotateAngle*pi)/180 ;
            crossx = ry * uz - rz*uy;
            crossy = rz * ux - rx * uz;
            crossz= rx * uy - ry *ux;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            ux = ux * cos(theta) + crossx* sin(theta);
            uy = uy * cos(theta) + crossy* sin(theta);
            uz = uz * cos(theta) + crossz* sin(theta);



            crossx = ry * lz - rz*ly;
            crossy = rz * lx - rx * lz;
            crossz= rx * ly - ry *lx;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            lx = lx * cos(theta) + crossx* sin(theta);
            ly = ly * cos(theta) + crossy* sin(theta);
            lz = lz * cos(theta) + crossz* sin(theta);
            break;
        case '4':
            theta=(-camRotateAngle*pi)/180 ;
            crossx = ry * uz - rz*uy;
            crossy = rz * ux - rx * uz;
            crossz= rx * uy - ry *ux;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            ux = ux * cos(theta) + crossx* sin(theta);
            uy = uy * cos(theta) + crossy* sin(theta);
            uz = uz * cos(theta) + crossz* sin(theta);



            crossx = ry * lz - rz*ly;
            crossy = rz * lx - rx * lz;
            crossz= rx * ly - ry *lx;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            lx = lx * cos(theta) + crossx* sin(theta);
            ly = ly * cos(theta) + crossy* sin(theta);
            lz = lz * cos(theta) + crossz* sin(theta);
            break;
        case '5':
            theta=(camRotateAngle*pi)/180 ;
            crossx = ly * uz - lz*uy;
            crossy = lz * ux - lx * uz;
            crossz= lx * uy - ly *ux;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            ux = ux * cos(theta) + crossx* sin(theta);
            uy = uy * cos(theta) + crossy* sin(theta);
            uz = uz * cos(theta) + crossz* sin(theta);



            crossx = ly * rz - lz*ry;
            crossy = lz * rx - lx * rz;
            crossz= lx * ry - ly *rx;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            rx = rx * cos(theta) + crossx* sin(theta);
            ry = ry * cos(theta) + crossy* sin(theta);
            rz = rz * cos(theta) + crossz* sin(theta);
            break;

        case '6':
            theta=(-camRotateAngle*pi)/180 ;
            crossx = ly * uz - lz*uy;
            crossy = lz * ux - lx * uz;
            crossz= lx * uy - ly *ux;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            ux = ux * cos(theta) + crossx* sin(theta);
            uy = uy * cos(theta) + crossy* sin(theta);
            uz = uz * cos(theta) + crossz* sin(theta);



            crossx = ly * rz - lz*ry;
            crossy = lz * rx - lx * rz;
            crossz= lx * ry - ly *rx;
            magnitude1 = sqrt((crossx * crossx)+(crossy * crossy)+(crossz * crossz));
            crossx= crossx/magnitude1 ;
            crossy = crossy/magnitude1 ;
            crossz = crossz/magnitude1 ;
            rx = rx * cos(theta) + crossx* sin(theta);
            ry = ry * cos(theta) + crossy* sin(theta);
            rz = rz * cos(theta) + crossz* sin(theta);
            break;
        case 'q':
            if(pqangle<=45 && pqboolp == true){
               pqangle+=1;
               pqboolq=true;
               angle += 1;
            }
            else {
            	//pqangle=0;
            	pqboolp=false;
            	pqboolq=true;
            }
            break;
        case 'w':
            if(pqangle >=-45 && pqboolq ==true){
            	pqangle-=1;
            	pqboolp = true;
            	angle -= 1;
            }
            else{
               pqboolq = false;
               pqboolp = true;
            }
            break;
        case 'e':
            if(pqangle2<=45 && pqbool2p == true){
               pqangle2+=1;
               pqbool2q=true;
               angle += 1;
            }
            else {

            	pqbool2p=false;
            	pqbool2q=true;
            }
            break;
        case 'r':
            if(pqangle2 >=-45 && pqbool2q ==true){
            	pqangle2-=1;
            	pqbool2p = true;
            	angle -= 1;
            }
            else{
               pqbool2q = false;
               pqbool2p = true;
            }
            break;

        case 'a':
            if(pipangle<=45 && pipp == true){
               pipangle+=1;
               angle+=1;
               pipq = true;
            }
            else{
               pipp=false;
               pipq=true;
            }
            break;
        case 's':
            if(pipangle >=-45 && pipq == true){
            pipangle-=1;
            angle-=1;
            pipp = true;
            }
            else{
                pipq=false;
                pipp=true;
            }
            break;

        case 'd':
            rotation+=1;
            break;
        case 'f':
            rotation-=1;
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            posx-=2*lx;
            posy-=2*ly;
            posz-=2*lz;
			break;
		case GLUT_KEY_UP:		// up arrow key
            posx+=2*lx;
            posy+=2*ly;
            posz+=2*lz;
			break;

		case GLUT_KEY_RIGHT:
            posx+=2*rx;
            posy+=2*ry;
            posz+=2*rz;
			break;
		case GLUT_KEY_LEFT:
            posx-=2*rx;
            posy-=2*ry;
            posz-=2*rz;
			break;

		case GLUT_KEY_PAGE_UP:
		    posx+=2*ux;
            posy+=2*uy;
            posz+=2*uz;
			break;
		case GLUT_KEY_PAGE_DOWN:
            posx-=2*ux;
            posy-=2*uy;
            posz-=2*uz;
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

	gluLookAt(posx,posy,posz, posx+lx,posy+ly,posz+lz,	ux,uy,uz);



	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

    drawAxes();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawCylinder(30,24,20);

      //drawSphere(30,24,20);


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/

    posx=+100;
    posy=-100;
    posz=-300;

    ux=0.301511;
    uy=0;
    uz=0;

    rx= 0;
    ry= 0.272751;
    rz= -0.09092;

    lx=-0.301511;
    ly= 0.301511;
    lz= 0.9045;

    drawaxes=1;
    camRotateAngle=0.5;
    angle=0;
    pqangle=0;
    pqboolp=true;
    pqboolq=true;
    pqangle2  =0;
    pqbool2q=true;
    pqbool2p=true;





    pipangle=0;

    pipp = true;
    pipq = true;


    rotation=0;

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
