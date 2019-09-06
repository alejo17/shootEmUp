#include <GL/glut.h>

#define SHOT_SIZE 0.6
#define SPEED_SHOT 4
#define SPEED_ENEMY_SHOT 2.0
#define RADIUS_OBJETIVE 10

extern float life;
extern bool shield;

class Shot
{
	public:
		Shot(float actual_x, float actual_y, float actual_z)
		{
			x = actual_x; 
			y = actual_y; 
			z = actual_z+30;
		}

		void set_position(float x , float y , float z)
		{
			this->x = x;
			this->y = y; 
			this->z = z;
		}

		void move(float vx=0,float vy=0, float vz=0)
		{
			x += vx; 
			y += vy; 
			z += vz;
		}

		~Shot()
		{
			printf("termino el disparo\n");
		}

		float x,y,z;
};

class Nave_shot: Shot
{
	public:
		Nave_shot(float x, float y, float z):Shot(x,y,z){;}

		void display()
		{
			glPushMatrix();
				glTranslatef(-x*2,-y*2,-z);
				glutSolidSphere(SHOT_SIZE, 10,10);
			glPopMatrix();

			z+=SPEED_SHOT;
		}
};

class Enemy_shot: Shot
{
	public:
		Enemy_shot(float x, float y, float z):Shot(x,y,z)
		{
			activate = true;
		}

		void display()
		{
			if(activate == false)
			{
				x = 200;	
			}
			
			glPushMatrix();
				glTranslatef(-x,-y,-z);
				glutSolidSphere(SHOT_SIZE, 10,10);
			glPopMatrix();

			x+=vx; y+=vy; z+=vz;
		}

		bool hit(float x, float y, float z)
		{
			float dx,dy,dz;
			dx = x-this->x;
			dy = y-this->y;
			dz = z-this->z;

			float dist = sqrt(dx*dx+dy*dy+dz*dz);
			
			if(shield)
			{
				if(dist < RADIUS_OBJETIVE+10)
				{
					return true;
				}
				return false;
			}

			else
			{
				if(dist < RADIUS_OBJETIVE)
				{
					life-=0.5;
					//cout<<"Tower hit me!!"<<endl;
					return true;
				}
				return false;
			}
		}

		void set_velocity(float vx, float vy, float vz)
		{
			this->vx = vx; 
			this->vy = vy; 
			this->vz = vz;
		}

		bool activate;
		float vx,vy,vz;
};
