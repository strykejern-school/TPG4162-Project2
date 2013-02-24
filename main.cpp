//
//  main.cpp
//  TPG4162-Project2
//
//  Created by Anders Eie on 11/7/12.
//  Copyright (c) 2012 Anders Eie. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>

#include "segy.h"

#ifdef __APPLE__
#include <glew.h>
#include <GLUT/GLUT.h>
#include <OpenGL/glext.h>
#elif defined (_WIN32)
#include <GL\glew.h>
#include <GL\glut.h>
#include <GL\GL.h>
#endif

using namespace std;

GLuint program, glsplit, gltex;
GLfloat	xrot, yrot, xspeed, yspeed;
GLfloat	z=-5.0f;

GLuint texture;

// Split value
float	split = 0.5f;
GLfloat zoom = 1.0f;

segy *seg;

char *readTextFile(const char *filename){
    ifstream TFile;
    TFile.open(filename);
    
    if (TFile.is_open()){
        TFile.seekg(0, ios::end);
        int length = (int)TFile.tellg();
        TFile.seekg(0, ios::beg);
        
        char *buffer = new char[length+1];
        
        TFile.read(buffer, length);
        
        buffer[length] = '\0';
        
        TFile.close();
        
        return buffer;
    }
    else{
        cout << "Error opening file" << endl;
        throw 404;
    }
}

void setGLSLvariables()
{
	glUseProgram(program);
    
	glUniform1f(glsplit, split);
    
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE0, texture);
	glUniform1i(gltex, 0);
}

void disableGLSL()
{
	glUseProgram(NULL);
}

static GLint glutTime = 0;
static void display(void)
{
	GLint bufferTime = glutGet(GLUT_ELAPSED_TIME);
	GLint deltaTime = bufferTime - glutTime;
	float factor = 0.3f * deltaTime;
	if (deltaTime < 0.0f) factor = 0.001f;
	glutTime = bufferTime;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	glTranslatef(0.0f,0.0f,z);
    
    glScalef(zoom, zoom, zoom);
    
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
    
	setGLSLvariables();
    
	glBegin(GL_QUADS);
        // Front Face
        glNormal3f( 0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        // Back Face
        glNormal3f( 0.0f, 0.0f,-1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        // Top Face
        glNormal3f( 0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        // Bottom Face
        glNormal3f( 0.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        // Right face
        glNormal3f( 1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
    
	xrot+=xspeed * factor;
	yrot+=yspeed * factor;
    
	disableGLSL();
    
    glutSwapBuffers();
}

void resize(GLint width, GLint height){
    if (height == 0 || width == 0) return;
    
	glViewport(0,0,width,height);						// Reset The Current Viewport
    
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
    
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
    
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

void GLSLInit(){
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    
    const char *vs = readTextFile("glsl.vert");
    const char *fs = readTextFile("glsl.frag");
    
    glShaderSource(vshader, 1, &vs, NULL);
    glShaderSource(fshader, 1, &fs, NULL);
    
    glCompileShader(vshader);
	glCompileShader(fshader);
    
	GLint status = 0;
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) cout << "Vertex shader compile error:" << endl << vs;
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) cout << "Fragment shader compile error" << endl << fs;
    
	program = glCreateProgram();
    
	glAttachShader(program,fshader);
	glAttachShader(program,vshader);
    
	glLinkProgram(program);
    
	glsplit = glGetUniformLocation(program, "splitter");
	gltex = glGetUniformLocation(program, "tex");
    
    delete [] vs;
    delete [] fs;
}

void CreateGLTextures()
{
	glGenTextures(1, &texture);
    
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    
	unsigned char* c_texture = new unsigned char[1024*1024];   // had to use the heap (too large for the stack)
    
    char** dataPoints = seg->getDataPoints(); // Get a 2D matrix of the data points from the segy file
    
	for(int i=0;i<1024;i++)
	{
		for(int j=0;j<1024;j++){
            int value = 128 + (int)dataPoints[1024-j][1024-i];
            c_texture[i*1024+j] = value;
        }
	}
    
	glBindTexture(GL_TEXTURE_2D, texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE, 1024, 1024, GL_LUMINANCE, GL_UNSIGNED_BYTE, c_texture);
    
    delete [] c_texture;
    seg->deallocate();
}

void initGL(){
    glewInit();
	CreateGLTextures();
    
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);				// Grey Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    
	GLfloat lPos[] = {0.0f, 0.0f, 5.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lPos);

	GLSLInit();
}

void keyboard(unsigned char button, int x, int y){
    switch (button){
        case 'd':
            yspeed += 0.1f;
            break;
        case 'a':
            yspeed -= 0.1f;
            break;
        case 'w':
            xspeed += 0.1f;
            break;
        case 's':
            xspeed -= 0.1f;
            break;
        case 'o':
            split -= 0.01f;
            if (split < 0.0f) split = 0.0f;
            cout << "Splitter: " << split << endl;
            break;
        case 'l':
            split += 0.01f;
            if (split > 1.0f) split = 1.0f;
            cout << "Splitter: " << split << endl;
            break;
        case '+':
            zoom *= 1.1f;
            break;
        case '-':
            zoom /= 1.1f;
            break;
        default:
            cout << button << endl;
            break;
    }
    //display();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    
    seg = new segy("NVGT-88-06.sgy", 3600, 240, 13483, 1750);
    
    xrot = 0;
    yrot = 0;
    xspeed = 0;
    yspeed = 0;
    
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    
    initGL();
    
    glutMainLoop();
}