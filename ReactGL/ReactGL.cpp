// ReactGl.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"
#include "Model.h"
#include "Events.h"

int garbage;

Game *game;
Camera cam;
Player *player;

//MOUSE
int mx, my;
//Keyboard
bool keystate[256];
//Screen
int screenHeight = 600, screenWidth = 800;

void DrawString(GLfloat x, GLfloat y, char * string)
{
	// po³o¿enie napisu
	glRasterPos2f(x, y);

	// wyœwietlenie napisu
	int len = strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);

}

void Display()
{
	glClearColor(0.878, 0.960, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	Player *player = game->getplayer();

	player->cam.set();
	player->Draw();
	float matrix[16];


	game->Draw();
	////PLAYER
	//glPushMatrix();
	//player->Draw(matrix);
	//glMultMatrixf(matrix);
	//glScalef(0.25, 1, 0.25);
	//glColor3f(0, 0, 0.5);
	////glutSolidCube(1);
	//glColor3f(1, 1, 1);
	//glutWireCube(1);
	//glPopMatrix();
	//player->Draw();

	//for (auto *i : game->effects)
	//{
	//	i->Draw();
	//}

	for (auto *i : game->map)
	{
		i->Draw();
	}

	//for (auto *i : game->objs)
	//{
	//	i->Draw();
	//}

	#pragma region 2D
	{
		glDisable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, 800, 600, 0.0, -1.0, 10.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_CULL_FACE);

		glClear(GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//DRAW 2d HERE
		player->Drawsight(screenWidth / 2, screenHeight / 2);
		player->DrawShootPower(screenWidth / 2, screenHeight);

		glColor3b(0, 0, 0);
		DrawString(20, 15, "O======");
		//
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		//glEnable(GL_LIGHTING);
	}

	glFlush();			// skierowanie poleceñ do wykonania
	glutSwapBuffers();	// zamiana buforów koloru

						//game.Update();
}

void Reshape(int width, int height)
{
	// obszar renderingu - ca³e okno
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// obliczenie aspektu obrazu z uwzglêdnieniem
	// przypadku, gdy wysokoœæ obrazu wynosi 0
	GLdouble aspect = 1;
	if (height > 0)
		aspect = width / (GLdouble)height;

	// rzutowanie perspektywiczne
	gluPerspective(90, aspect, 0.1, 60.0);

	Display();
}

void OnTimer(int id)
{
	glutTimerFunc(17, OnTimer, 0);

	Player *player = game->getplayer();

	if (keystate['u'])
		player->cam.rotate(UP, 2.5);
	if (keystate['o'])
		player->cam.rotate(DOWN, 2.5);
	if (keystate['w'])
		player->set_control(0);
	if (keystate['s'])
		player->set_control(1);
	if (keystate['a'])
		player->set_control(2);
	if (keystate['d'])
		player->set_control(3);

	if (keystate[' '])
		player->set_control(4);

	if (keystate[0])			//LPP
		player->set_control(5);
}

void OnKeyDown(unsigned char key, int x, int y)
{
	if (key == 27)
	{ // ESC - wyjœcie
		glutLeaveMainLoop();
	}
	if (key == 'x')
		game->plus();
	if (key == 'z')
		game->testshoot();
	if (key == 'c')
	{
		;
	}
	if (key == 'q')
		game->Update();
}

void OnKeyPress(unsigned char key, int x, int y) {
	//printf("KeyPress: %c\n", key);
	if (!keystate[key]) {
		OnKeyDown(key, x, y); // Emulacja zdarzenia zwi¹zanego z pojedynczym wciœniêciem klawisza
	}
	keystate[key] = true;
}


// Obs³uga zdarzenia puszczenia klawisza.
void OnKeyUp(unsigned char key, int x, int y)
{
	Player *player = game->getplayer();

	if (key == 'w')
		player->unset_control(0);
	if (key == 's')
		player->unset_control(1);
	if (key == 'a')
		player->unset_control(2);
	if (key == 'd')
		player->unset_control(3);
	if (key == ' ')
		player->unset_control(4);

	keystate[key] = false;
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		//exit(0);
		break;
	}

	Display();
}


void Idle()
{
	game->Update();
	//player->update();
	glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y)
{
	Player *player = game->getplayer();
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
			keystate[0] = true;
		else
		{
			player->unset_control(5);
			keystate[0] = false;
			game->testshoot();
		}
	}
	

}

// obs³uga ruchu kursora myszki
void MousePassiveMotion(int x, int y)
{
	Player *player = game->getplayer();

	//CAMERA ROTATION
	if (x > screenWidth-200 || x < 200 || y > screenHeight -150 || y < 150)
	{
		glutWarpPointer(400, 300);
		mx = screenWidth/2;
		my = screenHeight/2;
		return;
	}

	if (mx - x < 0)
		player->rotate2(RIGHT, (float)(mx - x) / -100);
	if (mx - x > 0)
		player->rotate2(LEFT, (float)(mx - x) / 100);
	if (my - y > 0)
		player->cam.rotate(UP, my - y);
	if (my - y < 0)
		player->cam.rotate(DOWN, my - y);
	mx = x; my = y;
}

void MouseMotion(int x, int y)
{
	MousePassiveMotion(x, y);
	glutPostRedisplay();
}

void EntryFunc(int state)
{
	;
}

int main(int argc, char * argv[])
{
/*
	rp3d::Vector3 vec(0, -10, 0);
	rp3d::DynamicsWorld *world = new rp3d::DynamicsWorld(vec);

	rp3d::RigidBody *b1 = NULL;
	rp3d::ProxyShape *p1 = NULL;
	rp3d::CollisionShape *s1 = NULL;

	vec.setAllValues(0, 0, 0);
	rp3d::Quaternion q(0, 0, 0);
	rp3d::Transform t;
	b1 = world->createRigidBody(t);

	rp3d::decimal r = rp3d::decimal(0.5);
	s1 = new rp3d::SphereShape(r);
	t = rp3d::Transform::identity();
	rp3d::decimal mass = rp3d::decimal(4.0);
	p1 = b1->addCollisionShape(s1, t, mass);

	rp3d::RigidBody *b2 = NULL;
	rp3d::ProxyShape *p2 = NULL;
	rp3d::CollisionBody *s2 = NULL;

	vec.setAllValues(0, 0, 0);
	t.setPosition(vec);
	b2 = world->createRigidBody(t);
	b2->setType(rp3d::STATIC);
	p2 = b2->addCollisionShape(s1, t, mass);

	////delete p1;
	//b1->removeCollisionShape(p1);
	////delete b1;
	//world->destroyRigidBody(b1);
	
	world->update(0.1);

	delete world;

	delete s1;
*/


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(200, 5);
	glutCreateWindow("LOL");
	glutSetCursor(GLUT_CURSOR_NONE);

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	//KEYBOARD
	glutKeyboardFunc(OnKeyPress);
	glutKeyboardUpFunc(OnKeyUp);
	//MOUSE
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutEntryFunc(EntryFunc);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_DEFAULT);
	//TIMER
	glutTimerFunc(17, OnTimer, 0);
	glutIdleFunc(Idle);

	//FOG
	glEnable(GL_FOG);
	float gl_fogcolor[] = { 0.875f, 0.957f, 1.0f, 1.0f };
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, gl_fogcolor);
	glFogf(GL_FOG_START, 6.0f);
	glFogf(GL_FOG_END, 24.0f);

	//ANTYALIASING
	//glEnable(GLUT_MULTISAMPLE);
	
	//LIGHT
	glEnable(GL_LIGHTING);
	float gl_amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gl_amb);

	float l0_amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float l0_dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float l0_spe[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float l0_pos[] = { -1.0f, 2, 0.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0_dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0_spe);
	glLightfv(GL_LIGHT0, GL_POSITION, l0_pos);


	game = new Game();

	glutMainLoop();
	
	printf("lol");

	delete game;

	return 0;
}
