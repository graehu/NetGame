#include "header/renderer.h"
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cassert>
#include <cmath>

Renderer::Renderer()
{
    //ctor
    xRes = 800;
    yRes = 600;
    int result = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
        assert(result == 0);
    SDL_Surface* screen = SDL_SetVideoMode(xRes, yRes, 0, SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF);
        assert(screen != NULL);

    glViewport(0,0,xRes,yRes);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, xRes, yRes, 0, 0, 1); // Parralel projection (no 3d or depth test)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
	glClearColor(0,0,0,0);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glPointSize(5);
	//glLineWidth(5);
}

Renderer::~Renderer()
{
    //dtor
}

bool Renderer::beginScene()
{

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, xRes, yRes, 0, 0, 1); // Parralel projection (no 3d or depth test)

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(xRes/2,yRes/2,0);
    return true;
}
void Renderer::draw(float xPos, float yPos, float xDir, float yDir)
{
    float length = sqrt((xDir*xDir)+(yDir*yDir));
    xDir /= length/15.0f;
    yDir /= length/15.0f;
    glBegin(GL_LINE_STRIP);
    glColor3f(1,0,0);
    glVertex2f(xPos,yPos);
    glColor3f(0,1,0);
    glVertex2f(xPos+xDir,yPos+yDir);
    glEnd();
}
void Renderer::endScene()
{
	glFlush();
    glFinish();
	SDL_GL_SwapBuffers();
    GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR)
	{
	    throw (const char*)gluErrorString(errorCode);
	}
}


