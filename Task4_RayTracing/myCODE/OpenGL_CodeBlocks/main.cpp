#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <vector>
#include <windows.h>
#include <glut.h>
#include "bitmap_image.hpp"
using namespace std;

#define eps 0.00001
#define WINDOW_WIDTH 500.0
#define WINDOW_HEIGHT 500.0
#define pi (2*acos(0.0))

#define MAX_VAL 9999999
#define MIN_VAL -9999999



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

class color {
public:
    double r, g, b;
    color(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    color() {
    }

};

class colorcomponents{
public:
    double ambient, diffuse, specular, reflectance;
    colorcomponents(double a, double d, double s, double r){
        this->ambient=a;
        this->diffuse=d;
        this->specular=s;
        this->reflectance=r;
    }
    colorcomponents(){
    }
};

class Primitives {
public:


    Point ref_point;
    color clr;
    colorcomponents coefficients;
    string name;
    int shine;


    virtual void draw() = 0;
    virtual double intersect_t(Point ray_start,Point ray_dir) =0;

    void setColor(color in) {
        this->clr=in;
    }

    void setShine(int shine) {
        this->shine = shine;
    }

     void setCoEfficients(colorcomponents inc) {
        this->coefficients = inc;
    }
};

class CheckerBoard:public Primitives{
public:

    double width, length;

    CheckerBoard(string name, double flrWidth, double tileWidth){
        this->name = name;
        width=flrWidth;
        length=tileWidth;
        ref_point= Point(-flrWidth/2, -flrWidth/2, 0);

    }

    void draw() {

        //int numOfTiles = 2.0*width/length;
        int numOfTiles = abs(ref_point.x*2/length);
        for (int i=0; i<numOfTiles; i++) {
            for (int j=0; j<numOfTiles; j++) {

                if ((i+j)%2 == 1) {
                    glColor3f(0, 0, 0);
                } else {
                    glColor3f(1, 1, 1);
                }

                glBegin(GL_QUADS);
                {
                    glVertex3f(ref_point.x+length*i, ref_point.y+length*j,0);
                    glVertex3f(ref_point.x+length*(i+1), ref_point.y+length*j, 0);
                    glVertex3f(ref_point.x+length*(i+1), ref_point.y+length*(j+1), 0);
                    glVertex3f(ref_point.x+length*i, ref_point.y+length*(j+1), 0);
                }
                glEnd();
            }
        }
    }

    Point getNormal(){
        return Point(0,0,1);
    }


    double intersect_t(Point ray_start,Point ray_dir){
      ray_dir.normalize();
      Point normal = this->getNormal();
      double t = ((-1.0) * ray_start.dot(normal))/ normal.dot(ray_dir);

      //double t= solveIntersectionT(ray_start,ray_dir);

      Point intsec=ray_start+ ray_dir*t;
      intsec.z=0;

      double xmin,xmax,ymin,ymax;
      xmin=ref_point.x;
      xmax=xmin * (-1);

      ymin=ref_point.y;
      ymax=ymin * (-1);

      if (xmin > intsec.x || intsec.x > xmax || ymin > intsec.y || intsec.y > ymax ) {
            return -1;
      }

      int xc=(intsec.x - this->ref_point.x)/ length;
      int yc=(intsec.y - this->ref_point.y)/ length;

      if((xc+yc)%2==1){
        this->setColor(color(0,0,0));
      }
      else {
        this->setColor(color(1,1,1));
      }
      return t;
    }



};

class Pyramid:public Primitives{
public:
    double width,height;
    Point p1,p2,p3,p4,p5;

    Pyramid(string name,Point p,double w, double h){
        this->name=name;
        ref_point=p;
        width=w;
        height=h;
        this->p1=Point(ref_point.x,ref_point.y,ref_point.z+height);

        this->p2=Point(ref_point.x-width/2,ref_point.y-width/2,ref_point.z);

        this->p3=Point(ref_point.x+width/2,ref_point.y-width/2,ref_point.z);


        this->p4=Point(ref_point.x+width/2,ref_point.y+width/2,ref_point.z);

        this->p5=Point(ref_point.x-width/2,ref_point.y+width/2,ref_point.z);



    }

    void draw(){

    glColor3f(clr.r,clr.g,clr.b);
        glPushMatrix();
    {
    //glTranslatef(ref_point.x,ref_point.y,ref_point.z);
    glBegin(GL_QUADS);{
        glVertex3f(ref_point.x-width/2,ref_point.y-width/2,ref_point.z);
        glVertex3f(ref_point.x+width/2,ref_point.y-width/2,ref_point.z);
        glVertex3f(ref_point.x+width/2,ref_point.y+width/2,ref_point.z);
        glVertex3f(ref_point.x-width/2,ref_point.y+width/2,ref_point.z);

        //p2,p3,p4

        //p3,p4,p5
    }glEnd();

    glBegin(GL_TRIANGLES);{
        //p1,p2,p3
        glVertex3f(ref_point.x,ref_point.y,ref_point.z+height);
        glVertex3f(ref_point.x-width/2,ref_point.y-width/2,ref_point.z);
        glVertex3f(ref_point.x+width/2,ref_point.y-width/2,ref_point.z);

        //p1,p3,p4
        glVertex3f(ref_point.x,ref_point.y,ref_point.z+height);
        glVertex3f(ref_point.x+width/2,ref_point.y-width/2,ref_point.z);
        glVertex3f(ref_point.x+width/2,ref_point.y+width/2,ref_point.z);

        //p1,p5,p4
        glVertex3f(ref_point.x,ref_point.y,ref_point.z+height);
        glVertex3f(ref_point.x-width/2,ref_point.y+width/2,ref_point.z);
        glVertex3f(ref_point.x+width/2,ref_point.y+width/2,ref_point.z);

        //p1,p2,p5
        glVertex3f(ref_point.x,ref_point.y,ref_point.z+height);
        glVertex3f(ref_point.x-width/2,ref_point.y+width/2,ref_point.z);
        glVertex3f(ref_point.x-width/2,ref_point.y-width/2,ref_point.z);
    }glEnd();
    }
    glPopMatrix();


    }


    double getDET(Point e1, Point e2, Point e3){
        return e1.x*(e2.y*e3.z - e2.z*e3.y) - e1.y*(e2.x*e3.z - e2.z*e3.x) + e1.z*(e2.x*e3.y - e2.y*e3.x);
    }

    double getsolveOne(Point ray_start, Point ray_dir, Point pp1, Point pp2, Point pp3){

        Point edge1 = pp1 - pp2;
        Point edge2 = pp1 - pp3;
        double det = this->getDET(edge1, edge2, ray_dir);
        if(det > -eps && det < eps) return -1;

        Point temp = pp1 - ray_start;
        double beta = this->getDET(temp, edge2, ray_dir)/det;
        double gamma = this->getDET(edge1, temp, ray_dir)/det;
        if(beta < 0 || gamma < 0 || beta+gamma > 1) return -1;
        double t = this->getDET(edge1, edge2, temp)/det;
        if(t<=0) return -1;
        else return t;

    }

    double intersect_t(Point ray_start,Point ray_dir){
        ray_dir.normalize();
        vector<double> solves;

        double t1=getsolveOne(ray_start,ray_dir,p1,p2,p3);
        if(t1>0) solves.push_back(t1);
        double t2=getsolveOne(ray_start,ray_dir,p1,p3,p4);
        if(t2>0) solves.push_back(t2);
        double t3=getsolveOne(ray_start,ray_dir,p1,p5,p4);
        if(t3>0) solves.push_back(t3);
        double t4=getsolveOne(ray_start,ray_dir,p1,p2,p5);
        if(t4>0) solves.push_back(t4);
        double t5=getsolveOne(ray_start,ray_dir,p2,p3,p4);
        if(t5>0) solves.push_back(t5);
        double t6=getsolveOne(ray_start,ray_dir,p3,p4,p5);
        if(t6>0) solves.push_back(t6);

        if(solves.size()==0){
            return -1;
        }
        else{
            sort(solves.begin(), solves.end());
            return solves[0];
        }
    }
};

void drawSphere(double radius,int slices,int stacks)
{
	Point points[100][100];
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


class Sphere:public Primitives{
public:
    double radius;
    Sphere(string name, Point c, double r){
        this->name=name;
        ref_point=c;
        radius=r;
    }

    void draw(){


    glColor3f(clr.r,clr.g,clr.b);
        glPushMatrix();
    {
    glTranslatef(ref_point.x,ref_point.y,ref_point.z);

    drawSphere(radius,24,20);
    }
    glPopMatrix();



    }

    double intersect_t(Point ray_start,Point ray_dir){
        ray_dir=ray_dir.normalize();
        Point R0 = ray_start - ref_point;
        double a,b,c,d;
        a=1;
        b=2* R0.dot(ray_dir);
        c= R0.dot(R0) - radius*radius;

        d = b*b - 4*c;

        if(d < 0) return -1;
        double t1 = (-b + sqrt(d))/(2.0);
        double t2 = (-b - sqrt(d))/(2.0);
        double t = t1<t2?t1:t2;
        return t;
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
double Near,Far,fovY,aspect_ratio;

int reccLevel;
int imageWidth;
int imageHeight;

vector<Primitives*>  objects;



void readData(){

    ifstream description;
    description.open("description.txt");
    description>> Near >> Far >> fovY >> aspect_ratio >> reccLevel >> imageWidth;
    imageHeight=imageWidth;

    Primitives* instance;
    double boardtile;
    description>> boardtile;
    colorcomponents board;
    description>> board.ambient >> board.diffuse>> board.reflectance;
    board.specular=0.0;
    instance=new CheckerBoard("Board",1000, boardtile);
    instance->setCoEfficients(board);
    instance->setShine(1);
    objects.push_back(instance);

    int numObj;
    description>> numObj;

    color it;
    colorcomponents co;
    int sh;
    for(int i=0; i<numObj; i++){
        string type;
        description>>type;
        if(type == "pyramid"){
            Point lowest;
            description>> lowest.x>> lowest.y>>lowest.z;
            double width, height;
            description>> width >> height;

            description>> it.r >> it.g >>it.b;

            description>>co.ambient>>co.diffuse>>co.specular>>co.reflectance;

            description>>sh;

            instance= new Pyramid(type,lowest,width,height);
            instance->setColor(it);
            instance->setCoEfficients(co);
            instance->setShine(sh);

        }
        else if(type== "sphere"){

            Point center;
            description>>center.x>>center.y>>center.z;
            double rad;
            description>>rad;
            description>> it.r >> it.g >>it.b;

            description>>co.ambient>>co.diffuse>>co.specular>>co.reflectance;

            description>>sh;

            instance=new Sphere(type,center,rad);
            instance->setColor(it);
            instance->setCoEfficients(co);
            instance->setShine(sh);


        }

        objects.push_back(instance);



    }


    description.close();
}


void click(){
    cout<<objects.size()<<endl;
    Point midPoint = pos + l * Near;
    double fov_y=fovY*(pi/180);
    double fov_x=fov_y * aspect_ratio;
    double h_pix = Near * tan(fov_y/2);
    double w_pix = Near * tan(fov_x/2);

    Point** pointBuffer = new Point*[imageWidth];
    int init= imageHeight / 2;

    for(int i = -init; i < (imageHeight + 1) / 2; i++){
        pointBuffer[i + init] = new Point [imageHeight];
        for(int j = -init; j < (imageHeight + 1) / 2; j++){
        pointBuffer[i + init][j + init] = midPoint + r * (i / (imageHeight / 2.0)) * w_pix - u * (j / (imageHeight / 2.0)) * h_pix;
        }
    }

    color** imageBuffer = new color*[imageWidth];
    for(int i = 0; i < imageHeight; i++){

         imageBuffer[i] = new color [imageHeight];
        for(int j = 0; j < imageHeight; j++){
            double t = MAX_VAL;
            int selectedObject = -1;
            for(int k = 0; k < objects.size(); k++){
                double temp = objects[k]->intersect_t(pointBuffer[i][j], pointBuffer[i][j] - pos);

                if(t > temp && temp > 0){
                    t = temp;
                    selectedObject = k;
                }
            }
            if(selectedObject >= 0){
                //Vector v = pointBuffer[i][j] - pos;
                //v.normalize();
                //v = pointBuffer[i][j] + v * t;
                imageBuffer[i][j] = objects[selectedObject]->clr;//getColor(v, pointBuffer[i][j], selectedObject, 3);
            }

            else imageBuffer[i][j] = color(0, 0, 0);

        }
    }

    bitmap_image image(imageHeight, imageHeight);
    for (int x = 0; x < imageHeight; x++) {
        for (int y = 0; y < imageHeight; y++) {
            imageBuffer[x][y].r = (imageBuffer[x][y].r > 1) ? 1 : imageBuffer[x][y].r;
            imageBuffer[x][y].g = (imageBuffer[x][y].g > 1) ? 1 : imageBuffer[x][y].g;
            imageBuffer[x][y].b = (imageBuffer[x][y].b > 1) ? 1 : imageBuffer[x][y].b;
            image.set_pixel(x, y, imageBuffer[x][y].r * 255, imageBuffer[x][y].g * 255, imageBuffer[x][y].b * 255);
        }
    }
    image.save_image("out.bmp");

    delete[] pointBuffer;
    delete[] imageBuffer;

}




void press1(){
            temp.x = l.x*cos(pi/90) - r.x*sin(pi/90);
            temp.y = l.y*cos(pi/90) - r.y*sin(pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;

            r.x = r.x*cos(pi/90) + l.x*sin(pi/90);
            r.y = r.y*cos(pi/90) + l.y*sin(pi/90);
            tempVal = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);
            r.x /= tempVal;
            r.y /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
}

void press2(){
            temp.x = l.x*cos(-pi/90) - r.x*sin(-pi/90);
            temp.y = l.y*cos(-pi/90) - r.y*sin(-pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;

            r.x = r.x*cos(-pi/90) + l.x*sin(-pi/90);
            r.y = r.y*cos(-pi/90) + l.y*sin(-pi/90);
            tempVal = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);
            r.x /= tempVal;
            r.y /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
}

void press3(){
            temp.x = l.x*cos(pi/90) + u.x*sin(pi/90);
            temp.y = l.y*cos(pi/90) + u.y*sin(pi/90);
            temp.z = l.z*cos(pi/90) + u.z*sin(pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            u.x = u.x*cos(pi/90) - l.x*sin(pi/90);
            u.y = u.y*cos(pi/90) - l.y*sin(pi/90);
            u.z = u.z*cos(pi/90) - l.z*sin(pi/90);
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
            l.z = temp.z;

}

void press4(){
            temp.x = l.x*cos(-pi/90) + u.x*sin(-pi/90);
            temp.y = l.y*cos(-pi/90) + u.y*sin(-pi/90);
            temp.z = l.z*cos(-pi/90) + u.z*sin(-pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            u.x = u.x*cos(-pi/90) - l.x*sin(-pi/90);
            u.y = u.y*cos(-pi/90) - l.y*sin(-pi/90);
            u.z = u.z*cos(-pi/90) - l.z*sin(-pi/90);
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
            l.z = temp.z;
}

void press5(){
            temp.x = r.x*cos(pi/90) - u.x*sin(pi/90);
            temp.y = r.y*cos(pi/90) - u.y*sin(pi/90);
            temp.z = r.z*cos(pi/90) - u.z*sin(pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            u.x = u.x*cos(pi/90) + r.x*sin(pi/90);
            u.y = u.y*cos(pi/90) + r.y*sin(pi/90);
            u.z = u.z*cos(pi/90) + r.z*sin(pi/90);
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            r.x = temp.x;
            r.y = temp.y;
            r.z = temp.z;
}

void press6(){

            temp.x = r.x*cos(-pi/90) - u.x*sin(-pi/90);
            temp.y = r.y*cos(-pi/90) - u.y*sin(-pi/90);
            temp.z = r.z*cos(-pi/90) - u.z*sin(-pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;


            u.x = u.x*cos(-pi/90) + r.x*sin(-pi/90);
            u.y = u.y*cos(-pi/90) + r.y*sin(-pi/90);
            u.z = u.z*cos(-pi/90) + r.z*sin(-pi/90);
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            r.x = temp.x;
            r.y = temp.y;
            r.z = temp.z;
}



void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 0.0, 0.0);
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
        case '0':
            cout<<"rendering image"<<endl;
            click();
            cout<<"rendering done"<<endl;
            break;
		case '1':
            press2();
			break;
        case '2':
            press1();
			break;
        case '3':
            press3();
			break;
        case '4':
            press4();
			break;
        case '5':
            press5();
			break;
        case '6':
            press6();
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
	for (int i=0; i < objects.size(); i++) {
        objects[i]->draw();
    }






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
	gluPerspective(fovY,	aspect_ratio,	Near, Far);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
    readData();
	glutInit(&argc,argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
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
