#include <stdio.h>
#include <iostream>
#include <GL/glut.h>

#include "objloader.h"
#include "shot.h"
#include "sounds.h"

using namespace std;

#define DIST_ENEMY 60

typedef struct 
{
	GLfloat nx, ny, nz;
	GLfloat x , y , z;
} 	PuntoShip;


class Ship
{
	public:
		Ship(const char * dir)
		{
			loadOBJ(dir, vertices, uvs, normals );

			point_list = new PuntoShip[vertices.size()];

			for(int i = 0 ; i < vertices.size() ; i++)
			{
				point_list[i].x = vertices[i].x;
				point_list[i].y = vertices[i].y;
				point_list[i].z = vertices[i].z;

				point_list[i].nx = normals[i].x;
				point_list[i].ny = normals[i].y;
				point_list[i].nz = normals[i].z;
			}

			wav_sounds = new Folder_Sound ("mario/", "wav");
		};

		~Ship(){cout<<"eliminado"<<endl;}

		void display()
		{
			glPushMatrix();
			glTranslatef(0,0,-52);
			glRotatef(0,0,1,0);//180 en y
	    	
	    	glInterleavedArrays(GL_N3F_V3F, sizeof(PuntoShip), (void*)point_list);
			glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

			glPopMatrix();
		}

		void new_normal_shot ()
		{
			//cout<<"disparo"<<endl;
			wav_sounds->play_random();
			Nave_shot * shot = new Nave_shot (x,y,z);
			shots.push_back(shot);
		}

		void display_normal_shots()
		{
			for (int i = 0; i < shots.size(); ++i)
			{
				shots[i]->display();
			}
		}

		void change_position(float x, float&y , float&z)
		{
			this->x = x; 
			this->y = y; 
			this->z = z;
		}

		void show_position()
		{
			// printf("Pos: ( %.2f , %.2f , %.2f )\n" , x , y , z);
		}

		void get_position_matrix(int&tx, int&ty, int&tz)
		{
			// cout<<"get"<<x<<" "<<y<<" "<<z<<endl;
			// cout<<"get position get_position_matrix"<<x<<" "<<y<<" "<<z<<endl;
			tx = -tx;
			ty = -ty;

			if(x == -0) x = 0;
			// cout<<"modulo "<<fmod(0,1)<<endl;

			if(fmod(x,1) < 0.5 && fmod(x,1) > 0)
			{
				if(x > 0)
					tx = x-0.5;
				if(x < 0)
					tx = x+0.5;
			}
			else
			{
				tx = x;
			}
			
			if(fmod(y,1) < 0.5 && fmod(y,1) > 0)
			{
				if(y > 0)
					ty = y-0.5;
				if(y < 0)
					ty = y+0.5;
			}
			else
			{
				ty = y;
			}
			
			tx += 9;
			ty += 9;
			
			if(fmod(z,1) == 0.5)
			{
				tz = z-0.5;
			}
			else
			{
				tz = z;
			}
			// cout<<tx<<ty<<tz<<endl;
		}

		float x, y, z;

	private:
		vector<glm::vec3> vertices;
		vector<glm::vec2> uvs;
		vector<glm::vec3> normals;

		PuntoShip *point_list;
		vector<Nave_shot *> shots;
		Folder_Sound * wav_sounds;
};

