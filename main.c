// g++ -o main main.cpp -lSDLmain -lSDL -lGL

#include <SDL/SDL.h>
#include "SDL/SDL_opengl.h"
#include <stdio.h>

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	//memoria
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// nome janela
	SDL_WM_SetCaption("Meu primeiro jogo SDL", NULL);

	//tamanho janela
	SDL_SetVideoMode(600,400,32,SDL_OPENGL);

	//cor
	glClearColor(1,1,1,1);

	//area exibida
	glViewport(0,0,600,400);

	//sombra
	glShadeModel(GL_SMOOTH);

	//2D
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // desenho geometrico

	//3D
	glDisable(GL_DEPTH_TEST);

	bool executando = true;

	SDL_Event eventos;

	//loop do jogo
	while(executando)
	{
		//eventos
		while(SDL_PollEvent(&eventos))
		{
			//fecha com o x da janela
			if(eventos.type == SDL_QUIT)
			{
				executando = false;
			}
			//esc
			if(eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_ESCAPE)
			{
				executando = false;
			}
	

			//LOGICA

			//RENDERIZACAO
			glClear(GL_COLOR_BUFFER_BIT); //LIMPA O BUFFER

			//inicia matriz
			glPushMatrix();

			//dimensoes da matriz
			glOrtho(0,600,400,0,-1,1);

			//glColor3d(1 ou 0)
			//glColor3f(decimais)
			//glColor3ub(R,G,B) 0 ate 255
			//glColor4ub(R,G,B,A) 0 ate 255

			glColor4ub(255,0,0,255); //vermelho

			//inicia desenho
			glBegin();


			//fecha
			glEnd();


			//fecha matrix
			glPopMatrix();


			//animacao
			SDL_GL_SwapBuffers();

		}

	printf("\n Executando \n");

	SDL_Quit();

	return 0;

}

















}