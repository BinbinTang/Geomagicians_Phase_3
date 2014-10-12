
#include "basicsP2\pointSetArray.h"
#include "basicsP2\trist.h"

#include "math.h"
#include <iostream>
#include <fstream>
#include <gl\glut.h>
#include <windows.h>
#include <cstdio>
#include <fstream>
#include <strstream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

double offset_x = 200;
double offset_y = 200;
int minX, minY, maxX, maxY;
float k = 1;
float tx = 0.0, ty=0.0;

Trist triangles;
vector<int> pointBuffer;
int dy_secs = 0;

bool DEBUG = false;

// These three functions are for those who are not familiar with OpenGL, you can change these or even completely ignore them

void drawAPoint(double x,double y)
{
		glPointSize(5);
		glBegin(GL_POINTS);
		glColor3f(0,0,0);
		glVertex2d(x,y);
		glEnd();
		glPointSize(1);
}

void drawALine(double x1,double y1, double x2, double y2)
{
		glPointSize(1);
		glBegin(GL_LINE_LOOP);
		glColor3f(0,0,1);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
		glEnd();
		glPointSize(1);
}

void drawATriangle(double x1,double y1, double x2, double y2, double x3, double y3)
{
		glBegin(GL_POLYGON);
		glColor3f(0,0.5,0);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
		glVertex2d(x3,y3);
		glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);	
	glTranslatef(tx, ty, 0);	

	int nbPoint = triangles.noPt();
	int p1Idx, p2Idx, p3Idx;
	LongInt px1, py1, px2, py2, px3, py3;

	glTranslated(offset_x,offset_y,0);
	vector<TriRecord> record = triangles.getTriangles();
	for(vector<TriRecord>::iterator it = record.begin(); it != record.end(); ++it){
		if(it->getVisibility()){
			int *vert = it->getVertices();
			p1Idx = vert[0];
			p2Idx = vert[1];
			p3Idx = vert[2];
			triangles.getPoint(p1Idx, px1, py1);
			triangles.getPoint(p2Idx, px2, py2);
			triangles.getPoint(p3Idx, px3, py3);

			if (DEBUG) {
				std::cout << "Drawing Tri" << it->getIdx() << std::endl;
				std::cout << p1Idx << "," << p2Idx << "," << p3Idx << std::endl;
				std::cout << px1.printOut() << "," << py1.printOut() << std::endl;
				std::cout << px2.printOut() << "," << py2.printOut() << std::endl;
				std::cout << px3.printOut() << "," << py3.printOut() << std::endl;
				std::cout << std::endl;
			}
		
			drawATriangle(px1.doubleValue(), py1.doubleValue(),
						  px2.doubleValue(), py2.doubleValue(),
						  px3.doubleValue(), py3.doubleValue());
			drawALine(px1.doubleValue(), py1.doubleValue(),
					  px2.doubleValue(), py2.doubleValue());
			drawALine(px2.doubleValue(), py2.doubleValue(),
					  px3.doubleValue(), py3.doubleValue());
			drawALine(px3.doubleValue(), py3.doubleValue(),
					  px1.doubleValue(), py1.doubleValue());
		}
	}

	vector<MyPoint> points = triangles.getPoints();
	int i=0;
	for (vector<MyPoint>::iterator it = points.begin(); it != points.end(); ++it) {
		if(it->visible){
			if (DEBUG) {
				std::cout << "Drawing Point" << i << std::endl;
				std::cout << it->x.printOut() << "," << it->y.printOut() << std::endl;
				std::cout << std::endl;
			}
			drawAPoint(it->x.doubleValue(), it->y.doubleValue());
		}
		i++;
	}

	glPopMatrix();
	glutSwapBuffers();
}

void reshape (int w, int h)
{

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


}

void init(void)
{
	glClearColor (1.0,1.0,1.0, 1.0);
}

void readFile(){

	string line_noStr;
	string line;   // each line of the file
	string command;// the command of each line
	string numberStr; // for single LongInt operation
	string outputAns = "Answer of your computation"; // the answer you computed
	ifstream inputFile("input.txt",ios::in);

	if(inputFile.fail()){
		cerr << "Error: Cannot read input file \"" << "input.txt" << "\"";
		return;
	}

	time_t curtime = time(NULL);
	while(inputFile.good()){
		Sleep(dy_secs * 1000);

		getline(inputFile,line);
		if(line.empty()) {
			command = "";
			continue; 
		}// in case the line has nothing in it

		stringstream linestream(line);

		linestream >> line_noStr;
		linestream >> command;         // get the command

		if(!command.compare("IP")){
			linestream >> numberStr;
			LongInt p1 = LongInt::LongInt(numberStr.c_str());
			linestream >> numberStr;
			LongInt p2 = LongInt::LongInt(numberStr.c_str());

			if (DEBUG){
				std::cout << "Reading point" << std::endl;
				std::cout << p1.printOut() << "," << p2.printOut() << std::endl;
				std::cout << std::endl;
			}
			if(p1 >= minX && p1 <= maxX && p2 >= minY && p2 <= maxY){
				pointBuffer.push_back(triangles.addPoint(p1,p2));
			}else{
				cout << "Point does not fit in window : " << p1.printOut() << "," << p2.printOut() << std::endl;
			}
			glutPostRedisplay();
		}
		else if(!command.compare("DY")){
			linestream >> numberStr;
			dy_secs = atof(numberStr.c_str());
			if(dy_secs < 0)
				dy_secs = 0;
		}
		else if(!command.compare("CD")){
			cout << "CD" << endl;
			for(int i=0; i < pointBuffer.size(); i++){
				triangles.triangulateByPoint(pointBuffer.at(i));
			}
			pointBuffer.clear();
			glutPostRedisplay();
		}
		else{
			cerr << "Exception: Wrong input command" << endl;
		}
		
	}
}

void writeFile()
{
	ofstream outputFile("savefile.txt",ios::out, ios_base::trunc);
	int no_line = 1;
	int p1Idx, p2Idx, p3Idx;
	OrTri tri;
	LongInt px, py;
	vector<MyPoint> points = triangles.getPoints();
	for (vector<MyPoint>::iterator it = points.begin(); it != points.end(); ++it) {
		if(it->visible){
			outputFile << no_line << ": IP " << it->x.printOut().c_str() << " " << it->y.printOut().c_str() << endl;
			no_line++;
		}
	}
	outputFile << no_line << ": CD " << endl;
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'i':
		case 'I':
			k += 0.1;
			glutPostRedisplay();
		break;

		case 'c':
		case 'C':
			glutPostRedisplay();
		break;

		case 'o':
		case 'O':
			if(k>0.1)
				k-=0.1;
			glutPostRedisplay();
		break;
			
		case 'r':
		case 'R':
			readFile();
		break;

		case 'w':
		case 'W':
			writeFile();
		break;

		case 'Q':
		case 'q':
			exit(0);
		break;

		case 's':
		case 'S':
			writeFile();
			exit(0);
		break;

		case 'L':
		case 'l':
			tx-= 1;
			glutPostRedisplay();
		break;

		case 'a':
		case 'A':
			tx+= 1;
			glutPostRedisplay();
		break;

		case 'd':
		case 'D':
			ty-= 1;
			glutPostRedisplay();
		break;

		case 'u':
		case 'U':
			ty+= 1;
			glutPostRedisplay();
		break;

		default:
		break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};
	if((button == MOUSE_RIGHT_BUTTON)&&(state == GLUT_UP))
	{
		x = x + minX;
		y = y + minY;
		std::cout << "clicked" << std::endl;
		std::cout << x << ", " << y << std::endl;
		if(x >= minX && x <= maxX && y >= minY && y <= maxY){ 
			triangles.addPointUpdate(LongInt::LongInt(x), LongInt::LongInt(y));
		}
	}

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	cout<<"CS5237 phase ii"<< endl<< endl;
	cout << "right mouse click: ot operation"<<endl;
	cout <<"i/o: zoom in/out"<<endl;
	cout <<"l/a: move left/right"<<endl;
	cout <<"u/d: move up or down"<<endl;
	cout << "q: quit" <<endl;
	cout << "r: read in control points from \"input.txt\"" <<endl;
	cout << "w: write control points to \"savefile.txt\"" <<endl;
	cout << "s: saving the final triangulation to \"savefile.txt\" and quit the program"<<endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1000, 700);

	minX = -(GLUT_WINDOW_WIDTH + offset_x/2);
	minY = -(GLUT_WINDOW_HEIGHT + offset_y/2);
	maxX = 1000 + minX;
	maxY = 700 + minY;
	
	glutInitWindowPosition(50, 50);
	glutCreateWindow ("CS5237 Phase II");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
