#include "stdafx.h"
#include "Model.h"
#include <vector>
#include "Vec3.h"

Model::Model()
{
}


Model::~Model()
{
}

void Model::Initialize()
{ 
}

void Model::Initialize(char *file_obj, char *file_tex)
{
	
	teksturaModelu = new CTexture(file_tex, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	teksturaModelu->Load();

	idModelu = LoadObj(file_obj);

	for (int i = 0; i < 4; i++)
		tablica[i] = false;
}

void Model::Update(void)
{
	//void przemieszczenie();
}

void Model::Render(void)
{
	glPushMatrix();

	glTranslatef(0, 2,-3);
	glRotatef(0, 1.0f, 0.0f, 0.0f);
	glRotatef(0, 0.0f, 1.0f, 0.0f);
	glRotatef(0, 0.0f, 0.0f, 1.0f);
	glScalef(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, teksturaModelu->GetId());
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glCallList(idModelu);

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Model::przemieszczanie()
{
	//if ((tablica[0] == true) && (tablica[1] != true))
	//	velM = speed;
	//if ((tablica[1] == true) && (tablica[0] != true))
	//	velM = -speed;
	//if ((tablica[2] == true) && (tablica[3] != true))
	//	velS = speed;
	//if ((tablica[3] == true) && (tablica[2] != true))
	//	velS = -speed;

	if ((rand() % 100 + 1) % 3 == 0) tablica[0] = true; else tablica[0] = false;
	if ((rand() % 100 + 1) % 5 == 0) tablica[1] = true; else tablica[1] = false;
	if ((rand() % 100 + 1) % 2 == 0) tablica[2] = true; else tablica[2] = false;
	if ((rand() % 100 + 1) % 10 == 0) tablica[3] = true; else tablica[3] = false;
}


GLuint Model::LoadObj(char * file) {

	FILE * fp = fopen(file, "r");

	if (fp == NULL) {
		printf("ERROR: Cannot read model file \"%s\".\n", file);
		return -1;
	}
	std::cout << "Wszystko git z modelem obj" << std::endl;
	std::vector<vec3> * v = new std::vector<vec3>();
	std::vector<vec3> * n = new std::vector<vec3>();
	std::vector<vec3> * t = new std::vector<vec3>();
	std::vector<SFace> * f = new std::vector<SFace>();

	char buf[128];

	while (fgets(buf, 128, fp) != NULL) {
		if (buf[0] == 'v' && buf[1] == ' ') {
			vec3 * vertex = new vec3();
			sscanf(buf, "v %f %f %f", &vertex->x, &vertex->y, &vertex->z);
			v->push_back(*vertex);
		}
		if (buf[0] == 'v' && buf[1] == 't') {
			vec3 * vertex = new vec3();
			sscanf(buf, "vt %f %f", &vertex->x, &vertex->y);
			t->push_back(*vertex);
		}
		if (buf[0] == 'v' && buf[1] == 'n') {
			vec3 * vertex = new vec3();
			sscanf(buf, "vn %f %f %f", &vertex->x, &vertex->y, &vertex->z);
			n->push_back(*vertex);
		}
		if (buf[0] == 'f' && buf[1] == ' ') {
			SFace * face = new SFace();
			sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&face->v[0], &face->t[0], &face->n[0],
				&face->v[1], &face->t[1], &face->n[1],
				&face->v[2], &face->t[2], &face->n[2]
			);
			f->push_back(*face);
		}
	}

	fclose(fp);

	GLuint dlId;
	dlId = glGenLists(1);
	glNewList(dlId, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < f->size(); ++i) {
		for (int j = 0; j < 3; ++j) {
			vec3 * cv = &(*v)[((*f)[i].v[j] - 1)];
			vec3 * ct = &(*t)[((*f)[i].t[j] - 1)];
			vec3 * cn = &(*n)[((*f)[i].n[j] - 1)];
			glTexCoord2f(ct->x, ct->y);
			glNormal3f(cn->x, cn->y, cn->z);
			glVertex3f(cv->x, cv->y, cv->z);
		}
	}
	glEnd();
	glEndList();

	delete v;
	delete n;
	delete t;
	delete f;

	return dlId;

}


GLuint Model::getID() {
	return idModelu;
}

CTexture * Model::getTexture()
{
	return teksturaModelu;
}