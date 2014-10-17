
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
#include <queue>

using namespace std;

double offset_x = 200;
double offset_y = 200;
int minX, minY, maxX, maxY;
float k = 1;
float tx = 0.0, ty=0.0;

struct command {
	string name;
	LongInt arg1;
	LongInt arg2;
	command() {
		name = "";
		arg1 = LongInt::LongInt(0);
		arg2 = LongInt::LongInt(0);
	}
};

queue<command> cmdbuffer;
Trist triangles;
vector<int> pointBuffer;
int dy_secs = 0;
int dy_current = 0;
bool jumpnext = false;

bool intriangulate = false;
int atPoint = 0;
TriangulateState *triState;

bool DEBUG = true;


void drawAPoint(double x, double y, bool isNew = false)
{
	glPointSize(5);
	glBegin(GL_POINTS);
	if (isNew)
		glColor3f(0, 1, 0);
	else
		glColor3f(0.5, 0.5, 0.5);
	glVertex2d(x, y);
	glEnd();
	glPointSize(1);
}

void drawAPoint(double x, double y, float r, float g, float b)
{
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(r, g, b);
	glVertex2d(x, y);
	glEnd();
	glPointSize(1);
}

void drawALine(double x1, double y1, double x2, double y2, bool isNew = false)
{
	glPointSize(1);
	glBegin(GL_LINE_LOOP);
	if (isNew)
		glColor3f(0.7, 0.7, 0);
	else
		glColor3f(0, 0, 1);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
	glPointSize(1);
}

void drawATriangle(double x1, double y1, double x2, double y2, double x3, double y3, bool isNew = false)
{
	glBegin(GL_POLYGON);
	if (isNew)
		glColor3f(0.75, 0, 0);
	else
		glColor3f(0, 0.5, 0);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();
}

int currenttime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	int current = (((st.wHour * 60 + st.wMinute) * 60) + st.wSecond) * 1000 + st.wMilliseconds;
	return current;
}

void runcmd(command cmd)
{
	if (DEBUG)
		cout << cmd.name << " " << cmd.arg1.printOut() << " " << cmd.arg2.printOut() << endl;

	if (!cmd.name.compare("CD")){
		if (pointBuffer.size() > 0){
			// TIMER COUNTER INITIALISE FOR THIS "CD" STEP
			intriangulate = true;
			atPoint = 1;
			triState = triangles.triangulateByPoint(pointBuffer.at(0));
		}
	}
	else if (!cmd.name.compare("IP")) {
		if (cmd.arg1 >= minX && cmd.arg1 <= maxX && cmd.arg2 >= minY && cmd.arg2 <= maxY){
			pointBuffer.push_back(triangles.addPoint(cmd.arg1, cmd.arg2));
		}
		else if (DEBUG) {
			cout << "Point does not fit in window : " << cmd.arg1.printOut() << "," << cmd.arg2.printOut() << std::endl;
		}
	}
}

void updatescene(void)
{
	int cur = currenttime();
	if (dy_secs == 0 || jumpnext || ((cur - dy_current) >= dy_secs * 1000))
	{
		jumpnext = false;
		if (intriangulate){
			// TIMER START FOR THIS STEP
			if (!triState->isDone()){
				triangles.triangulateByPointStep(triState);
			}
			else{
				if (atPoint < pointBuffer.size()){
					delete triState;
					triState = triangles.triangulateByPoint(pointBuffer.at(atPoint));
				}
				else{
					triangles.hideBigTriangle(triState);
					triangles.clearActive();
					delete triState;
					intriangulate = false;
					pointBuffer.clear();
					// "CD" STEP DONE! ADD THIS TIMER TO GET FINAL TIME FOR TRIANGULATION
				}
				atPoint++;
			}
			// TIMER STOP FOR THIS STEP, ADD TO TIMER COUNTER
			glutPostRedisplay();
		}
		else if (cmdbuffer.size() > 0){
			command cmd = cmdbuffer.front();
			cmdbuffer.pop();
			runcmd(cmd);
			string last_cmd = cmd.name;

			// run multiple similiar commands together
			while (cmdbuffer.size() != 0){
				cmd = cmdbuffer.front();
				if (!cmd.name.compare(last_cmd)){
					cmdbuffer.pop();
					runcmd(cmd);
				}
				else
					break;
			}
			glutPostRedisplay();
		}
		dy_current = cur;
	}
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
	bool isNew = true;

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

			isNew = triangles.isTriangleLastAdded(it->getIdx());
			drawATriangle(px1.doubleValue(), py1.doubleValue(),
						  px2.doubleValue(), py2.doubleValue(),
						  px3.doubleValue(), py3.doubleValue(), isNew);
			drawALine(px1.doubleValue(), py1.doubleValue(),
					  px2.doubleValue(), py2.doubleValue(), triangles.isActiveEdge(p1Idx, p2Idx));
			drawALine(px2.doubleValue(), py2.doubleValue(),
					  px3.doubleValue(), py3.doubleValue(), triangles.isActiveEdge(p2Idx, p3Idx));
			drawALine(px3.doubleValue(), py3.doubleValue(),
					  px1.doubleValue(), py1.doubleValue(), triangles.isActiveEdge(p3Idx, p1Idx));
		}
	}

	vector<MyPoint> points = triangles.getPoints();
	int i = 0;
	for (vector<MyPoint>::iterator it = points.begin(); it != points.end(); ++it) {
		if(it->visible){
			isNew = triangles.isActivePoint(i) || triangles.isPointLastAdded(i);
			if (isNew)
				drawAPoint(it->x.doubleValue(), it->y.doubleValue(), isNew);
			else if (triangles.isPointOnTri(i))
				drawAPoint(it->x.doubleValue(), it->y.doubleValue(), 0, 0, 0);
			else
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
	string cmd;// the command of each line
	string numberStr; // for single LongInt operation
	string outputAns = "Answer of your computation"; // the answer you computed
	ifstream inputFile("input.txt",ios::in);

	if(inputFile.fail()){
		cerr << "Error: Cannot read input file \"" << "input.txt" << "\"";
		return;
	}

	time_t curtime = time(NULL);
	while(inputFile.good()){
		getline(inputFile,line);
		if(line.empty()) {
			cmd = "";
			continue; 
		}// in case the line has nothing in it

		stringstream linestream(line);

		linestream >> line_noStr;
		linestream >> cmd;         // get the command

		if(!cmd.compare("IP")){
			linestream >> numberStr;
			LongInt p1 = LongInt::LongInt(numberStr.c_str());
			linestream >> numberStr;
			LongInt p2 = LongInt::LongInt(numberStr.c_str());
			command newcmd;
			newcmd.name = "IP";
			newcmd.arg1 = p1;
			newcmd.arg2 = p2;
			cmdbuffer.push(newcmd);
		}
		else if(!cmd.compare("DY")){
			linestream >> numberStr;
			dy_secs = atof(numberStr.c_str());
			if(dy_secs < 0)
				dy_secs = 0;
		}
		else if(!cmd.compare("CD")){
			command newcmd;
			newcmd.name = "CD";
			cmdbuffer.push(newcmd);
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
}

void specialkeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (dy_secs > 0 && !jumpnext) {
			jumpnext = true;
			glutPostRedisplay();
		}
		break;

	default:
		break;
	}
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
		if (DEBUG) {
			std::cout << "clicked" << std::endl;
			std::cout << x << ", " << y << std::endl;
		}
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
	cout << "Right arrow to jump to next step" << endl;
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
	glutIdleFunc(updatescene);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialUpFunc(specialkeys);
	glutMainLoop();

	return 0;
}
