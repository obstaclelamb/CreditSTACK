#pragma once
/* ¤è¶E*/
class CUBOID {
public:
	float vertex[8][3];
	float texcoord[4][2];
	GLubyte color[3];

	~CUBOID() {}
	CUBOID() {}
	CUBOID(float(*V)[3]);
	CUBOID(float(*V)[3], GLubyte *C);
	CUBOID(float(*V)[3], float(*T)[2], GLubyte *C);
	CUBOID(const CUBOID&C);

	virtual void SetColor(GLubyte C[]);
	virtual void SetVertex(float(*V)[3]);
	virtual void SetTexcoord(float(*T)[2]);
};

class fallingCUBOID :public CUBOID {
public:
	bool osci;
	float Rot;


	~fallingCUBOID() {}
	fallingCUBOID() {}
	fallingCUBOID(float(*V)[3],float(*T)[2], GLubyte C[],bool b) :CUBOID(V,T,C), osci(b), Rot(0.0f) {}
	fallingCUBOID(const CUBOID&C) :CUBOID(C) {}
	fallingCUBOID(const fallingCUBOID&fC);
	void reset(float(*V)[3], float(*T)[2], GLubyte C[], bool b);
	void reset(bool b);
	void SetColor(GLubyte C[]) { this->CUBOID::SetColor(C); }
	void SetVertex(float(*V)[3]) { this->CUBOID::SetVertex(V); }
	void render(float t,bool b);
	void render_skeleton(float t, bool b);
	void render_texture(float t, bool b);
};

