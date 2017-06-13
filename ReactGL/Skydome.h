#pragma once

// Skydome, czyli oteksturowana p�kula imituj�ca niebo.
class CSkydome
{
public:
	rp3d::Vector3 Position;
	CSkydome(void);
	~CSkydome(void);
	void Initialize(void);
	void Update(void);
	void Render(void);

protected:
	GLuint _displayListId;
	CTexture * _skyTexture;
};

