// g++ -o main main.cpp -lSDLmain -lSDL -lGL -lSDL_image
#include <SOIL/SOIL.h>
#include <SDL/SDL.h>
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include "SDL/SDL_image.h"
#include "string"
#include <iostream>
//#include <stlib.h>

#define BLOCOS 48 // quantidade de blocos

GLuint loadTexture(const std::string&fileName)
{
	SDL_Surface *imagem = IMG_Load(fileName.c_str()); //carrega imagem
	SDL_DisplayFormatAlpha(imagem); // formato alpha 
	unsigned objetct(0); //cria textura
	glGenTextures(1, &objetct); //gera textura
	glBindTexture(GL_TEXTURE_2D, objetct); // tipo de textura, 2D

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, imagem->w, imagem->h,0, GL_RGBA, GL_UNSIGNED_BYTE,imagem->pixels);

	SDL_FreeSurface(imagem);

	return objetct;
}

//A -elemento 1  - inimigo
//B -elemento 2  - personagem o outro elemento

bool  colisao(float Ax, float Ay, float Acomp, float Aalt, float Bx, float By,float Bcomp, float Balt )
{
	if(Ay+Aalt <By) return false;
	else if(Ay > By+Balt) return false;
	else if(Ax+Acomp<Bx) return false;
	else if (Ax > Bx+Bcomp) return false;

	return true; // houve colisao
}

struct Bloco
{
		float x, y, comp, alt;
		bool vivo;
};

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
	SDL_WM_SetCaption("SpaceSuper M", NULL);

	//tamanho janela
	SDL_SetVideoMode(800,800,32,SDL_OPENGL);

	//cor
	glClearColor(0,0,0,1);

	//area exibida
	glViewport(0,0,800,800);

	//sombra
	glShadeModel(GL_SMOOTH);

	//2D
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // desenho geometrico

	//3D
	glDisable(GL_DEPTH_TEST);

	// uso da imagem
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	bool vitoria = false;
	bool executando = true;
	bool paused = true;
	bool modoGod = true;
	int vidasinimigoE =6;
	bool vivo = true;
	bool comeco= true;
	bool comecoN= true;
	bool gameOver = true;



	SDL_Event eventos;
	//personagem
	float personX=300;
	float personY=340;
	float personComp = 80;
	float personAlt = 40;
	

	//tiro Obs.: o tiro chama INIMIGO no código (além de tiro e atirar)
	float inimigoX= personX+(personComp/2); // posicao central do personagem
	float inimigoY= personY-personAlt;
	float inimigoCA= 10;
	float inimigoCAy = 20;
	bool  tiroVivo;
	bool  atirar = false;

	
	// velocidade de movimento
	float velX = 3;
	float velY = 5;

	Bloco blocos[BLOCOS];

	//criação dos blocos "inimigos"
	for( int n = 0, x = 100, y = 10, t=25; n <BLOCOS; n++, x +=50)
	{ if (n<8){
		
	} else {
		t=0;
	}
		if (x > 460)
		{
			x = 100;
			y += 45;
		}

		blocos[n].x = x;
		blocos[n].y = y;
		blocos[n].comp = 15+t;
		blocos[n].alt = 15;
		blocos[n].vivo = true;
	}	

	bool esq = false, dir = false;

	unsigned int person_textura = 0;
	person_textura = loadTexture("pngegg.png");

	unsigned int fundo_textura =0;
	fundo_textura = loadTexture("spaceG.png");

	unsigned int inimigos_textura =0;
	inimigos_textura = loadTexture("iniE.png");

	unsigned int inimigosF_textura =0;
	inimigosF_textura = loadTexture("iniF.png");

	unsigned int tiro_textura =0;
	tiro_textura = loadTexture("tiro1.png");

	unsigned int inicio_textura =0;
	inicio_textura = loadTexture("spaceH.png");

	unsigned int fim_textura =0;
	fim_textura = loadTexture("spaceH.png");

	unsigned int vitoria_textura =0;
	vitoria_textura = loadTexture("winner1.png");



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
				paused = false;
			}
			// fecha com esc
			if(eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_ESCAPE)
			{
				executando = false;
				paused = false;
			}

			//pause 
			if(eventos.type == SDL_KEYDOWN)// tecla pressionada
			{
				if ((eventos.key.keysym.sym == SDLK_p)&& ( comecoN == false))  //|| ( SDL_MOUSEMOTION <200))
				{
					paused = !paused;   
				}
			}

			//start 
			if(eventos.type == SDL_KEYDOWN)// tecla pressionada
			{
				if ((eventos.key.keysym.sym == SDLK_s) && ( comecoN == true)) //|| ( SDL_MOUSEMOTION <200))
				{
					comeco = !comeco; 
					comecoN = false;
				}
			}

			//modoGod  (fica imortal para os inimigos pequenos)
			if(eventos.type == SDL_KEYDOWN)// tecla pressionada
			{
				if (eventos.key.keysym.sym == SDLK_1)  //|| ( SDL_MOUSEMOTION <200))
				{
					modoGod = !modoGod;   
				}
			}

			// eventos de ações: movimentos com 'setas' ou 'a' e 'd'. 'Espaço' atira.
			if(eventos.type == SDL_KEYDOWN)// tecla pressionada
			{
				if ((eventos.key.keysym.sym == SDLK_LEFT) || (eventos.key.keysym.sym == SDLK_a)) //|| ( SDL_MOUSEMOTION <200))
				{
					esq = true;
				}
				else if((eventos.key.keysym.sym == SDLK_RIGHT)|| (eventos.key.keysym.sym == SDLK_d))
				{
					dir = true;
				}
			}
			else if (eventos.type == SDL_KEYUP)
			{
				if((eventos.key.keysym.sym == SDLK_LEFT) || (eventos.key.keysym.sym == SDLK_a))
				{
					esq = false;
				}
				else if ((eventos.key.keysym.sym == SDLK_RIGHT)|| (eventos.key.keysym.sym == SDLK_d))
				{
					dir = false;
				}
			}

		}

		
	if((!paused)&&(comeco == false)){

			//LOGICA

			//movimento do personagem
			if(esq == true) // se seta esqueda for pressionada
			{
				personX -=8;
				inimigoX = personX;
				 
			}
			else if (dir == true)// se seta direita pressionada
			{
				personX +=8;
				inimigoX = personX;
			}
			if(personX<0)
			{
				personX = 0;
				inimigoX = personX + (personComp/2);
				
			}
			else if(personX + personComp >600)
			{
				personX = 600- personComp;
				inimigoX = personX + (personComp/2);			
			}
		

			//atirar				
				if (eventos.type == SDL_KEYDOWN)
					if ((eventos.key.keysym.sym == SDLK_SPACE))
				{			
			
					atirar = true;
					tiroVivo = true;

					
				} 	else if (eventos.type == SDL_KEYUP)
					if ((eventos.key.keysym.sym == SDLK_SPACE))
					{
						atirar = false;
						tiroVivo = false;
						inimigoX = personX + (personComp);
						inimigoY = personY ;

					}
			

			//movimento tiro 

			
				if (atirar == true)
				{
					inimigoY-=velY;
				}	
			
			 
			 //fim do jogo ..personagem morreu 
				if(vivo == false){
					SDL_Delay(2000);
					executando= false;
				}				
								

			float vely = 0.1;//velecoidade dos BLOCOS (inimigos)
			float velx = 1;
			//Logica de colisão e movimentoDosBlocos
			for (int n = 8; n < BLOCOS; n++)
			{    
				blocos[n].y +=vely;
				blocos[n].x +=velx;

				if (blocos[n].vivo == true)
				{					 
				 if((blocos[n].x >550) && (velx != (-2)))
				{	
					velx = -2.5;							 
				
				}else
				if((blocos[n].x <0))
				{
					velx = 2.5;				
										
				}
				}
				
				//se ativar modo god fica imortal pros inimigos pequenos
				if(modoGod){

					//colisão do personagem com BLOCOS e BLOCOS com o fim da tela
				if (blocos[n].vivo == true)
				{	//((colisao(blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].alt, personX, personY, personComp, personAlt) == true) || (blocos[n].y >400))
					if (colisao(blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].alt, personX, personY, personComp, personAlt) == true) 
				{
					vivo = false;				
					
				} 
			 }
			}
					if (blocos[n].vivo == true)
				{					
					//colisão do tiro com os Blocos	
					if(colisao(inimigoX, inimigoY, inimigoCA, inimigoCAy, blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].alt) == true)
					{
						
						blocos[n].vivo = false;
						tiroVivo = false;
						atirar = false;
						inimigoX = personX + (personComp/2)-3;
						inimigoY = personY +2;					
						
						break;
					} 
				}
			}
						
			
			//inimigos especiais
			for (int n = 0; n < 8; n++)
			{ blocos[n].y +=0.1;
				if (blocos[n].vivo == true)
				{	
					if ((colisao(blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].alt, personX, personY, personComp, personAlt) == true) || (blocos[n].y >400))
				{
					vivo = false;				
					
				}
				} 		 
			
					if (blocos[n].vivo == true)
				{					
					//colisão do tiro com os Blocos	
					if(colisao(inimigoX, inimigoY, inimigoCA, inimigoCAy, blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].alt) == true)
					{
						
						--vidasinimigoE;
						blocos[n].y -=1;  // inimigo sente o impacto do tiro ^^

						if (vidasinimigoE == 0)
						{
							vitoria = true;
						}
												
						tiroVivo = false;
						atirar = false;
						inimigoX = personX + (personComp/2)-3;
						inimigoY = personY +2;											
						
						break;
					} 
				}
			}

		
		}	// até onde o "paused" funciona 
			
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

			glColor4ub(255,255,255,255); //branco

			//inicia as texturas e o jogo (start)
			if(comeco) 
			{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, inicio_textura);
			glBegin(GL_TRIANGLE_FAN);

			glTexCoord2d(0,0); glVertex3f(0,0,1);
			glTexCoord2d(1,0); glVertex3f(600,0,1);
			glTexCoord2d(1,1); glVertex3f(600,400,1);			
			glTexCoord2d(0,1); glVertex3f(0,400,1);


			glEnd();	}

			if(!comeco) 
			{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, fundo_textura);
			glBegin(GL_TRIANGLE_FAN);

			glTexCoord2d(0,0); glVertex3f(0,0,1);
			glTexCoord2d(1,0); glVertex3f(600,0,1);
			glTexCoord2d(1,1); glVertex3f(600,400,1);			
			glTexCoord2d(0,1); glVertex3f(0,400,1);

			glEnd();

			glBindTexture(GL_TEXTURE_2D, person_textura);

			
			//inicia desenho personagem
			glBegin(GL_QUADS); // GL_POINTS , GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON

			glTexCoord2d(0,0); glVertex3f(personX,personY,0);
			glTexCoord2d(1,0); glVertex3f(personX+personComp,personY,0);
			glTexCoord2d(1,1); glVertex3f(personX+personComp,personY+personAlt,0);			
			glTexCoord2d(0,1); glVertex3f(personX,personY+personAlt,0);

			//fecha
			glEnd();

			

			//inicia desenho tiro			
				if ((atirar == true) && (tiroVivo==true))
				{

				glBindTexture(GL_TEXTURE_2D, tiro_textura);
				glBegin(GL_QUADS); // GL_POINTS , GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
				
				glTexCoord2d(0,0);	glVertex2f(inimigoX,inimigoY);
				glTexCoord2d(1,0);	glVertex2f(inimigoX+inimigoCA,inimigoY);
				glTexCoord2d(1,1);	glVertex2f(inimigoX+inimigoCA,inimigoY+inimigoCAy);			
				glTexCoord2d(0,1);	glVertex2f(inimigoX,inimigoY+inimigoCAy);

				//fecha
			
				glEnd();
				}

				if(inimigoY<10){ //reseta a posição do tiro
					atirar= false;
					tiroVivo=false;
					inimigoX = personX + (personComp/2)-3;
					inimigoY = personY +2;
				}
			

			glBindTexture(GL_TEXTURE_2D, inimigos_textura);		

			glBegin(GL_QUADS);			

			for (int n = 8; n < BLOCOS; n++)
			{
				if(blocos[n].vivo == true){

				glTexCoord2d(0,0);	glVertex2f(blocos[n].x, blocos[n].y);
				glTexCoord2d(1,0);	glVertex2f(blocos[n].x + blocos[n].comp, blocos[n].y);
				glTexCoord2d(1,1);	glVertex2f(blocos[n].x + blocos[n].comp, blocos[n].y + blocos[n].alt);
				glTexCoord2d(0,1);	glVertex2f(blocos[n].x, blocos[n].y + blocos[n].alt);
				}
			}

			glEnd();

			glBindTexture(GL_TEXTURE_2D, inimigosF_textura);		

			glBegin(GL_QUADS);

			for (int n = 0; n < 8; n++)
			{
				if(blocos[n].vivo == true){

				glTexCoord2d(0,0);	glVertex2f(blocos[n].x, blocos[n].y);
				glTexCoord2d(1,0);	glVertex2f(blocos[n].x + blocos[n].comp, blocos[n].y);
				glTexCoord2d(1,1);	glVertex2f(blocos[n].x + blocos[n].comp, blocos[n].y + blocos[n].alt);
				glTexCoord2d(0,1);	glVertex2f(blocos[n].x, blocos[n].y + blocos[n].alt);
				}
			}


			glEnd();
			glDisable(GL_TEXTURE_2D);
		}

		// vitoria personagem ganhou
				if(vitoria == true)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, vitoria_textura);
					glBegin(GL_TRIANGLE_FAN);

					glTexCoord2d(0,0); glVertex3f(100,100,1);
					glTexCoord2d(1,0); glVertex3f(500,100,1);
					glTexCoord2d(1,1); glVertex3f(500,300,1);			
					glTexCoord2d(0,1); glVertex3f(100,300,1);

					glEnd();

					vivo = false;					
				}

			//fecha matrix
			glPopMatrix();

			//animacao
			SDL_GL_SwapBuffers();

	}
		

	printf("\n Executando \n");

	SDL_Quit();

	return 0;

}

















