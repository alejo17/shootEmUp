class Enemy
{
	public:

		Enemy(float x, float y, float z)
		{
			this->x = x; 
			this->y = y; 
			this->z = z;
			time(&last_shot);
			time_lapse = 2;

		}

		void shoot(float x, float y, float z)
		{
			for (int i = 0; i < shots.size(); ++i)
			{
				shots[i]->display();
				bool hited = shots[i]->hit(x,y,z-3);
				
				if(hited)
				{
					shots[i]->activate=false;
					break;
				}
			}
		}

		void load_shot()
		{
			if(is_ready())
			{
				//cout<<"Tower is shooting!"<<endl;
				Enemy_shot * shot = new Enemy_shot(x,y,z);
				shot->set_velocity(vx,vy,vz);
				shots.push_back(shot);
				time(&last_shot);
			}
		}

		bool is_ready()
		{
			time_t now;
			time(&now);
			float diff = now - last_shot;
			// cout<<diff<<endl;
			if(diff >= time_lapse)
				return true;
			else
				return false;
		}

		bool is_near(float x, float y, float z)
		{
			if(this->z < z) return false; // no dispara hacia atras

			float dx,dy,dz;
			dx = x - this->x;
			dy = y - this->y;
			dz = z - this->z;

			float dist = sqrt(dx*dx+dy*dy+dz*dz);

			if(dist < DIST_ENEMY)
				return true;
			else
				return false;
		}

		void calculate_direction(float x, float y, float z)
		{
			vx = x-this->x;
			vy = y-this->y;
			vz = z-this->z;

			float n = sqrt(vx*vx+vy*vy+vz*vz+0.00001);
			
			vx = vx/n*SPEED_ENEMY_SHOT;
			vy = vy/n*SPEED_ENEMY_SHOT;
			vz = vz/n*SPEED_ENEMY_SHOT;

			// cout<<"velocity: "<<vx<<" "<<vy<<" "<<vz<<endl;
		}

		float x, y,z;
		float vx,vy,vz;
		int time_lapse;
		time_t last_shot;
		vector<Enemy_shot *> shots;	
};
