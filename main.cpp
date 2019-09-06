#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "ship.h"
#include "hud.h"
#include "map.h"
#include "enemy.h"

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27
#define SHOOT 32
#define SHIELD 115

#define MOVE 0.8
#define SPEED 1.5

GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; //set the light specular to white
GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; //set the light ambient to black
GLfloat whiteDiffuseLight[] = {0.3f, 0.3f, 0.3f , 1.0f}; //set the diffuse light to white
GLfloat whiteDiffuseLight2[] = {0.3f, 0.3f, 0.3f , 1.0f}; //set the diffuse light to white
GLfloat lightpos2[] = {-1, -1, -1, -1};//set light position

GLfloat yellowLight[] = {0.6f , 0.6f , 0.6f , 0.4};

//variables para el gizmo
float delta_x = 0.0; 
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = 0.0;
float var_y = 0.0;
float var_z = -30.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)

Mapa* map_1;
Ship * nave; 
Hud * hud;
vector<Enemy *> enemies;
sf::SoundBuffer buffer;
MyStream stream;

float life = 100;
float energy = 100;
bool shield;

GLvoid initGL()
{
    glClearColor(RED, GREEN, BLUE, ALPHA);
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat light_diffuse[] = { 0.25, 0.25, 0.25, 1.0 };
    GLfloat light_specular[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat lightpos[] = { -20.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    // glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_ambient1[] = {0.1 , 0.1 , 0.1 , 1.0};
    GLfloat light_diffuse1[] = { 0.6, 0.6 , 0.0 , 1.0};
    GLfloat light_specular1[] = {1.0 , 1.0 , 0.0 , 0.9};
    GLfloat lightpos1[] = {0 , 0 , 4 , 0};
    GLfloat spot_direction1[] = {-1.0 , -1.0 , 0.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    // glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);
    // glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
    // glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,30.0f);

    glEnable(GL_LIGHT1);
}

GLfloat lightpos1[] = {0 , 0, 10 , 0};
GLfloat lightpos[] = { 0, 10.0, 0.0, 0 };

double delta = 0.1;
double deltaZ = 0.1;

void draw_hud()
{
	hud->exe();
}

void draw_battleCruiser()
{
	glPushMatrix();
		glTranslatef(var_x, -var_y, 30);
		nave->display();

		if(shield){
			// glDisable(GL_BLEND);
			// glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			energy-=0.5;
			glTranslatef(-var_x/2,var_y/2,-40);
			glColor4ub(0,255,0,64);
			glRotatef(var_z*150,10,30,20);
			glutWireSphere(1.75, 30,40);

		}

	glPopMatrix();
}

void save_ship_position()
{
	int tx, ty, tz;
	nave->get_position_matrix(tx,ty,tz);
	// cout<<tx<<" "<<ty<<" "<<tz<<endl;
	map_1->update_ship(tx,ty,tz);
}

//function called on each frame
GLvoid window_idle()
{
	glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////
// 			Keyboard Interaction
///////////////////////////////////////////////////////////////////////////////
GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) 
	{
		case ECHAP:
			exit(1);
			break;
		case SHOOT:
			nave->new_normal_shot();
			break;
		case SHIELD:
			shield = true;
			break;
		default:
			printf("La touche %d non active.\n", key);
			break;
	}
}

GLvoid window_key_up(unsigned char key, int x, int y){
	switch (key) 
	{
		case ECHAP:
			exit(1);
			break;
		case SHIELD:
			shield = false;
		break;
	}
}

GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			var_y += MOVE;
			glutPostRedisplay();
			break;

		case GLUT_KEY_DOWN:
			var_y -= MOVE;
			glutPostRedisplay();
			break;

		case GLUT_KEY_LEFT:
			var_x += MOVE;
			glutPostRedisplay();
			break;

		case GLUT_KEY_RIGHT:
			var_x -= MOVE;
			glutPostRedisplay();
			break;

		case GLUT_KEY_PAGE_UP:
			step++;
			glutPostRedisplay();
			break;
		case GLUT_KEY_PAGE_DOWN:
			step--;
			glutPostRedisplay();
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
// 				Mouse Functions
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse_x = x;
		mouse_y = y;
	}
}

GLvoid callback_motion(int x, int y)
{
	delta_x += x - mouse_x;
	// delta_y += y - mouse_y;
	delta_y -= 0.1;
	// var_x += -delta_x/1000;
	var_y += -delta_y/1000 ;
	glutPostRedisplay();
}

GLvoid window_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(-45.0f, 1.0f, 0.01f, 100.0f);

	save_ship_position();
	draw_battleCruiser();

	glPushMatrix();
		nave->change_position(-var_x,var_y,var_z);

		for(int i = 0 ; i < enemies.size() ; ++i)
		{
			if( enemies[i] -> is_near(nave->x,nave->y,nave->z))
			{
				enemies[i] -> calculate_direction(nave->x,nave->y,nave->z);
				enemies[i] -> load_shot();
			}
		}

		glTranslatef(-var_x, var_y, var_z);
		map_1->draw_map(var_z);
		nave->display_normal_shots();

		for (int i = 0; i < enemies.size(); ++i)
		{
			enemies[i] -> shoot(nave->x, nave->y, nave->z);
		}
		// printf("x; y ; z: %f, %f, %f\n", nave->x, nave->y, nave->z);
	glPopMatrix();

	// nave->show_position();
	draw_hud();

	glEnable(GL_LIGHTING);
	glEnable( GL_DEPTH_TEST ) ;
	glutSwapBuffers();
	glFlush();

	//cout<<var_z<<endl;
	var_z += SPEED;
	map_1->restart_map(var_z);
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
	string soundlist = "02 Title Theme.ogg";
	buffer.loadFromFile(soundlist);

	stream.load(buffer);
	stream.play();

	shield = false;

	enemies.push_back(new Enemy(0,0,150));
	enemies.push_back(new Enemy(7,0,170));
	enemies.push_back(new Enemy(-7,0,180));
	enemies.push_back(new Enemy(-8,0,190));
	enemies.push_back(new Enemy(8,0,300));
	enemies.push_back(new Enemy(0,0,310));
	enemies.push_back(new Enemy(7,0,320));
	enemies.push_back(new Enemy(-7,0,430));
	enemies.push_back(new Enemy(0,0,440));
	enemies.push_back(new Enemy(8,0,480));

	map_1 = new Mapa("map");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(30, 20);
	glutCreateWindow("Shoot Em Up"); 

	initGL();

	hud = new Hud("hudfin.png", "mariofin.png", "nave.png");
	nave = new Ship ("001battlecrusier.obj");
	nave->change_position(var_x,var_y,var_z);

	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);
	glutKeyboardFunc(&window_key);
	glutKeyboardUpFunc(&window_key_up); //key release events
	glutSpecialFunc(&callback_special);
	glutIdleFunc(&window_idle);
	glutMainLoop();

	return 1;
}