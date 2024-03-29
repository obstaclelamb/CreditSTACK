#include<cmath>
#include<Windows.h>
#include<gl\GLU.H>
#include<gl\GLAUX.H>
#include<gl\GLUT.H>
#include"CUBOID.h"
#include"render.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glut32.lib")


/* 建構子-頂點座標 */
CUBOID::CUBOID(float(*V)[3]) {
	this->SetVertex(V);
}

/* 建構子-頂點座標 + 顏��E*/
CUBOID::CUBOID(float(*V)[3], GLubyte *C) {
	this->SetVertex(V);
	this->SetColor(C);
}

/* 建構子-頂點座標 + 貼圖座標 + 顏��E*/
CUBOID::CUBOID(float(*V)[3], float(*T)[2], GLubyte *C) {
	this->SetVertex(V);
	this->SetTexcoord(T);
	this->SetColor(C);
}

/* 複製建構子 */
CUBOID::CUBOID(const CUBOID&C) {
	this->color[0] = C.color[0];
	this->color[1] = C.color[1];
	this->color[2] = C.color[2];
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 3; i++) {
			this->vertex[j][i] = *(*(C.vertex + j) + i);
		}
	}
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 2; i++) {
			this->texcoord[j][i] = *(*(C.texcoord + j) + i);
		}
	}
}

/* 設定顏��E*/
void CUBOID::SetColor(GLubyte C[]) {
	this->color[0] = C[0];
	this->color[1] = C[1];
	this->color[2] = C[2];
}

/* 設定座標 */
void CUBOID::SetVertex(float(*V)[3]) {
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 3; i++) {
			this->vertex[j][i] = *(*(V + j) + i);
		}
	}
}

/* 設定材質座標 */
void CUBOID::SetTexcoord(float(*T)[2]) {
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 2; i++) {
			this->texcoord[j][i] = T[j][i];
		}
	}
	
}

/* 複製建構子 */
fallingCUBOID::fallingCUBOID(const fallingCUBOID&fC) {
	this->osci = fC.osci;
	this->Rot = fC.Rot;
	this->color[0] = fC.color[0];
	this->color[1] = fC.color[1];
	this->color[2] = fC.color[2];
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 3; i++) {
			this->vertex[j][i] = *(*(fC.vertex + j) + i);
		}
	}
}
extern bool show_CubRotate;
void fallingCUBOID::render(float t,bool b) {
	if ( abs(this->vertex[0][0] - this->vertex[5][0]) <= 0.01f||abs(this->vertex[0][2] - this->vertex[3][2]) <= 0.01f ) { return; }// wx,wz小於閾值就不畫
	if (t < 100) {
		glMatrixMode(GL_MODELVIEW); 
		glPushMatrix();
		glColor3ubv(this->color);
		glTranslatef(0.0f, -0.01*t*t, 0.0f);
		float r = 2.0f * float(this->osci^b) - 1.0f;

		if (show_CubRotate) {
			if (this->osci) { glRotatef(this->Rot, 0.8f, 0.0f, r); }	// 睛岕是x方向震盪的話
			else { glRotatef(this->Rot, r, 0.0f, 0.8f); }
		}

		renderCuboidFaceAll(this->vertex);
		glPopMatrix();
		this->Rot += 2.0f;
	}
	else {
		return;
	}
}

void fallingCUBOID::render_skeleton(float t, bool b) {
	if (abs(this->vertex[0][0] - this->vertex[5][0]) <= 0.01f || abs(this->vertex[0][2] - this->vertex[3][2]) <= 0.01f) { return; }// wx,wz小於閾值就不畫
	if (t < 100) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		// glColor3ubv(this->color);
		glTranslatef(0.0f, -0.01*t*t, 0.0f);
		float r = 2.0f * float(this->osci^b) - 1.0f;
		if (this->osci) { glRotatef(this->Rot, 0.8f, 0.0f, r); }	// 睛岕是x方向震盪的話
		else { glRotatef(this->Rot, r, 0.0f, 0.8f); }
		renderCuboidEdge(this->vertex);
		glPopMatrix();
		this->Rot += 2.0f;
	}
	else {
		return;
	}
}

void fallingCUBOID::render_texture(float t, bool b) {
	if (abs(this->vertex[0][0] - this->vertex[5][0]) <= 0.01f || abs(this->vertex[0][2] - this->vertex[3][2]) <= 0.01f) { return; }// wx,wz小於閾值就不畫
	if (t < 100) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glColor3ubv(this->color);
		glTranslatef(0.0f, -0.01*t*t, 0.0f);
		float r = 2.0f * float(this->osci^b) - 1.0f;
		if (this->osci) { glRotatef(this->Rot, 0.8f, 0.0f, r); }	// 睛岕是x方向震盪的話
		else { glRotatef(this->Rot, r, 0.0f, 0.8f); }
		renderCuboidFace_No_y(this->vertex);
		renderCuboidFace_Texture_y(*this);
		glPopMatrix();
		this->Rot += 2.0f;
	}
	else {
		return;
	}
}

void fallingCUBOID::reset(float(*V)[3], float(*T)[2], GLubyte C[], bool b) {
	this->SetVertex(V);
	this->SetTexcoord(T);
	this->SetColor(C);
	this->osci = b;
	this->Rot = 0;
}
void fallingCUBOID::reset(bool b) {
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 3; i++) {
			this->vertex[j][i] = 0;
		}
	}
}