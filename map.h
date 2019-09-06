#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
using namespace std;

#define BIT_SIZE 2
extern float life;

class Mapa
{
	public:
		Mapa(string map_file)
		{
			this->map_file = map_file;
			load_map();
			width = vector_map[0][0].size();
			height = vector_map[0].size();
			//show_map();
		};

		~Mapa(){}

		void draw_cube(int z , int y , int x)
		{
			glPushMatrix();
				glTranslatef(width/BIT_SIZE, -height/BIT_SIZE,0);
				glTranslatef(x*BIT_SIZE,-y*BIT_SIZE,-z);
				glutSolidCube(BIT_SIZE-0.5);
			glPopMatrix();
		}

		void draw_enemy(int z, int y, int x)
		{
			glPushMatrix();
				glTranslatef(width/BIT_SIZE, -height/BIT_SIZE,0);
				glTranslatef(x*BIT_SIZE,-y*BIT_SIZE,-z);
				glutSolidCube(BIT_SIZE);
			glPopMatrix();
		}

		void load_map()
		{
			ifstream fstream_map_file (map_file.c_str());
			string line;
			vector <string> vec;

			while(!fstream_map_file.eof())
			{
				getline(fstream_map_file,line);
				if(line == "2")
				{
					vector_map.push_back(vec);
					vec.clear();
				}
				else
				{
					vec.push_back(line);
				}
			}
			fstream_map_file.close();
		}

		void show_map()
		{
			cout<<"show_map"<<endl;
			for (int i = 0; i < vector_map.size(); ++i)
			{
				for (int j = 0; j < vector_map[i].size(); ++j)
				{
					cout<<vector_map[i][j]<<endl;
				}
			}
		}

		void draw_map(float & var_z)
		{
			int mid = 4;
			glColor3f(0,1,0);
			for(int i = 0 ; i < vector_map.size() ; ++i)
			{
				if(i <= var_z + 100 and i > var_z)
				{
					for(int j = 0 ; j < vector_map[i].size() ; ++j)
					{
						for (int k = 0; k < vector_map[i][j].size(); ++k)
						{
							if(vector_map[i][j][k] == '1')
							{
								draw_cube(i,mid-j,mid-k);
							}
							if(vector_map[i][j][k] == '3')
							{
								draw_enemy(i,mid-j,mid-k);
							}
						}
					}
				}
			}

			int restante = (int)(vector_map.size() - var_z - 100);
			//cout<<restante<<endl;
			for(int i = 0 ; i < 100-restante ; i++)
			{
				if(var_z+100 > vector_map.size()+i)
				{
					for(int j = 0 ; j < vector_map[i].size() ; ++j)
					{
						for (int k = 0; k < vector_map[i][j].size(); ++k)
						{
							if(vector_map[i][j][k] == '1')
							{
								draw_cube(vector_map.size()+i,mid-j,mid-k);
							}
						}
					}
				}
			}
		}

		void restart_map(float &var_z)
		{
			if(var_z+30 >= vector_map.size())
			{
				//cout<<"REINICIO"<<endl;
				var_z = -30;
			}
		}

		bool valid (float x, float y, float z)
		{
			if(vector_map[floor(z)][floor(y)][floor(x)] == '1') return 0;
			return 1;
		}

		int modulo(int D, int d)
		{
			if (d >= 0)
				return d;
			if (d < 0)
				return D+d;
		}

		void update_ship(int x, int y, int z)
		{
			int nz = z+30;
			int ny = vector_map[nz].size() - 1 - y;
			int total_v = vector_map.size();
			int bz;
			
			for(int i = 0 ; i < 12 ; i++)
			{
				bz = modulo(total_v,nz-i);   //back Z
				// cout<<bz<<"  --"<<endl;
				if((vector_map[bz][ny][x] == '0' or vector_map[bz][ny][x] == 'N') and
					(vector_map[bz][ny][x+1] == '0' or vector_map[bz][ny][x+1] == 'N') and
					(vector_map[bz][ny][x-1] == '0' or vector_map[bz][ny][x-1] == 'N') )
				{
					// cout<<"ok"<<endl;
					vector_map[bz][ny][x] = vector_map[bz][ny][x-1] = vector_map[bz][ny][x+1] = 'N';
				}
				else
				{
					// cout<<"map:updt "<<x<<" "<<ny<<" "<<nz<<endl;
					// cout<<vector_map[nz][ny]<<endl;
					//cout<<"La nave se choco!"<<endl;
					life-=0.2;
					// exit(1);
				}
			}

			//show_current_z(nz);
			
			// Clean
			for(int i = 0 ; i < vector_map[nz].size(); i++)
			{
				for(int j = 0 ; j < vector_map[nz][i].size() ; ++j)
				{
					if(vector_map[nz][i][j] == 'N') vector_map[nz][i][j] = '0';
				}
			}

		}

		void show_current_z(int&nz)
		{
			for(int i = 0 ; i < vector_map[nz].size(); i++)
			{
				for (int j = 0; j < vector_map[nz][i].size(); ++j)
				{
					cout<<vector_map[nz][i][j];
				}
				cout<<endl;
			}
			cout<<endl;
		}

		int width, height;

	private:
		string map_file;
		vector<vector <string> >vector_map;
};