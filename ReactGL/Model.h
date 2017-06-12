#pragma once
#include "stdafx.h"
#include "Texture.h"

struct SFace {
	int v[3];
	int n[3];
	int t[3];
};

class Model 
	//: public CSceneObject
{
private:
	GLuint idModelu;
	CTexture *teksturaModelu;

public:

	Model();
	~Model();

	void Initialize();
	void Initialize(char *file_obj, char *file_tex);
	void Initialize(char *file_obj, char *file_tex, char *object_name, float posx, float posy, float posz, float dirx, float diry, float dirz, float speedf);
	void Update(void);
	void Render(void);

	bool tablica[4];
	void przemieszczanie();

	GLuint LoadObj(char *file);
	GLuint getID();
	CTexture * getTexture();
	
};

	

