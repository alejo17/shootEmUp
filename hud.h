#include <iostream>
#include <GL/glut.h>
#include "TextureManager.cpp"
using namespace std;

#define SIZE_HUD 0.5
#define TOTAL_LIFE 100

extern float life;
extern float energy;

void display_text(float x, float y, const char *string1)
{
	glPushMatrix();
		glTranslatef( x, y, -20);
		glRasterPos2f( 0,0 ) ; 

		char buf[300];

		sprintf( buf, string1 , "%s") ;
		const char * p1 = buf ;
		do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *p1 ); while( *(++p1) ) ;
	glPopMatrix();
}

void rectangulo(float x, float y, float largo , float alto, float life, float r, float g, float b)
{
	glPushMatrix();
		glTranslatef(x, y, -20);
		
		glBegin(GL_QUADS);

			glColor3d(r,g,b);
			glVertex2d(0, 0);
			glVertex2d(0, -alto);
			glVertex2d(-largo*life/TOTAL_LIFE, -alto);
			glVertex2d(-largo*life/TOTAL_LIFE, 0);
	
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
}

class Hud
{
	public:
		Hud(string hud_t, string animation_t, string nave)
		{
			hud_texture = TextureManager::Inst()->LoadTexture(hud_t.c_str(), GL_BGRA_EXT, GL_RGBA);
			animation_texture = TextureManager::Inst()->LoadTexture(animation_t.c_str(), GL_BGRA_EXT, GL_RGBA);
			nave_texture = TextureManager::Inst()->LoadTexture(nave.c_str(), GL_BGRA_EXT, GL_RGBA);
			time_var = 0;
			time_varbase = 0;
			anim = 0;
			i = 0;
		}

		void exe()
		{
			glDisable(GL_LIGHTING);

			display_life();
			display_score();

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			animate_character();
			display_frame();

			//glEnd();
			glDisable(GL_BLEND);
			glEnable(GL_LIGHTING);
		}

		void display_frame()
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, hud_texture);

			glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color

			glBegin(GL_QUADS);

			float z = -0.60f;

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(SIZE_HUD/2, SIZE_HUD/2,z);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(SIZE_HUD/2, -SIZE_HUD/2,z);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f( -SIZE_HUD/2, -SIZE_HUD/2,z); 

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-SIZE_HUD/2, SIZE_HUD/2,z);

			glEnd();

			glDisable(GL_TEXTURE_2D);
		}

		void display_character(int pos)
		{
			float number_frames_in_a_row = 8;
			float number_rows = 1;
			float factor_x = 1.0 / number_frames_in_a_row;	
			float factor_y =  1.0 / number_rows;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, animation_texture);

			glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color

			glPushMatrix();
			glTranslatef(0.7, 0.65, -0.1);
			glBegin(GL_QUADS);

			float z = -2.0f;

			glTexCoord2f(0.0f + factor_x*i, factor_y*(pos-1));
			glVertex3f(SIZE_HUD/2, SIZE_HUD/2,z);

			glTexCoord2f(0.0f + factor_x*i, factor_y*pos); 
			glVertex3f(SIZE_HUD/2, -SIZE_HUD/2,z);

			glTexCoord2f(factor_x*(i + 1.0), factor_y*pos); 
			glVertex3f( -SIZE_HUD/2, -SIZE_HUD/2,z); 

			glTexCoord2f(factor_x*(i + 1.0), factor_y*(pos-1));
			glVertex3f(-SIZE_HUD/2, SIZE_HUD/2,z);

			glEnd();
			glPopMatrix();

			glDisable(GL_TEXTURE_2D);
		}

		void display_nave(int pos)
		{
			float number_frames_in_a_row = 8;
			float number_rows = 1;
			float factor_x = 1.0 / number_frames_in_a_row;	
			float factor_y =  1.0 / number_rows;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, nave_texture);

			glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color

			glPushMatrix();
			glTranslatef(-0.33, 0.6, -0.1);
			glBegin(GL_QUADS);

			float z = -2.0f;

			glTexCoord2f(0.0f + factor_x*i, factor_y*(pos-1));
			glVertex3f(SIZE_HUD/2, SIZE_HUD/2,z);

			glTexCoord2f(0.0f + factor_x*i, factor_y*pos); 
			glVertex3f(SIZE_HUD/2, -SIZE_HUD/2,z);

			glTexCoord2f(factor_x*(i + 1.0), factor_y*pos); 
			glVertex3f( -SIZE_HUD/2, -SIZE_HUD/2,z); 

			glTexCoord2f(factor_x*(i + 1.0), factor_y*(pos-1));
			glVertex3f(-SIZE_HUD/2, SIZE_HUD/2,z);

			glEnd();
			glPopMatrix();

			glDisable(GL_TEXTURE_2D);
		}

		void animate_character()
		{
			time_var = glutGet(GLUT_ELAPSED_TIME); 
			float dt = time_var -time_varbase;
			time_varbase = time_var;
			anim += dt;

			if (anim / 5000.0 > 0.2){
				i++;
				anim = 0.0;
			}
			if (i == 8) i = 0;

			display_character(i);
			display_nave(i);
		}

		void display_life()
		{
			rectangulo(6.3, -6.5, 3.8 ,0.8, life, 1,0,0);
			rectangulo(6.3, -5.5, 3.8 ,0.8, energy, 0,0.5,1);
		}

		void display_score()
		{
			char array[10];
			snprintf(array, sizeof(array), "%f", life);
			display_text(6, -4.8, "SCORE: ");
			display_text(4.5, -4.8, array);
		}
		
		~Hud(){;}

	private:
		GLint hud_texture;
		GLint animation_texture;
		GLint nave_texture;
		int time_var;
		int time_varbase;
		int anim ;
		int i;

};