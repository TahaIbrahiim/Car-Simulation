#define STB_IMAGE_IMPLEMENTATION
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <GL/stb_image.h>
#include <math.h>
#include<string.h>

#define ESCAPE 27

unsigned int texture;
int w, h, nrChannels ,d=0; //nerChannels = type of photo RGB or grey or binary  //d = flag for right or left for keyboard animation
unsigned char* data = NULL;

float width = GetSystemMetrics(SM_CXSCREEN);
float height = GetSystemMetrics(SM_CYSCREEN);

float i,theta;
GLint nml=0,day=1;

char proj_name[]="Car Simulator";

GLfloat xt=0.0,yt=0.0,zt=0.0,xw=0.0;   /* x,y,z translation */  // xt = car // xw = road

GLfloat xs=1.0,ys=1.0,zs=1.0;		   /* xs = scale on x Axis */

GLfloat xangle=0.0,yangle=0.0,zangle=0.0,angle=0.0;   /* axis angles  , last angle for wheels */

GLfloat r = 0, g = 0, b = 1, //car colorz
rc = 1, gc = 1, bc = 1; //cube colors
GLint light=1;
int count=1;    //count for lighting 
int view = 0, c = 0;	//view for view scene  , c for car stopping
int flag1=0,aflag=1;       //to switch car driving mode
int flag2=0,wheelflag=0;   //to switch fog effect
GLUquadricObj *t;



/* Simple transformation routine */
GLvoid Transform(GLfloat Width, GLfloat Height)
{
  glViewport(0, 0, Width, Height);              /* Set the viewport */
  glMatrixMode(GL_PROJECTION);                  /* Select the projection matrix */
  glLoadIdentity();				/* Reset The Projection Matrix */
  gluPerspective(45.0,Width/Height,0.1,100.0);  /* Calculate The Aspect Ratio Of The Window */
  glMatrixMode(GL_MODELVIEW);                   /* Switch back to the modelview matrix */
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
GLvoid InitGL(GLfloat Width, GLfloat Height)
{

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glLineWidth(2.0);              /* Add line width,   ditto */
  Transform( Width, Height ); /* Perform the transformation */
  //newly added
	t=gluNewQuadric();
	gluQuadricDrawStyle(t, GLU_FILL);

	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);

// Create light components
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { 1.5f, 1.0f, 4.0f, 1.0f };

// Assign created components to GL_LIGHT0
glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
glLightfv(GL_LIGHT0, GL_POSITION, position);

}

/* The function called when our window is resized  */
GLvoid ReSizeGLScene(GLint Width, GLint Height)
{
  if (Height==0)     Height=1;                   /* Sanity checks */
  if (Width==0)      Width=1;
  Transform( Width, Height );                   /* Perform the transformation */
}

void init()
{
    //glClearColor(0,0,0,0);
	glEnable(GL_TEXTURE_2D);
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,900.0,0.0,600.0,50.0,-50.0);
	glutPostRedisplay(); 		// request redisplay
	
}


/********************* First screen ************************/

void display_string(int x, int y, char *string, int font)
{
	
	glRasterPos2f(x, y);
    int len = (int) strlen(string);
    for (int i = 0; i < len; i++) {
    if(font==1)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
	if(font==2)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
	if(font==3)
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,string[i]);
	if(font==4)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,string[i]);
	}

}

void display1(void)
{

	glClearColor(1.0,0.9,0.8,1.0);
	display_string(180,540,"BFCAI",1);
	display_string(215,500,proj_name,1);
	display_string(390,440,"HELP",2);
	display_string(10,390,"MOUSE",2);
	display_string(10,360,"PRESS RIGHT BUTTON FOR MENU",3);
	display_string(10,320,"KEYBOARD",2);
	display_string(10,280,"X-Y-Z KEYS FOR CORRESPONDING ROTATION",3);
	display_string(10,250,"A-S-Q CAR CUSTOM SIZE SELECTION",3);
	display_string(10,220,"U-F FOR CAMERA VIEW SETTINGS",3);
	display_string(10,190,"USE LEFT ARROW(<-) AND RIGHT ARROW(->) TO MOVE CAR",3);
	display_string(10,160,"ESCAPE TO EXIT",3);
	display_string(250,110,"PRESS SPACE BAR TO ENTER",2);
	glutPostRedisplay();
	glutSwapBuffers();

}


/*****************Collision***********************/
void collision()
{
	if (xt <= -.8 && flag1)
	{
		rc = gc = bc = .09;  // Cube RGB
		c = 1;				// Car Stopping
	}
	else
	{
		rc = gc = bc = 1;
		c = 0;
	}
	glutPostRedisplay();
}

/*****************Texture**************************/
void check(unsigned char* data) {
	if (data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
void load(int imgnum) {
	if (imgnum == 1) {

		data = stbi_load("wall.bmp", &w, &h, &nrChannels, 0);
		check(data);
	}
}


/****************Animation*************************/
void timer(int v) {
	if(!d){
	if(wheelflag==1  && !c ){
	    angle += 3;
		xt -= 0.03;
		glutPostRedisplay();
		glutTimerFunc(1000/60, timer, v);
		collision();
	}
	}
	else
	{
		if(wheelflag==1 ){
	    angle += 3;
		xt += 0.03;
		glutPostRedisplay();
		glutTimerFunc(1000/60, timer, v);
	}
	
	}

	


}

GLvoid DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear The Screen And The Depth Buffer */
	if (view == 0)
	{
		init();
		display1();
	}
	else
	{
		
		if (count == 1)
			InitGL(width, height);
		if (aflag == 1)/* Initialize our window. */
			glClearColor(1, 1, 1, 1);
		else
			glClearColor(0.1, 0.1, 0.1, 0);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(-1.0, 0.0, -3.5);
		glRotatef(xangle, 1.0, 0.0, 0.0);
		glRotatef(yangle, 0.0, 1.0, 0.0);
		glRotatef(zangle, 0.0, 0.0, 1.0);

		//--------------------cube---------------------//
		if (flag1 == 1)
		{
			glPushMatrix();
			glTranslatef(-1, yt + .11, zt + .37);
			glScalef(.15, .15, .15);
			load(1);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			   // Top face (y = 1.0f)
			   // Define vertices in counter-clockwise (CCW) order with normal pointing out
			glColor3f(rc, gc, bc);     // black
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(1.0f, 2.0f, -1.0f);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(-1.0f, 2.0f, -1.0f);
			glTexCoord2d(1.0f,1.0f);
			glVertex3f(-1.0f, 2.0f, 1.0f);
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(1.0f, 2.0f, 1.0f);

			// Bottom +face (y = -1.0f)
			//glColor3f(1.0f, 0.5f, 0.0f);     // Orange
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 1.0f);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(-1.0f, 0.0f, 1.0f);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(1.0f, 0.0f, -1.0f);

			// Front face  (z = 1.0f)
			//glColor3f(1.0f, 0.0f, 0.0f);     // Red
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(1.0f, 2.0f, 1.0f);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(-1.0f, 2.0f, 1.0f);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, 1.0f);
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(1.0f, 0.0f, 1.0f);

			// Back face (z = -1.0f)
			//glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, -1.0f);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(-1.0f, 2.0f, -1.0f);
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(1.0f, 2.0f, -1.0f);

			// Left face (x = -1.0f)
			//glColor3f(0.0f, 0.0f, 1.0f);     // Blue
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(-1.0f, 2.0f, 1.0f);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(-1.0f, 2.0f, -1.0f);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, 1.0f);

			// Right face (x = 1.0f)
			//glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
			glTexCoord2d(0.0f, 0.0f);
			glVertex3f(1.0f, 2.0f, -1.0f);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3f(1.0f, 2.0f, 1.0f);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3f(1.0f, 0.0f, 1.0f);
			glTexCoord2d(0.0f, 1.0f);
			glVertex3f(1.0f, 0.0f, -1.0f);
			glEnd();  // End of drawing color-cube
			glPopMatrix();
		}

		collision();
		glTranslatef(xt, yt, zt);
		glScalef(xs, ys, zs);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

		if (flag2 == 1)
		{
			GLfloat fogcolour[4] = { 1.0,1.0,1.0,1.0 };

			glFogfv(GL_FOG_COLOR, fogcolour);              /* Define the fog colour */
			glFogf(GL_FOG_DENSITY, 0.1);                   /* How dense */
			glFogi(GL_FOG_MODE, GL_EXP);                   /* exponential decay */
			glFogf(GL_FOG_START, 3.0);                   /* Where we start fogging */
			glFogf(GL_FOG_END, 100.0);                       /* end */
			glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
			glEnable(GL_FOG);								/* ENABLE */
		}
		if (flag2 == 0)
		{
			glDisable(GL_FOG);
		}

		if (!aflag) {
			glBegin(GL_POINTS);
			glColor3f(1, 1, 1);
			glPointSize(200.0);
			int ccount = 0;
			float x = 10, y = 10;
			while (ccount < 20)
			{
				glVertex2f(x, y);
				x += 10;
				y += 10;
				if (y > height) y -= 10;
				if (x > width) x -= 10;
				ccount++;
			}
			glEnd();
		}

		glColor3f(1.0, .75, 0.0);
		glPointSize(30.0);
		glBegin(GL_POINTS);
		glVertex3f(0.2, 0.3, 0.3);
		glVertex3f(0.2, 0.3, 0.5);
		glEnd();
		glPointSize(200.0);

		/* top of cube*/
		glBegin(GL_QUADS);                /* OBJECT MODULE*/
		//************************FRONT BODY****************************************
		glColor3f(r, g, b);
		glVertex3f(0.2, 0.4, 0.6);
		glVertex3f(0.6, 0.5, 0.6);
		glVertex3f(0.6, 0.5, 0.2);
		glVertex3f(0.2, 0.4, 0.2);

		/* bottom of cube*/
		glVertex3f(0.2, 0.2, 0.6);
		glVertex3f(0.6, 0.2, 0.6);
		glVertex3f(0.6, 0.2, 0.2);
		glVertex3f(0.2, 0.2, 0.2);

		/* front of cube*/
		glVertex3f(0.2, 0.2, 0.6);
		glVertex3f(0.2, 0.4, 0.6);
		glVertex3f(0.2, 0.4, 0.2);
		glVertex3f(0.2, 0.2, 0.2);

		/* back of cube.*/
		glVertex3f(0.6, 0.2, 0.6);
		glVertex3f(0.6, 0.5, 0.6);
		glVertex3f(0.6, 0.5, 0.2);
		glVertex3f(0.6, 0.2, 0.2);

		/* left of cube*/
		glVertex3f(0.2, 0.2, 0.6);
		glVertex3f(0.6, 0.2, 0.6);
		glVertex3f(0.6, 0.5, 0.6);
		glVertex3f(0.2, 0.4, 0.6);

		/* Right of cube */
		glVertex3f(0.2, 0.2, 0.2);
		glVertex3f(0.6, 0.2, 0.2);
		glVertex3f(0.6, 0.5, 0.2);
		glVertex3f(0.2, 0.4, 0.2);
		//****************************************************************************
		glVertex3f(0.7, 0.65, 0.6);
		glVertex3f(0.7, 0.65, 0.2);
		glVertex3f(1.7, 0.65, 0.2);        //top cover
		glVertex3f(1.7, 0.65, 0.6);
		//***************************back guard******************************
		glColor3f(r, g, b);            /* Set The Color To Blue*/
		glVertex3f(1.8, 0.5, 0.6);
		glVertex3f(1.8, 0.5, 0.2);
		glVertex3f(2.1, 0.4, 0.2);
		glVertex3f(2.1, 0.4, 0.6);

		/* bottom of cube*/
		glVertex3f(2.1, 0.2, 0.6);
		glVertex3f(2.1, 0.2, 0.2);
		glVertex3f(1.8, 0.2, 0.6);
		glVertex3f(1.8, 0.2, 0.6);

		/* back of cube.*/
		glVertex3f(2.1, 0.4, 0.6);
		glVertex3f(2.1, 0.4, 0.2);
		glVertex3f(2.1, 0.2, 0.2);
		glVertex3f(2.1, 0.2, 0.6);

		/* left of cube*/
		glVertex3f(1.8, 0.2, 0.2);
		glVertex3f(1.8, 0.5, 0.2);
		glVertex3f(2.1, 0.4, 0.2);
		glVertex3f(2.1, 0.2, 0.2);

		/* Right of cube */
		glVertex3f(1.8, 0.2, 0.6);
		glVertex3f(1.8, 0.5, 0.6);
		glVertex3f(2.1, 0.4, 0.6);
		glVertex3f(2.1, 0.2, 0.6);
		//******************MIDDLE BODY************************************
		glVertex3f(0.6, 0.5, 0.6);
		glVertex3f(0.6, 0.2, 0.6);
		glVertex3f(1.8, 0.2, 0.6);
		glVertex3f(1.8, 0.5, 0.6);

		/* bottom of cube*/
		glVertex3f(0.6, 0.2, 0.6);
		glVertex3f(0.6, 0.2, 0.2);
		glVertex3f(1.8, 0.2, 0.2);
		glVertex3f(1.8, 0.2, 0.6);

		/* back of cube.*/
		glVertex3f(0.6, 0.5, 0.2);
		glVertex3f(0.6, 0.2, 0.2);
		glVertex3f(1.8, 0.2, 0.2);
		glVertex3f(1.8, 0.5, 0.2);
		//*********************ENTER WINDOW**********************************
		glColor3f(0.3, 0.3, 0.3);
		glVertex3f(0.77, 0.63, 0.2);
		glVertex3f(0.75, 0.5, 0.2);        //quad front window
		glVertex3f(1.2, 0.5, 0.2);
		glVertex3f(1.22, 0.63, 0.2);

		glVertex3f(1.27, 0.63, .2);
		glVertex3f(1.25, 0.5, 0.2);        //quad back window
		glVertex3f(1.65, 0.5, 0.2);
		glVertex3f(1.67, 0.63, 0.2);

		glColor3f(r, g, b);
		glVertex3f(0.7, 0.65, 0.2);
		glVertex3f(0.7, 0.5, .2);       //first separation
		glVertex3f(0.75, 0.5, 0.2);
		glVertex3f(0.77, 0.65, 0.2);

		glVertex3f(1.2, 0.65, 0.2);
		glVertex3f(1.2, 0.5, .2);       //second separation
		glVertex3f(1.25, 0.5, 0.2);
		glVertex3f(1.27, 0.65, 0.2);

		glVertex3f(1.65, 0.65, 0.2);
		glVertex3f(1.65, 0.5, .2);     //3d separation
		glVertex3f(1.7, 0.5, 0.2);
		glVertex3f(1.7, 0.65, 0.2);

		glVertex3f(0.75, 0.65, 0.2);
		glVertex3f(0.75, 0.63, 0.2);        //line strip
		glVertex3f(1.7, 0.63, 0.2);
		glVertex3f(1.7, 0.65, 0.2);

		glVertex3f(0.75, 0.65, 0.6);
		glVertex3f(0.75, 0.63, 0.6);        //line strip
		glVertex3f(1.7, 0.63, 0.6);
		glVertex3f(1.7, 0.65, 0.6);

		glColor3f(0.3, 0.3, 0.3);
		glVertex3f(0.77, 0.63, 0.6);
		glVertex3f(0.75, 0.5, 0.6);        //quad front window
		glVertex3f(1.2, 0.5, 0.6);
		glVertex3f(1.22, 0.63, 0.6);

		glVertex3f(1.27, 0.63, .6);
		glVertex3f(1.25, 0.5, 0.6);        //quad back window
		glVertex3f(1.65, 0.5, 0.6);
		glVertex3f(1.67, 0.63, 0.6);

		glColor3f(r, g, b);
		glVertex3f(0.7, 0.65, 0.6);
		glVertex3f(0.7, 0.5, .6);       //first separation
		glVertex3f(0.75, 0.5, 0.6);
		glVertex3f(0.77, 0.65, 0.6);

		glVertex3f(1.2, 0.65, 0.6);
		glVertex3f(1.2, 0.5, .6);       //second separation
		glVertex3f(1.25, 0.5, 0.6);
		glVertex3f(1.27, 0.65, 0.6);

		glVertex3f(1.65, 0.65, 0.6);
		glVertex3f(1.65, 0.5, .6);
		glVertex3f(1.7, 0.5, 0.6);
		glVertex3f(1.7, 0.65, 0.6);
		glEnd();

		//**************************************************************
		glBegin(GL_QUADS);

		/* top of cube*/
		glColor3f(0.3, 0.3, 0.3);
		glVertex3f(0.6, 0.5, 0.6);
		glVertex3f(0.6, 0.5, 0.2);        //quad front window
		glVertex3f(0.7, 0.65, 0.2);
		glVertex3f(0.7, 0.65, 0.6);

		glVertex3f(1.7, 0.65, .6);
		glVertex3f(1.7, 0.65, 0.2);        //quad back window
		glVertex3f(1.8, 0.5, 0.2);
		glVertex3f(1.8, 0.5, 0.6);





		//*****************************road and surrounding development***********************************
		if (flag1)
		{
			glPushMatrix();
			glTranslatef(xw, 0, 0);
			glColor3f(0, 1, 0);
			glVertex3f(-100, 0.1, -100);
			glVertex3f(-100, 0.1, 0);         //a green surroundings
			glVertex3f(100, 0.1, 0);
			glVertex3f(100, 0.1, -100);

			glColor3f(0.7, 0.7, 0.7);
			glVertex3f(-100, 0.1, 0);
			glVertex3f(-100, 0.1, 0.45);         //a long road
			glVertex3f(100, 0.1, 0.45);
			glVertex3f(100, 0.1, 0);

			glColor3f(1.0, 0.75, 0.0);
			glVertex3f(-100, 0.1, 0.45);       //a median
			glVertex3f(-100, 0.1, 0.55);
			glVertex3f(100, 0.1, 0.55);
			glVertex3f(100, 0.1, 0.45);



			glColor3f(0.7, 0.7, 0.7);
			glVertex3f(-100, 0.1, 0.55);
			glVertex3f(-100, 0.1, 1);         //a long road
			glVertex3f(100, 0.1, 1);
			glVertex3f(100, 0.1, 0.55);

			glColor3f(0, 1, 0);
			glVertex3f(-100, 0.1, 1);
			glVertex3f(-100, 0.1, 100);         //a green surroundings
			glVertex3f(100, 0.1, 100);
			glVertex3f(100, 0.1, 1);
			glPopMatrix();
		}
		glEnd();

		if (wheelflag)
		{
			glPushMatrix();
			glTranslatef(xw, 0, 0);
			glColor3f(0.5, .2, 0.3);
			glBegin(GL_QUADS);
			for (i = 0; i < 200; i += 0.2)
			{
				glVertex3f(-100 + i, 0, 1);
				glVertex3f(-99.9 + i, 0, 1);
				glVertex3f(-99.9 + i, 0.2, 1);
				glVertex3f(-100 + i, 0.2, 1);
				i += 0.5;
			}
			for (i = 0; i < 200; i += 0.2)
			{
				glVertex3f(-100 + i, 0, 0);
				glVertex3f(-99.9 + i, 0, 0);
				glVertex3f(-99.9 + i, 0.2, 0);
				glVertex3f(-100 + i, 0.2, 0);
				i += 0.5;
			}
			glEnd();
			glPopMatrix();
		}
		//*************************************************************************************************
		glBegin(GL_TRIANGLES);                /* start drawing the cube.*/

		  /* top of cube*/
		glColor3f(0.3, 0.3, 0.3);
		glVertex3f(0.6, 0.5, 0.6);
		glVertex3f(0.7, 0.65, 0.6);       //tri front window
		glVertex3f(0.7, 0.5, 0.6);

		glVertex3f(0.6, 0.5, 0.2);
		glVertex3f(0.7, 0.65, 0.2);       //tri front window
		glVertex3f(0.7, 0.5, 0.2);

		glVertex3f(1.7, 0.65, 0.2);
		glVertex3f(1.8, 0.5, 0.2);       //tri back window
		glVertex3f(1.7, 0.5, 0.2);

		glVertex3f(1.7, 0.65, 0.6);
		glVertex3f(1.8, 0.5, 0.6);       //tri back window
		glVertex3f(1.7, 0.5, 0.6);

		glEnd();
		//************IGNITION SYSTEM**********************************
		glPushMatrix();
		glColor3f(0.7, 0.7, 0.7);
		glTranslatef(1.65, 0.2, 0.3);
		glRotatef(90.0, 0, 1, 0);
		gluCylinder(t, 0.02, 0.03, .5, 10, 10);
		glPopMatrix();
		//********************WHEEL*********************************************

		glColor3f(0.7, 0.7, 0.7);
		glPushMatrix();
		glBegin(GL_LINE_STRIP);
		for (theta = 0; theta < 360; theta = theta + 20)
		{
			glVertex3f(0.6, 0.2, 0.62);
			glVertex3f(0.6 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.62);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (theta = 0; theta < 360; theta = theta + 20)
		{
			glVertex3f(0.6, 0.2, 0.18);
			glVertex3f(0.6 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.18);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (theta = 0; theta < 360; theta = theta + 20)
		{
			glVertex3f(1.7, 0.2, 0.18);
			glVertex3f(1.7 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.18);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (theta = 0; theta < 360; theta = theta + 20)
		{
			glVertex3f(1.7, 0.2, 0.62);
			glVertex3f(1.7 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.62);
		}
		glEnd();
		glPopMatrix();
		glTranslatef(0.6, 0.2, 0.6);
		glColor3f(0, 0, 0);
		glutSolidTorus(0.025, 0.07, 10, 25);

		glTranslatef(0, 0, -0.4);
		glutSolidTorus(0.025, 0.07, 10, 25);

		glTranslatef(1.1, 0, 0);
		glutSolidTorus(0.025, 0.07, 10, 25);

		glTranslatef(0, 0, 0.4);
		glutSolidTorus(0.025, 0.07, 10, 25);
		//*************************************************************

		glPopMatrix();

		glEnable(GL_DEPTH_TEST);
		glutPostRedisplay();
		glutSwapBuffers();

	}
}

void NormalKey(GLubyte key, GLint x, GLint y)
{
    switch ( key )    {
     case ESCAPE : printf("escape pressed. exit.\n");
	   
	               exit(0);		/* Kill our window */
                   break;

      case ' ':view=1;
                   DrawGLScene();
                   break;

      case 'x': xangle += 5.0;
                      glutPostRedisplay();
                      break;

       case 'X':xangle -= 5.0;
                       glutPostRedisplay();
                       break;

      case 'y':
        yangle += 5.0;
        glutPostRedisplay();
        break;

     case 'Y':
        yangle -= 5.0;
        glutPostRedisplay();
        break;

     case 'z':
        zangle += 5.0;
        glutPostRedisplay();
        break;

     case 'Z':
        zangle -= 5.0;
        glutPostRedisplay();
        break;

      case 'u':                          /* Move up */
        yt += 0.2;
        glutPostRedisplay();
        break;

      case 'U':
        yt -= 0.2;                      /* Move down */
        glutPostRedisplay();
        break;

      case 'f':                          /* Move forward */
        zt += 0.2;
        glutPostRedisplay();
        break;

      case 'F':
        zt -= 0.2;                      /* Move away */
        glutPostRedisplay();
        break;

	  case 's':zs+=.2;					/* scale shallow */
		  glutPostRedisplay();
		  break;

	  case 'S':zs-=0.2;					/* scale deep */
		  glutPostRedisplay();
		  break;

	  case 'a':ys+=.2;					/* scale Up */
		  glutPostRedisplay();
		  break;

	  case 'A':ys-=0.2;					/* scale down */
		  glutPostRedisplay();
		  break;

	  case 'q':xs+=.2;					/* scale right */
		  glutPostRedisplay();
		  break;

	  case 'Q':xs-=0.2;					/* scale left */
		  glutPostRedisplay();
		  break;


     default:
	break;
    }

}

static void SpecialKeyFunc(int Key, int x, int y)
{
	switch (Key) {
	case GLUT_KEY_RIGHT:
		d=1;
		if (!wheelflag)
			xt += 0.15;
		if (wheelflag)
		{
			int v=0;
			timer(v);
			
		}
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		d=0;
		if (!wheelflag)
			if (!c)
				xt -= 0.15;
		if (wheelflag)
		{
			int v=0;
			timer(v);
		}
		glutPostRedisplay();
		break;
	}
}

void myMenu(int id)
{
	if (id == 1)//car model mode 
	{
		flag1 = 0;
		wheelflag = 0;
		glutPostRedisplay();

	}
	if (id == 2)// car driving mode
	{
		flag1 = 1;
		flag2 = 0;
		wheelflag = 0;
		xangle = 5.0;
		glutPostRedisplay();
	}
	if (id == 3) // fog 
	{
		flag2 = 1;
		wheelflag = 0;
		xangle = 5.0;
		glutPostRedisplay();
	}
	if (id == 4) // wheel animation
	{
		
		wheelflag = 1;
		glutPostRedisplay();
	}
	if (id == 5) // toggle light 
	{
		if (day)
		{

			if (light)
			{
				count++;
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
				light = 0;
			}
			else
			{
				count--;
				light = 1;
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
			}
			glutPostRedisplay();
		}
		else
		{

			if (nml == 0 && flag2 == 2)
			{
				flag2 = 0;
				nml = 1;
			}
			else
			{
				flag2 = 2;
				nml = 0;

				aflag = 0;
				day = 0;

				glClearColor(0.1, 0.1, 0.1, 0);
				GLfloat fogcolour[4] = { 0.0,0.0,0.0,1.0 };

				glFogfv(GL_FOG_COLOR, fogcolour);              /* Define the fog colour */
				glFogf(GL_FOG_DENSITY, 0.5);                   /* How dense */
				glFogi(GL_FOG_MODE, GL_EXP);                   /* exponential decay */
						/* end */
				glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
				glEnable(GL_FOG);


				glutPostRedisplay();
			}
		}

	}


	if (id == 12) // day mode
	{
		aflag = 1;
		day = 1;
		glClearColor(1, 1, 1, 1);
		glDisable(GL_FOG);
		glutPostRedisplay();
	}

	if (id == 13) // night mode
	{
		aflag = 0;
		day = 0;
		flag2 = 2;
		glClearColor(0.1, 0.1, 0.1, 0);
		GLfloat fogcolour[4] = { 0.0,0.0,0.0,1.0 };

		glFogfv(GL_FOG_COLOR, fogcolour);              /* Define the fog colour */
		glFogf(GL_FOG_DENSITY, 0.5);                   /* How dense */
		glFogi(GL_FOG_MODE, GL_EXP);                   /* exponential decay */
							   /* end */
		glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
		glEnable(GL_FOG);

		glutPostRedisplay();
	}
}

void colorMenu(int id)
{
		if (id==6)
	{
		r=g=0;
		b=1;
	glutPostRedisplay();

	}
	if(id ==7)
	{
		r=0.8;
		b=g=0;
		glutPostRedisplay();
	}
	if(id==8)
	{
	    g=1;
		r=b=0;
		glutPostRedisplay();
	}
	if (id==9)
	{
		 r=b=g=0;
		 glutPostRedisplay();
	}
	if(id==10)
	{
		b=0;
		r=g=1;
        glutPostRedisplay();
	}
	if(id==11)
	{
		b=r=g=.7;
        glutPostRedisplay();
	}

}

void myreshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat ratio = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, ratio, zNear and zFar
   gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	gluLookAt(
	   0,3,8,
	   0,2,-10,
	   0,2,0f
	   );
   glMatrixMode(GL_MODELVIEW);
}

//*************************** Main ***************************************************************

int main(int argc, char **argv)
{
/* Initialisation and window creation */

  glutInit(&argc, argv);               /* Initialize GLUT state. */

  glutInitDisplayMode(GLUT_RGBA |      /* RGB and Alpha */
                      GLUT_DOUBLE|     /* double buffer */
                      GLUT_DEPTH);     /* Z buffer (depth) */

  glutInitWindowSize(width,height);
  glutInitWindowPosition(0,0);         /* upper left corner of the screen. */

  glutCreateWindow("Car simulation"); /* Open a window with a title. */

/* Now register the various callback functions */

  glutReshapeFunc(myreshape);
  glutDisplayFunc(DrawGLScene);        /* Function to do all our OpenGL drawing. */
  glutReshapeFunc(ReSizeGLScene);
  glutKeyboardFunc(NormalKey);         /*Normal key is pressed */
  glutSpecialFunc( SpecialKeyFunc );
  InitGL(width,height);
  int submenu=glutCreateMenu(colorMenu);
  glutAddMenuEntry("blue", 6);
	glutAddMenuEntry("red", 7);
	glutAddMenuEntry("green",8);
	glutAddMenuEntry("black",9);
	glutAddMenuEntry("yellow",10);
	glutAddMenuEntry("grey",11);
  glutCreateMenu(myMenu);
	glutAddMenuEntry("car model mode", 1);
	glutAddMenuEntry("car driving mode", 2);
	glutAddMenuEntry("fog effect",3);
	glutAddMenuEntry("wheel effect",4);
	glutAddMenuEntry("toggle light",5);
	glutAddSubMenu("car colors",submenu);
	glutAddMenuEntry("daymode",12);
	glutAddMenuEntry("Night mode",13);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutTimerFunc(100, timer, 0);
  /* Now drop into the event loop from which we never return */

  glutMainLoop();                      /* Start Event Processing Engine. */
  return 1;
}

