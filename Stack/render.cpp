// E14092013���㴭
#include<Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <ctime>
#include <gl\GLU.H>
#include <gl\GLAUX.H>
#include <gl\GLUT.H>
#include "render.h"
#include "font.h"
#include "bitmap.h"
#include "texture.h"
#include "buttombitmap.h"
#include "CUBOID.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glut32.lib")
bool show_Falling = SHOW_EVERYTHING;
bool show_CubRotate = SHOW_EVERYTHING;
bool show_SlowDown = SHOW_EVERYTHING;
bool show_Buttom = SHOW_EVERYTHING;
bool show_2OrderSys = SHOW_EVERYTHING;

using std::cout;	// ���ե�
using std::cin;		// ���ե�
using std::string;	// �ù��e�r��

// ����ø�s��
fallingCUBOID fallcub;	// ���������(�ثe�u�g�@��)
std::vector<CUBOID> cubList;			// �|�n������}�C
float wx = STACK_CUBOID_INITIAL_WIDTH;	// �{�b�����
float wz = STACK_CUBOID_INITIAL_WIDTH;	// �{�b����e
float center[3] = { 0,0,0 };			// �{�b���������
float center_last[3] = { 0,0,0 };		// �e�@�Ӥ��������
float face_vertex[8][3] = { 0 };		// �b�ʪ�������y��
float face_normal[6][3] = {				// �k�V�q
	{ 1,0,0 },/* +x */
	{ 0,1,0 },/* +y */
	{ 0,0,1 },/* +z */
	{ -1,0,0 },/* -x */
	{ 0,-1,0 },/* -y */
	{ 0,0,-1 } /* -z */ };
float face_texture[4][2] = { 0 };			// �K�Ϯy��
float face_texture_falling[4][2] = { 0 };	// ���U�h���K�Ϯy��
const int hy = 20;						// ������שw��
const int osci_length = 180;			// �_���Z���w��
bool osci_direct = Z_OSCILLATION;		// �_����V T:x  F:z
float t = 0;		// �_���ήɶ�
float dt = 0;		// �ɶ��ܤƶq
float ddt = 0.01f;	// (�ɶ��ܤƶq)�ܤƶq:���״��ɥ�

GLuint Texture;						// �������
GLubyte CuboidColor[] = { 0,0,0 };	// ����C��
int CuboidColor_H;		// 0 <= H <= 360
int CuboidColor_S;		// 0 <= S <= 100
int CuboidColor_V;		// 0 <= V <= 100

float center_fall[3] = { 0,0,0 };		// �b�U�������������
float face_vertex_fall[8][3] = { 0 };	// �b�U����������y��
bool b;									// ������������O����

// �r�鵥�]�w
const char FontName[] = "New Times Roman";	// �ϥΦr��W
GLubyte FontColorLight[] = { 255,255,255 };	// �r���C��-�L��
GLubyte FontColorDark[] = { 10,10,10 };		// �r���C��-�`��

// �����]����
bool w_bigger_than_h;					// �������μe�H
GLfloat ratio;							// �������e��
int window_width, window_height;		// �������e
GLFONT * Font_big;						// �j���r
GLFONT * Font_small;					// �p���r
GLfloat bitmapcolor;					// ���s�ϼ��C��
GLfloat ClearColorLight[] = { 0.7f,0.7f,0.7f };	//�M����-�L��
GLfloat ClearColorDark[] = { 0.1f,0.1f,0.1f };	//�M����-�`��
GLfloat(*ClearColorNow)[3];					//�{�b�M����

// �C���ާ@��
int password = 0;						// �q���K�X/���դH��(?)�O�_�}��
int scene = STACK_START;				// �{�b����
int skin = STACK_TEXTURE_PURE_COLOR;	// ���ø�s�Ҧ�
bool random_skin = FALSE;				// �H��ø�s�Ҧ�

// �C��������
int max_score = 0;	// �̤j����
int credit = 0;		// �Ǥ�(x) ����K�ϲֿn(o)
std::vector<int> records;			// �U�ج���
std::vector<int> texture_unlocked;	// ���ꪺ�K�϶���

/* ���F�W�U���k�� */
void SpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_UP) {
		// start --> page1 --> page2 --> start
		if (scene == STACK_START || scene == STACK_GAME_OVER) {
			scene = STACK_TEXTURE_SELECTION_PAGE_1;
		}
		else if (scene == STACK_TEXTURE_SELECTION_PAGE_1) {
			scene = STACK_TEXTURE_SELECTION_PAGE_2;
		}
		else if (scene == STACK_TEXTURE_SELECTION_PAGE_2) {
			scene = STACK_START;
		}
	}
	else if (key == GLUT_KEY_LEFT || key == GLUT_KEY_DOWN) {
		// start --> page2 --> page1 --> start
		if (scene == STACK_START || scene == STACK_GAME_OVER) {
			scene = STACK_TEXTURE_SELECTION_PAGE_2;
		}
		else if (scene == STACK_TEXTURE_SELECTION_PAGE_1) {
			scene = STACK_START;
		}
		else if (scene == STACK_TEXTURE_SELECTION_PAGE_2) {
			scene = STACK_TEXTURE_SELECTION_PAGE_1;
		}
	}
	glutPostRedisplay();				// ���s��z����:)
}

/* ���F�r�������� */
void NormalKeys(unsigned char key, int x, int y) {
	switch (key) {
	case ' ':SpaceBar(); break;
	case KEY_EXIT:save_record(); exit(0); break;
	}
	if (password == 4) {// �}�ҥ��q�Ҧ�
		switch (key) {
		case KEY_SAVE_RECORD:		save_record(); break;
		case KEY_LOAD_RECORD:		load_record(); break;
		case KEY_COLOR_NEXTSTEP:	ColorCircle(); break;
		case KEY_COLOR_RESTART:		cubList.clear(); SetupParameter(); break;
		case KEY_TEXTURE_PURE:		skin = STACK_TEXTURE_PURE_COLOR; break;
		case KEY_TEXTURE_SKELE:		skin = STACK_TEXTURE_SKELETON; break;
		case KEY_TEXTURE_RANDOM:	random_skin = !random_skin; break;
		case KEY_SCENE_START:		scene = STACK_START; break;
		case KEY_SCENE_PLAY:		scene = STACK_PLAY; break;
		case KEY__SCENE_OVER:		scene = STACK_GAME_OVER; break;
		case KEY_SELECT_1:			scene = STACK_TEXTURE_SELECTION_PAGE_1; break;
		case KEY_SELECT_2:			scene = STACK_TEXTURE_SELECTION_PAGE_2; break;
		case KEY_SHOW_ALL:			show_2OrderSys = show_Buttom = show_CubRotate = show_Falling = show_SlowDown = TRUE; break;
		case KEY_SHOW_FALLING:		show_Falling = !show_Falling; testing_index(); break;
		case KEY_SHOW_fall_ROTATE:	show_CubRotate = !show_CubRotate; testing_index(); break;
		case KEY_SHOW_SLOW_FALL:	show_SlowDown = !show_SlowDown; testing_index(); break;
		case KEY_SHOW_BUTTOM:		show_Buttom = !show_Buttom; testing_index(); break;
		case KEY_SHOW_BOUNCING:		show_2OrderSys = !show_2OrderSys; testing_index(); break;
		case KEY_TEXTURE_NCKU:		skin = STACK_TEXTURE_NCKU; break;
		case KEY_TEXTURE_NCKUME:	skin = STACK_TEXTURE_NCKUME; break;
		case KEY_TEXTURE_NCKUMEVR:	skin = STACK_TEXTURE_NCKUMEVR; break;
		}
	}
	else {// �}�ҹL�N���P�_�F
		switch (password)// password = ncku
		{
		case 0:if (key == 'n' || key == 'N') { password++; break; }
		case 1:if (key == 'c' || key == 'C') { password++; break; }
		case 2:if (key == 'k' || key == 'K') { password++; break; }
		case 3:
			if (key == 'u' || key == 'U') {
				password++;
				max_score = INT_MAX;
				credit = 144; // �n���x���Ʀr��...���D�O...���~�Ǥ�...
				calculate_record();
				cout << "Onlocked\n";
			}
			break;
		case 4:break;
		default:password = 0; // ���m
		}
	}
	if (scene != STACK_GAME_OVER) { // �o�O���F�קK�b�qgameover�����start�ɷ|�h�X�@��CubList���I�s
		glutPostRedisplay();				// ���s��z����:)
	}
}

/* �C���ɪ��ť���ηƹ��ƥ� */
void SpaceBar() {
	// scene loop: START -> PLAY -> GAMEOVER -> START
	switch (scene)
	{
	case STACK_START:
		scene = STACK_PLAY;
		dt = 5;
		break;
	case STACK_GAME_OVER:
		cubList.clear();
		SetupParameter();
		break;
	case STACK_TEXTURE_SELECTION_PAGE_1:
		scene = STACK_TEXTURE_SELECTION_PAGE_2;
		break;
	case STACK_TEXTURE_SELECTION_PAGE_2:
		scene = STACK_START;
		break;
	case STACK_PLAY:
		if (abs(center[0] - center_last[0]) > 2 * wx || abs(center[2] - center_last[2]) > 2 * wz) {
			// �鱼�F
			if (max_score < cubList.size()) { 
				max_score = cubList.size() - 1; 
				credit += (cubList.size() / 10); }
			scene = STACK_GAME_OVER;
			fallcub.reset(0);// ���U������h��
			stack_gameover_scene(RESET_TIME);	// ���]�ʵe�ɶ�
			stack_initial_scene(RESET_TIME);	// ���]�ʵe�ɶ�
		}
		else {
			// �٨S��
			CutTheCuboid();
			if (cubList.size() % 3 == 0) { dt += ddt; } // ����up
		}
		break;
	default:
		break;
	}
}


/* �B�z�����-�|�۪��P���U�� */
void CutTheCuboid() {
	center_fall[0] = center[0]; center_fall[1] = center[1]; center_fall[2] = center[2];
	float wx2_ = wx; float wz2_ = wz;
	if (osci_direct) { // �p�G�O x ��V�Ӧ^
		float center_temp_x = (center_last[0] + center[0]) / 2;		// C2
		wx2_ = abs(center_temp_x - center[0]);						// wx2'
		if (center[0] > center_last[0]) { // �p�G�������O + ����
			center_fall[0] = center_temp_x + wx; b = TRUE;			// C2'
			face_texture_falling[0][1] = face_texture_falling[1][1] =
				face_texture[2][1] = face_texture[3][1] = face_texture[0][1] + (wx - wx2_) / STACK_CUBOID_INITIAL_WIDTH; /* z+ */

		}
		else { // �������O - ����
			center_fall[0] = center_temp_x - wx; b = FALSE;
			face_texture_falling[2][1] = face_texture_falling[3][1] =
				face_texture[0][1] = face_texture[1][1] = face_texture[2][1] - (wx - wx2_) / STACK_CUBOID_INITIAL_WIDTH; /* z- */
		}
		wx = wx - wx2_;												// wx2
		center_last[0] = center[0] = center_temp_x;
	}
	else {// �p�G�O z ��V�Ӧ^
		float center_temp_z = (center_last[2] + center[2]) / 2;		// C2
		wz2_ = abs(center_temp_z - center[2]);						// wz2'
		if (center[2] > center_last[2]) {// �������O + ����
			center_fall[2] = center_temp_z + wz; b = TRUE;			// C2'
			face_texture_falling[0][0] = face_texture_falling[3][0] =
				face_texture[1][0] = face_texture[2][0] = face_texture[0][0] + (wz - wz2_) / STACK_CUBOID_INITIAL_WIDTH; /* x+ */
		}
		else {// �������O - ����
			center_fall[2] = center_temp_z - wz; b = FALSE;
			face_texture_falling[1][0] = face_texture_falling[2][0] =
				face_texture[0][0] = face_texture[3][0] = face_texture[1][0] - (wz - wz2_) / STACK_CUBOID_INITIAL_WIDTH; /* x- */
		}
		wz = wz - wz2_;												// wz2
		center_last[2] = center[2] = center_temp_z;
	}


	CalCuboidVertex(face_vertex_fall, center_fall, wx2_, wz2_);	// �U����
	fallcub.reset(face_vertex_fall, face_texture_falling, CuboidColor, osci_direct);

	CalCuboidVertex(face_vertex, center, wx, wz);				// ���|�W��
	CUBOID cub(face_vertex, face_texture, CuboidColor);
	cubList.push_back(cub);

	for (int j = 0; j < 4; j++) { for (int i = 0; i < 2; i++) { face_texture_falling[j][i] = face_texture[j][i]; } }
	osci_direct = !osci_direct; t = 0;
	ColorCircle();
}

/* �p��|�ʪ���������� */
void CalCuboidCenter() {
	center[0] = center_last[0] + osci_direct*osci_length*cos(float(t)*3.141592f / 180.0f);//X�_��
	center[2] = center_last[2] + (!osci_direct)*osci_length*cos(float(t)*3.141592f / 180.0f);//Z�_��
}

/* �����ߩM����p���������I�y�� */
void CalCuboidVertex(float(*F)[3], float*C, float X, float Z) {
	/* 1. + + +*/
	F[0][0] = C[0] + X; F[0][1] = hy; F[0][2] = C[2] + Z;
	/* 2. + - + */
	F[1][0] = C[0] + X; F[1][1] = 0; F[1][2] = C[2] + Z;
	/* 3. + - - */
	F[2][0] = C[0] + X; F[2][1] = 0; F[2][2] = C[2] - Z;
	/* 4. + + - */
	F[3][0] = C[0] + X; F[3][1] = hy; F[3][2] = C[2] - Z;
	/* 5. - + - */
	F[4][0] = C[0] - X; F[4][1] = hy; F[4][2] = C[2] - Z;
	/* 6. - + + */
	F[5][0] = C[0] - X; F[5][1] = hy; F[5][2] = C[2] + Z;
	/* 7. - - + */
	F[6][0] = C[0] - X; F[6][1] = 0; F[6][2] = C[2] + Z;
	/* 1' - - - */
	F[7][0] = C[0] - X; F[7][1] = 0; F[7][2] = C[2] - Z;
}

/* �e�b�ʪ���� */
void renderCuboid() {
	CalCuboidCenter();
	CalCuboidVertex(face_vertex, center, wx, wz);
	if (skin == STACK_TEXTURE_PURE_COLOR) {		// �¦�
		glColor3ubv(CuboidColor);
		renderCuboidFaceAll(face_vertex);
	}
	else if (skin == STACK_TEXTURE_SKELETON) {	// �u�����[
		renderCuboidEdge(face_vertex);
	}
	else if (skin == STACK_TEXTURE_GLASS) {		// �z���Pı
	}
	else if (skin == STACK_TEXTURE_MIRROR) {	// �譱�ĪG
	}
	else {		// ���K�Ϫ�
		renderCuboidFace_Texture_y(face_vertex);
		glColor3ubv(CuboidColor); 
		renderCuboidFace_No_y(face_vertex);	// ����N���q�C��
	}

}

/* �e�w�g��n����� */
void renderCuboidList() {
	int cubnum = cubList.size();	// ����Ӽ�
	if (t < hy && show_SlowDown) {// �w��
		glMatrixMode(GL_MODELVIEW); glPushMatrix();
		glTranslatef(0, hy - t, 0);
	}
	if (skin != STACK_TEXTURE_SKELETON) {
		renderFirstCub(cubnum + 1);		// ���\�L���=>���U�@�h
	}
	for (int i = 0; i < cubnum; i++) {
		glMatrixMode(GL_MODELVIEW); glPushMatrix();
		glTranslatef(0, -hy*(cubnum - i), 0);
		glColor3ubv(cubList[i].color);
		if (skin == STACK_TEXTURE_PURE_COLOR) {		// �¦�
			renderCuboidFaceAll(cubList[i].vertex);
		}
		else if (skin == STACK_TEXTURE_SKELETON) {	// �u�����[
			renderCuboidEdge(cubList[i].vertex);
		}
		else if (skin == STACK_TEXTURE_GLASS) {		// �z���ĪG
		}
		else if (skin == STACK_TEXTURE_MIRROR) {	// �譱�ĪG
		}
		else {	// ���K�Ϫ�
			renderCuboidFace_Texture_y(cubList[i]);
			glColor3ubv(cubList[i].color); 
			renderCuboidFace_No_y(cubList[i].vertex);	// ����N���q�C��
		}
		glMatrixMode(GL_MODELVIEW); glPopMatrix();
	}
	if (t < hy && show_SlowDown) { // �w������
		glMatrixMode(GL_MODELVIEW); glPopMatrix();
	}
}

/* �e�̫�@�Ӥ���������� */
void renderFirstCub(int cubnum) {
	glMatrixMode(GL_MODELVIEW); glPushMatrix();
	glTranslatef(0, GLfloat(-hy*cubnum), 0);
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	// +x
	glNormal3fv(face_normal[0]);
	glColor4ub(cubList[0].color[0], cubList[0].color[1], cubList[0].color[2], 255);
	glVertex3fv(cubList[0].vertex[3]); glVertex3fv(cubList[0].vertex[0]);
	glColor4f((*ClearColorNow)[0], (*ClearColorNow)[1], (*ClearColorNow)[2], 0.0f);
	glVertex3f(cubList[0].vertex[1][0], -3.0f*hy, cubList[0].vertex[1][2]);
	glVertex3f(cubList[0].vertex[2][0], -3.0f*hy, cubList[0].vertex[2][2]);
	// +z
	glNormal3fv(face_normal[2]);
	glColor4ub(cubList[0].color[0], cubList[0].color[1], cubList[0].color[2], 255);
	glVertex3fv(cubList[0].vertex[0]); glVertex3fv(cubList[0].vertex[5]);
	glColor4f((*ClearColorNow)[0], (*ClearColorNow)[1], (*ClearColorNow)[2], 0.0f);
	glVertex3f(cubList[0].vertex[6][0], -7.0f*hy, cubList[0].vertex[6][2]);
	glVertex3f(cubList[0].vertex[1][0], -7.0f*hy, cubList[0].vertex[1][2]);
	// -x
	glNormal3fv(face_normal[3]);
	glColor4ub(cubList[0].color[0], cubList[0].color[1], cubList[0].color[2], 255);
	glVertex3fv(cubList[0].vertex[5]); glVertex3fv(cubList[0].vertex[4]);
	glColor4f((*ClearColorNow)[0], (*ClearColorNow)[1], (*ClearColorNow)[2], 0.0f);
	glVertex3f(cubList[0].vertex[7][0], -7.0f*hy, cubList[0].vertex[7][2]);
	glVertex3f(cubList[0].vertex[6][0], -7.0f*hy, cubList[0].vertex[6][2]);
	// -z
	glNormal3fv(face_normal[5]);
	glColor4ub(cubList[0].color[0], cubList[0].color[1], cubList[0].color[2], 255);
	glVertex3fv(cubList[0].vertex[4]); glVertex3fv(cubList[0].vertex[3]);
	glColor4f((*ClearColorNow)[0], (*ClearColorNow)[1], (*ClearColorNow)[2], 0.0f);
	glVertex3f(cubList[0].vertex[2][0], -7.0f*hy, cubList[0].vertex[2][2]);
	glVertex3f(cubList[0].vertex[7][0], -7.0f*hy, cubList[0].vertex[7][2]);
	glEnd();
	glMatrixMode(GL_MODELVIEW); glPopMatrix();
	glEnable(GL_LIGHTING);
}

/* ����m��`�� */
void ColorCircle() {
	static int ds = 5;	static int dv = 5;
	static int dh = 10;	static int di = 0;
	switch (di) {
	case 0:
		CuboidColor_H = ((CuboidColor_H + dh) < 0 ? (CuboidColor_H - dh) : (CuboidColor_H + dh)) % 360; break;
	case 1:
		CuboidColor_S = CuboidColor_S + ds;
		if (CuboidColor_S > 100) { CuboidColor_S = 100; ds *= -1; }
		if (CuboidColor_S < 20) { CuboidColor_S = 20; ds *= -1; }
		break;
	case 2:
		CuboidColor_V = CuboidColor_V + dv;
		if (CuboidColor_V > 100) { CuboidColor_V = 100; dv *= -1; }
		if (CuboidColor_V < 20) { CuboidColor_V = 20; dv *= -1; }
		break;
	}

	if (rand() > 30000) { dh = -dh; }			// ��������
	if (rand() > 30000) { di = rand() % 3; }	// ���@�Ӧ樫��V

	int H = (CuboidColor_H / 60) % 6;
	float v_ = float(CuboidColor_V) / 100.0f;
	float s_ = float(CuboidColor_S) / 100.0f;
	float f_ = float(CuboidColor_H) / 60.0f - float(H);
	float p_ = v_ * (1 - s_);
	float q_ = v_ * (1 - f_ *s_);
	float t_ = v_ * (1 - (1 - f_)*s_);

	float r, g, b;
	switch (H) {
		case 0: r = v_; g = t_; b = p_; break;
		case 1: r = q_; g = v_; b = p_; break;
		case 2: r = p_; g = v_; b = t_; break;
		case 3: r = p_; g = q_; b = v_; break;
		case 4: r = t_; g = p_; b = v_; break;
		case 5: r = v_; g = p_; b = q_; break;
		default:break;
	}
	CuboidColor[0] = GLubyte(r * 255);
	CuboidColor[1] = GLubyte(g * 255);
	CuboidColor[2] = GLubyte(b * 255);
	if (CuboidColor_V > 40 || skin == STACK_TEXTURE_SKELETON) { ClearColorNow = (&ClearColorDark); }
	else { ClearColorNow = (&ClearColorLight); }
	glClearColor((*ClearColorNow)[0], (*ClearColorNow)[1], (*ClearColorNow)[2], 0.0f);
}

/* �e�u�����[��� */
void renderCuboidEdge(float(*C)[3]) {
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0); // �G�u - ����
	glVertex3fv(C[0]); glVertex3fv(C[3]);
	glVertex3fv(C[0]); glVertex3fv(C[5]);
	glVertex3fv(C[2]); glVertex3fv(C[3]);
	glVertex3fv(C[2]); glVertex3fv(C[7]);
	glVertex3fv(C[4]); glVertex3fv(C[5]);
	glVertex3fv(C[4]); glVertex3fv(C[3]);
	glVertex3fv(C[4]); glVertex3fv(C[7]);
	glVertex3fv(C[6]); glVertex3fv(C[5]);
	glVertex3fv(C[6]); glVertex3fv(C[7]);

	glColor3ub(100, 0, 0); // �t�u - �I��
	glVertex3fv(C[0]); glVertex3fv(C[1]);
	glVertex3fv(C[2]); glVertex3fv(C[1]);
	glVertex3fv(C[6]); glVertex3fv(C[1]);
	glEnd();
}

/* �e������Ҧ��� */
void renderCuboidFaceAll(float(*F)[3]) {
	glBegin(GL_QUADS);
	// +y
	glNormal3fv(face_normal[1]);
	glVertex3fv(F[4]); glVertex3fv(F[5]); glVertex3fv(F[0]); glVertex3fv(F[3]);
	// +x
	glNormal3fv(face_normal[0]);
	glVertex3fv(F[0]); glVertex3fv(F[1]); glVertex3fv(F[2]); glVertex3fv(F[3]);
	// +z
	glNormal3fv(face_normal[2]);
	glVertex3fv(F[0]); glVertex3fv(F[5]); glVertex3fv(F[6]); glVertex3fv(F[1]);
	// -x
	glNormal3fv(face_normal[3]);
	glVertex3fv(F[7]); glVertex3fv(F[6]); glVertex3fv(F[5]); glVertex3fv(F[4]);
	// -y
	glNormal3fv(face_normal[4]);
	glVertex3fv(F[1]); glVertex3fv(F[6]); glVertex3fv(F[7]); glVertex3fv(F[2]);
	// -z
	glNormal3fv(face_normal[5]);
	glVertex3fv(F[7]); glVertex3fv(F[4]); glVertex3fv(F[3]); glVertex3fv(F[2]);
	glEnd();
}

/* �e������F+y���� */
void renderCuboidFace_No_y(float(*F)[3]) {
	glBegin(GL_QUADS);
	// +x
	glNormal3fv(face_normal[0]);
	glVertex3fv(F[0]); glVertex3fv(F[1]); glVertex3fv(F[2]); glVertex3fv(F[3]);
	// +z
	glNormal3fv(face_normal[2]);
	glVertex3fv(F[0]); glVertex3fv(F[5]); glVertex3fv(F[6]); glVertex3fv(F[1]);
	// -x
	glNormal3fv(face_normal[3]);
	glVertex3fv(F[7]); glVertex3fv(F[6]); glVertex3fv(F[5]); glVertex3fv(F[4]);
	// -y
	glNormal3fv(face_normal[4]);
	glVertex3fv(F[1]); glVertex3fv(F[6]); glVertex3fv(F[7]); glVertex3fv(F[2]);
	// -z
	glNormal3fv(face_normal[5]);
	glVertex3fv(F[7]); glVertex3fv(F[4]); glVertex3fv(F[3]); glVertex3fv(F[2]);
	glEnd();
}

/* �e��������K��+y��(�b�ʪ�) */
void renderCuboidFace_Texture_y(float(*F)[3]) {
	glPushMatrix();
	glEnable(GL_TEXTURE);
	SetupTexture();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glNormal3fv(face_normal[1]);
	glTexCoord2fv(face_texture[0]); glVertex3fv(face_vertex[4]);
	glTexCoord2fv(face_texture[1]); glVertex3fv(face_vertex[5]);
	glTexCoord2fv(face_texture[2]); glVertex3fv(face_vertex[0]);
	glTexCoord2fv(face_texture[3]); glVertex3fv(face_vertex[3]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE);
	Texture = NULL;
	glPopMatrix();
}

/* �e��������K��+y��(�S�b�ʪ�) */
void renderCuboidFace_Texture_y(CUBOID &C) {
	glPushMatrix();
	glEnable(GL_TEXTURE);
	SetupTexture();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glNormal3fv(face_normal[1]);
	glTexCoord2fv(C.texcoord[0]); glVertex3fv(C.vertex[4]);
	glTexCoord2fv(C.texcoord[1]); glVertex3fv(C.vertex[5]);
	glTexCoord2fv(C.texcoord[2]); glVertex3fv(C.vertex[0]);
	glTexCoord2fv(C.texcoord[3]); glVertex3fv(C.vertex[3]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE);
	Texture = NULL;
	glPopMatrix();
}

/* �_���ĪG�A�^�Ǿ_����m�Cí�A��m�� 1, Ts = 3, P.O. =  8% */
float transiet_respond(float steady_respond, float t_now, float Ts)
{
	// steady_respond	�r��
	// t_now			�{�b�ɶ�
	// Ts				�w�w�ɶ� Ts > t
	// ���w �a=0.632, �sn =3.16 ,Ts = 3, P.O. =  8%
	// T = (11.1*(s+18)) / ((s+20)*(s^2+4*s+10))
	// �o�Өt�Υu�|�u���@���ӥB�w�w�ɶ��Z�u�A�ܴ�
	if (t_now > Ts) { return steady_respond; }
	float t = 3.0f*t_now / Ts;
	float respond = ((37.0f * exp(-20.0f * t)) / 11000.0f - (5513.0f * exp(-2.0f * t)*(cos(pow(6.0f, 0.5f)*t) + (48 * pow(6.0f, 0.5f)*sin(pow(6.0f, 0.5f)*t)) / 149.0f)) / 5500.0f + 999.0f / 1000.0f);
	if (abs(respond - 1.0f) < 0.02) { return steady_respond; }
	else { return steady_respond*respond; }
}

/* ø�s Tap to START �}�l��r */
void renderTapToSTART() {
	glMatrixMode(GL_MODELVIEW); glPushMatrix();
	glLoadIdentity();
	glColor3ubv(FontColorLight);
	glRasterPos2f(0.0, 25.0f);

	string t = "TAP to START";
	char const *n_char = t.c_str();
	FontPrintf(Font_big, 0, n_char);
	glMatrixMode(GL_MODELVIEW); glPopMatrix();

}

/* �e�I�� */
void renderBackground() {
	glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); glOrtho(-1, 1, -1, 1, -3, 3);
	glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

	glBegin(GL_QUADS);
	glColor3fv(ClearColorDark);
	glVertex3f(-1, -1, -3);
	glVertex3f(1, -1, -3);
	glColor3ubv(CuboidColor);
	glVertex3f(1, 1, -3);
	glVertex3f(-1, 1, -3);
	glEnd();

	glMatrixMode(GL_MODELVIEW); glPopMatrix();
	glMatrixMode(GL_PROJECTION); glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

/* �_�l���� */
void stack_initial_scene(bool first) {
	static int t_pop_up = 1;
	if (first) { // �Ĥ@���I�s�N�u���]�ɶ�
		t_pop_up = 1;// 0 ���ܴN�۷��S���_���ҥH�]1
	}
	else {
		if (t_pop_up < 20 && show_2OrderSys) {
			t_pop_up += 1;
			glMatrixMode(GL_MODELVIEW); glPushMatrix();
			glTranslatef(0.0, -300.0f + transiet_respond(300.0f, t_pop_up, 20), 0.0f);
		}
		glMatrixMode(GL_MODELVIEW); glPushMatrix();
		renderCuboidList();
		renderTapToSTART();
		glMatrixMode(GL_MODELVIEW); glPopMatrix();
		if (t_pop_up < 20 && show_2OrderSys) {
			glMatrixMode(GL_MODELVIEW); glPopMatrix();
		}
	}
	if (show_Buttom) {stack_initial_buttom();}
}

/* �C������ */
void stack_playing_scene() {
	// renderBackground();
	glMatrixMode(GL_MODELVIEW); glPushMatrix();
	renderCuboid();		/* �b�ʪ� */
	if (show_Falling)
	{
		if (skin >= STACK_TEXTURE_STATIC) { fallcub.render_texture(t, b); }/* �������� */
		else if (skin == STACK_TEXTURE_SKELETON) { fallcub.render_skeleton(t, b); }
		else { fallcub.render(t, b); }
	}
	renderCuboidList();	/* �w�g�|�n�� */
	glMatrixMode(GL_MODELVIEW); glPopMatrix();
	if (show_Buttom) { stack_playing_buttom(); }
}

/* �������� */
void stack_gameover_scene(bool first) {
	static float t_zoom = 15.0f;
	if (first) {t_zoom = 15.0f;}// �Ĥ@���I�s�N�u���]�˼Ʈɶ�
	else if(show_SlowDown){		// �ĤG���I�s�}�l�~�u���}�l�e
				// �ʵe���� t_zoom = 0 ����
		if (t_zoom > 0) {t_zoom = t_zoom - 1.0f;}
		float zoom = 0.5f + t_zoom / 30.0f;
		glMatrixMode(GL_MODELVIEW); glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glScalef(zoom, zoom, zoom);
		renderCuboidList();
		glMatrixMode(GL_MODELVIEW); glPopMatrix();
	}
	if (show_Buttom) {stack_gameover_buttom();}
}

/* ��ܶK�ϳ��� */
void stack_texture_scene() {
	stack_texture_buttom(); //�����F�C
}

/* �[�W glScalef �� glTranslatef */
void Translatef_Scalef(float x, float y, float z) {
	glTranslatef(x, y, z);
	if (w_bigger_than_h) { glScalef(1 / ratio, 1.0f, 1.0f); }
	else { glScalef(1.0f, 1.0f / ratio, 1.0f); }
}

/* �I�����I�줰��? */
void ProcessSelection(int xPos, int yPos) {
	GLuint selectBuff[BUFFER_LENGTH];
	GLint hits, viewport[4];
	glSelectBuffer(BUFFER_LENGTH, selectBuff);
	glGetIntegerv(GL_VIEWPORT, viewport); // (0,0) (w,h)
	glMatrixMode(GL_PROJECTION); glPushMatrix();
	glRenderMode(GL_SELECT);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -3, 3);
	gluPickMatrix(xPos, viewport[3] - yPos, 0.1f, 0.1f, viewport);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (scene) {
	case STACK_START:
		stack_initial_feedback();
		break;
	case STACK_PLAY:
		stack_playing_feedback();
		break;
	case STACK_GAME_OVER:
		stack_gameover_feedback();
		break;
	case STACK_TEXTURE_SELECTION_PAGE_1: case STACK_TEXTURE_SELECTION_PAGE_2:
		stack_texture_feedback(); //
		break;
	}
	hits = glRenderMode(GL_RENDER);
	if (hits == 0) { // �S�I��F��
		if (scene < STACK_TEXTURE_SELECTION_PAGE_1) { SpaceBar(); }
	}
	else {			// �I��F��F�A���I��T�p
		switch (selectBuff[3]) {
		case BUTTOM_EXIT:save_record(); exit(0); break;
		case BUTTOM_CLEAR:delete_record(); break;
		case BUTTOM_RESTART:
			scene = STACK_START;
			fallcub.reset(0);
			cubList.clear();
			SetupParameter();
			stack_initial_scene(RESET_TIME);
			break;
		case BUTTOM_TEXTURE:
			scene = STACK_TEXTURE_SELECTION_PAGE_1;
			fallcub.reset(0);
			cubList.clear();
			break;
		case BUTTOM_LASTPAGE:scene = STACK_TEXTURE_SELECTION_PAGE_1; break;
		case BUTTOM_NEXTPAGE:scene = STACK_TEXTURE_SELECTION_PAGE_2; break;
		case BUTTOM_CANCEL:
			scene = STACK_START;
			fallcub.reset(0);
			cubList.clear();
			SetupParameter();
			break;
		default:
			if (scene == STACK_TEXTURE_SELECTION_PAGE_1) {
				switch (selectBuff[3])
				{
				case BUTTOM_TL:	skin = STACK_TEXTURE_PURE_COLOR;
					break;
				case BUTTOM_TM: skin = STACK_TEXTURE_SKELETON;
					break;
				case BUTTOM_TR:
					random_skin = !random_skin;
					if (!random_skin) { skin = STACK_TEXTURE_PURE_COLOR; }
					break;
				case BUTTOM_CL: skin = STACK_TEXTURE_STATIC;
					break;
				case BUTTOM_CM: skin = STACK_TEXTURE_DYNAMIC;
					break;
				case BUTTOM_CR: skin = STACK_TEXTURE_THERMODYNAMIC;
					break;
				case BUTTOM_BL: skin = STACK_TEXTURE_METERIAL;
					break;
				case BUTTOM_BM: skin = STACK_TEXTURE_FLUID;
					break;
				case BUTTOM_BR: skin = STACK_TEXTURE_ENGMATH;
					break;
				}
				if (selectBuff[3] != BUTTOM_TR) { random_skin = FALSE; }
			}
			else {// scene == STACK_TEXTURE_SELECTION_PAGE_2
				random_skin = FALSE;
				switch (selectBuff[3])
				{
				case BUTTOM_TL: skin = STACK_TEXTURE_ADVENGMATH;
					break;
				case BUTTOM_TM: skin = STACK_TEXTURE_MECHANISM;
					break;
				case BUTTOM_TR: skin = STACK_TEXTURE_EE;
					break;
				case BUTTOM_CL: skin = STACK_TEXTURE_ED;
					break;
				case BUTTOM_CM: skin = STACK_TEXTURE_MANUFACTURE;
					break;
				case BUTTOM_CR: skin = STACK_TEXTURE_DESIGN;
					break;
				case BUTTOM_BL: skin = STACK_TEXTURE_CONTROL;
					break;
				case BUTTOM_BM: skin = STACK_TEXTURE_CPP;
					break;
				case BUTTOM_BR: skin = STACK_TEXTURE_CG;
					break;
				}
			}

			break;
		}
	}
	if (!(skin*records[skin])) { skin = STACK_TEXTURE_PURE_COLOR; } // �S���ꤣ����
	glMatrixMode(GL_PROJECTION); glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

/* ��� bitmap �C����: ���s, �h�X */
void stack_playing_buttom() {
	glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();glOrtho(-1, 1, -1, 1, -3, 3);
	if (*ClearColorNow == ClearColorDark) { bitmapcolor = 0.6f; }
	else { bitmapcolor = 0.9f; }
	glMatrixMode(GL_MODELVIEW); glPushMatrix();
	// �Ϯש����ܲ`
	glColor3f((*ClearColorNow[0])*bitmapcolor, (*ClearColorNow[0])*bitmapcolor, (*ClearColorNow[0])*bitmapcolor);	
	glLoadIdentity(); Translatef_Scalef(0.8f, 0.9f, 1.5f);	glutSolidCube(BUTTOM_SIZE);	// exit
	glLoadIdentity(); Translatef_Scalef(-0.8f, 0.9f, 1.5f); glutSolidCube(BUTTOM_SIZE);	// restart
	glLoadIdentity();
	// �Ϯץ����ܲL
	glColor3f((*ClearColorNow[0]) / bitmapcolor, (*ClearColorNow[0]) / bitmapcolor, (*ClearColorNow[0]) / bitmapcolor);
	glRasterPos3f(0.8f, 0.9f, 2.0f); buttom_exit32();
	glRasterPos3f(-0.8f, 0.9f, 2.0f); buttom_restart32();
	glMatrixMode(GL_PROJECTION); glPopMatrix();
	glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

/* �I��buffer-�C���� */
void stack_playing_feedback() {
	glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

	glInitNames();glPushName(0);// ��l�� name stack�B�S�I�����F��N�O0
	glPushMatrix(); Translatef_Scalef(0.8f, 0.9f, 2.0f);	glLoadName(BUTTOM_EXIT);	glutSolidCube(BUTTOM_SIZE); glPopMatrix();
	glPushMatrix(); Translatef_Scalef(-0.8f, 0.9f, 2.0f);	glLoadName(BUTTOM_RESTART);	glutSolidCube(BUTTOM_SIZE); glPopMatrix();

	glPopMatrix();
}

/* ��� bitmap �_�l: ��K��, �M������, �h�X, */
void stack_initial_buttom() {
	glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();glOrtho(-1, 1, -1, 1, -3, 3);
	if (*ClearColorNow == ClearColorDark) { bitmapcolor = 0.6f; }
	else { bitmapcolor = 0.9f; }
	glMatrixMode(GL_MODELVIEW); glPushMatrix();
	// �Ϯש����ܲ`
	glColor3f((*ClearColorNow[0])*bitmapcolor, (*ClearColorNow[0])*bitmapcolor, (*ClearColorNow[0])*bitmapcolor);
	glLoadIdentity(); Translatef_Scalef(0.8f, 0.9f, 1.5f);	glutSolidCube(BUTTOM_SIZE);	// exit
	glLoadIdentity(); Translatef_Scalef(-0.2f, -0.8f, 1.5f);glutSolidCube(BUTTOM_SIZE);	// texture
	glLoadIdentity(); Translatef_Scalef(0.2f, -0.8f, 1.5f);	glutSolidCube(BUTTOM_SIZE);	// clear

	glLoadIdentity();
	// �Ϯץ����ܲL
	glColor3f((*ClearColorNow[0]) / bitmapcolor, (*ClearColorNow[0]) / bitmapcolor, (*ClearColorNow[0]) / bitmapcolor);
	glRasterPos3f(0.8f, 0.9f, 2.0f); buttom_exit32();
	glRasterPos3f(-0.2f, -0.8f, 2.0f); buttom_texture32();
	glRasterPos3f(0.2f, -0.8f, 2.0f); buttom_clear32();

	glMatrixMode(GL_PROJECTION); glPopMatrix();
	glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

/* �I��buffer-�_�l: ��K��, �M������, �h�X, */
void stack_initial_feedback() {
	glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

	glInitNames(); glPushName(0);// ��l�� name stack�B�S�I�����F��N�O0
	glPushMatrix(); Translatef_Scalef(0.8f, 0.9f, 2.0f);	glLoadName(BUTTOM_EXIT);	glutSolidCube(BUTTOM_SIZE);	glPopMatrix();
	glPushMatrix(); Translatef_Scalef(-0.2f, -0.8f, 2.0f);	glLoadName(BUTTOM_TEXTURE);	glutSolidCube(BUTTOM_SIZE);	glPopMatrix();
	glPushMatrix(); Translatef_Scalef(0.2f, -0.8f, 2.0f);	glLoadName(BUTTOM_CLEAR);	glutSolidCube(BUTTOM_SIZE);	glPopMatrix();

	glPopMatrix();
}

/* ��� bitmap-����: ��K��, �h�X */
void stack_gameover_buttom() {
	glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); glOrtho(-1, 1, -1, 1, -3, 3);
	if (*ClearColorNow == ClearColorDark) { bitmapcolor = 0.6f; }
	else { bitmapcolor = 0.9f; }
	glMatrixMode(GL_MODELVIEW); glPushMatrix();
	// �Ϯש����ܲ`
	glColor3f((*ClearColorNow[0])*bitmapcolor, (*ClearColorNow[0])*bitmapcolor, (*ClearColorNow[0])*bitmapcolor);
	glLoadIdentity(); Translatef_Scalef(0.2f, -0.8f, 1.5f);	glutSolidCube(BUTTOM_SIZE);	// exit
	glLoadIdentity(); Translatef_Scalef(-0.2f, -0.8f, 1.5f);glutSolidCube(BUTTOM_SIZE);	// texture

	glLoadIdentity();
	// �Ϯץ����ܲL
	glColor3f((*ClearColorNow[0]) / bitmapcolor, (*ClearColorNow[0]) / bitmapcolor, (*ClearColorNow[0]) / bitmapcolor);
	glRasterPos3f(0.2f, -0.8f, 2.0f); buttom_exit32();
	glRasterPos3f(-0.2f, -0.8f, 2.0f); buttom_texture32();

	glMatrixMode(GL_PROJECTION); glPopMatrix();
	glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

/* �I��buffer-����: ��K��, �h�X */
void stack_gameover_feedback() {
	glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

	glInitNames(); glPushName(0);// ��l�� name stack�B�S�I�����F��N�O0
	glPushMatrix(); Translatef_Scalef(0.2f, -0.8f, 2.0f);	glLoadName(BUTTOM_EXIT);	glutSolidCube(BUTTOM_SIZE);	glPopMatrix();
	glPushMatrix(); Translatef_Scalef(-0.2f, -0.8f, 2.0f);	glLoadName(BUTTOM_TEXTURE);	glutSolidCube(BUTTOM_SIZE);	glPopMatrix();

	glPopMatrix();
}

/* ��� bitmap-���: 1~9�ӶK��, �W�U�@��, ����, �h�X */
void stack_texture_buttom() {
	glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); glOrtho(-1, 1, -1, 1, -3, 3);
	if (*ClearColorNow == ClearColorDark) { bitmapcolor = 0.6f; }
	else { bitmapcolor = 0.9f; }
	glMatrixMode(GL_MODELVIEW); glPushMatrix();
	
	float color = (*ClearColorNow[0])*bitmapcolor;// �Ϯש����ܲ`
	glColor3f(color, color, color);
	glLoadIdentity(); Translatef_Scalef(0.8f, 0.9f, 1.5f);		glutSolidCube(BUTTOM_SIZE);	// exit
	glLoadIdentity(); Translatef_Scalef(-0.8f, 0.9f, 1.5f);		glutSolidCube(BUTTOM_SIZE);	// cancel
	if (scene == STACK_TEXTURE_SELECTION_PAGE_1)
	{glLoadIdentity(); Translatef_Scalef(0.2f, 0.75f, 1.5f);	glutSolidCube(BUTTOM_SIZE);}	// next page
	else 
	{glLoadIdentity(); Translatef_Scalef(-0.2f, 0.75f, 1.5f);	glutSolidCube(BUTTOM_SIZE);}	// last page
	
	// �ﶵ�E�c��
	glLoadIdentity(); Translatef_Scalef(-0.5f, 0.5f, 1.5f);		TextureSolidCube(SAMPLE_SIZE, BUTTOM_TL);	// 1
	glLoadIdentity(); Translatef_Scalef(0.0f, 0.5f, 1.5f);		TextureSolidCube(SAMPLE_SIZE, BUTTOM_TM);	// 2
	glLoadIdentity(); Translatef_Scalef(0.5f, 0.5f, 1.5f);		TextureSolidCube(SAMPLE_SIZE, BUTTOM_TR);	// 3
	glLoadIdentity(); Translatef_Scalef(-0.5f, 0.0f, 1.5f);		TextureSolidCube(SAMPLE_SIZE, BUTTOM_CL);	// 4
	glLoadIdentity(); Translatef_Scalef(0.0f, 0.0f, 1.5f);		TextureSolidCube(SAMPLE_SIZE, BUTTOM_CM);	// 5
	glLoadIdentity(); Translatef_Scalef(0.5f, 0.0f, 1.5f);		TextureSolidCube(SAMPLE_SIZE, BUTTOM_CR);	// 6
	glLoadIdentity(); Translatef_Scalef(-0.5f, -0.5f, 1.5f);	TextureSolidCube(SAMPLE_SIZE, BUTTOM_BL);	// 7
	glLoadIdentity(); Translatef_Scalef(0.0f, -0.5f, 1.5f);		TextureSolidCube(SAMPLE_SIZE, BUTTOM_BM);	// 8
	glLoadIdentity(); Translatef_Scalef(0.5f, -0.5f, 1.5f);		TextureSolidCube(SAMPLE_SIZE, BUTTOM_BR);	// 9

	glLoadIdentity();
	color = (*ClearColorNow[0])/bitmapcolor;// �Ϯץ����ܲL	
	glColor3f(color, color, color);
	glRasterPos3f(0.8f, 0.9f, 2.0f); buttom_exit32();
	glRasterPos3f(-0.8f, 0.9f, 2.0f); buttom_cancel32();
	if (scene == STACK_TEXTURE_SELECTION_PAGE_1) {glRasterPos3f(0.2f, 0.75f, 2.0f); buttom_nextpage32();}
	else {glRasterPos3f(-0.2f, 0.75f, 2.0f); buttom_lastpage32();}
	
	glMatrixMode(GL_PROJECTION); glPopMatrix();
	glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

/* �I��buffer-���: 1~9�ӶK��, �W�U�@��, ����, �h�X */
void stack_texture_feedback() {
	glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
	glInitNames(); glPushName(0);// ��l�� name stack�B�S�I�����F��N�O0

	glPushMatrix(); Translatef_Scalef(0.8f, 0.9f, 1.5f);	glLoadName(BUTTOM_EXIT);	glutSolidCube(BUTTOM_SIZE); glPopMatrix();	// exit
	glPushMatrix(); Translatef_Scalef(-0.8f, 0.9f, 1.5f);	glLoadName(BUTTOM_CANCEL);	glutSolidCube(BUTTOM_SIZE); glPopMatrix();	// cancel
	if (scene == STACK_TEXTURE_SELECTION_PAGE_1)
	{glPushMatrix(); Translatef_Scalef(0.2f, 0.75f, 1.5f);	glLoadName(BUTTOM_NEXTPAGE); glutSolidCube(BUTTOM_SIZE); glPopMatrix();}	// next page
	else
	{glPushMatrix(); Translatef_Scalef(-0.2f, 0.75f, 1.5f);	glLoadName(BUTTOM_LASTPAGE); glutSolidCube(BUTTOM_SIZE); glPopMatrix();}	// last page
	

	glPushMatrix(); Translatef_Scalef(-0.5f, 0.5f, 1.5f);	glLoadName(BUTTOM_TL); glutSolidCube(SAMPLE_SIZE); glPopMatrix();	// 1
	glPushMatrix(); Translatef_Scalef(0.0f, 0.5f, 1.5f);	glLoadName(BUTTOM_TM); glutSolidCube(SAMPLE_SIZE); glPopMatrix();	// 2
	glPushMatrix(); Translatef_Scalef(0.5f, 0.5f, 1.5f);	glLoadName(BUTTOM_TR); glutSolidCube(SAMPLE_SIZE); glPopMatrix();	// 3
	glPushMatrix(); Translatef_Scalef(-0.5f, 0.0f, 1.5f);	glLoadName(BUTTOM_CL); glutSolidCube(SAMPLE_SIZE); glPopMatrix();	// 4
	glPushMatrix(); Translatef_Scalef(0.0f, 0.0f, 1.5f);	glLoadName(BUTTOM_CM); glutSolidCube(SAMPLE_SIZE); glPopMatrix();	// 5
	glPushMatrix(); Translatef_Scalef(0.5f, 0.0f, 1.5f);	glLoadName(BUTTOM_CR); glutSolidCube(SAMPLE_SIZE); glPopMatrix();	// 6
	glPushMatrix(); Translatef_Scalef(-0.5f, -0.5f, 1.5f);	glLoadName(BUTTOM_BL); glutSolidCube(SAMPLE_SIZE); glPopMatrix();	// 7
	glPushMatrix(); Translatef_Scalef(0.0f, -0.5f, 1.5f);	glLoadName(BUTTOM_BM); glutSolidCube(SAMPLE_SIZE); glPopMatrix();	// 8
	glPushMatrix(); Translatef_Scalef(0.5f, -0.5f, 1.5f);	glLoadName(BUTTOM_BR); glutSolidCube(SAMPLE_SIZE); glPopMatrix();	// 9

	glPopMatrix();
}

/* ��ܵe�����Y��ø�s */
void TextureSolidCube(float w_, int location) {
	GLuint Texture_sample; 	float w = w_ / 2.0f; int skin_rendering;
	glEnable(GL_TEXTURE); glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if (scene == STACK_TEXTURE_SELECTION_PAGE_1) {
		switch (location)
		{
		case BUTTOM_TL:	skin_rendering = STACK_TEXTURE_PURE_COLOR;
			break;
		case BUTTOM_TM: skin_rendering = STACK_TEXTURE_SKELETON;
			break;
		case BUTTOM_TR: skin_rendering = STACK_TEXTURE_RAMDOM;
			break;
		case BUTTOM_CL: skin_rendering = STACK_TEXTURE_STATIC;
			break;
		case BUTTOM_CM: skin_rendering = STACK_TEXTURE_DYNAMIC;
			break;
		case BUTTOM_CR: skin_rendering = STACK_TEXTURE_THERMODYNAMIC;
			break;
		case BUTTOM_BL: skin_rendering = STACK_TEXTURE_METERIAL;
			break;
		case BUTTOM_BM: skin_rendering = STACK_TEXTURE_FLUID;
			break;
		case BUTTOM_BR: skin_rendering = STACK_TEXTURE_ENGMATH;
			break;
		}
	}
	else {// scene == STACK_TEXTURE_SELECTION_PAGE_2
		switch (location)
		{
		case BUTTOM_TL: skin_rendering = STACK_TEXTURE_ADVENGMATH;
			break;
		case BUTTOM_TM: skin_rendering = STACK_TEXTURE_MECHANISM;
			break;
		case BUTTOM_TR: skin_rendering = STACK_TEXTURE_EE;
			break;
		case BUTTOM_CL: skin_rendering = STACK_TEXTURE_ED;
			break;
		case BUTTOM_CM: skin_rendering = STACK_TEXTURE_MANUFACTURE;
			break;
		case BUTTOM_CR: skin_rendering = STACK_TEXTURE_DESIGN;
			break;
		case BUTTOM_BL: skin_rendering = STACK_TEXTURE_CONTROL;
			break;
		case BUTTOM_BM: skin_rendering = STACK_TEXTURE_CPP;
			break;
		case BUTTOM_BR: skin_rendering = STACK_TEXTURE_CG;
			break;
		}
	}
	Texture_sample = SetupTexture(skin_rendering);
	glColor3ub(255, 255, 255);
	if (skin_rendering >= STACK_TEXTURE_THERMODYNAMIC) {
		if (!records[skin_rendering]) { glColor3ub(50, 50, 50); }
	}
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(w, w, w);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-w, w, w);
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-w, -w, w);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(w, -w, w);
	glEnd();
	glDisable(GL_TEXTURE); glDisable(GL_TEXTURE_2D);
	
	glColor3ub(255,255,0);
	if ((skin == skin_rendering && !random_skin) || (skin_rendering == STACK_TEXTURE_RAMDOM && random_skin)) { // ��۪�����
		glPushMatrix(); glTranslatef(0.0f, 0.0f, -0.1f);
		glutSolidCube(SAMPLE_SIZE + 0.05f);
		glPopMatrix();
	}
}

/* �e���� */
void render_score() {
	glPushMatrix(); glLoadIdentity(); /* viewport �Y�� (-1,-1)~(1,1)*/
	if (CuboidColor_V > 40 || skin == STACK_TEXTURE_SKELETON) { glColor3ubv(FontColorLight); }
	else { glColor3ubv(FontColorDark); }
	if (window_height > 1.25f*window_width) { glRasterPos3f(0, 0.5f, 0.1f); }
	else { glRasterPos3f(0, 0.75f, 0.1f); }
	// �{�b�o��
	string t = std::to_string(cubList.size() - 1);
	char const *n_char = t.c_str();
	FontPrintf(Font_big, 0, n_char);
	// �p�G���̤j�o��
	if (max_score) {
		string t_max_score = "Max score: " + std::to_string(max_score);
		char const *m_char = t_max_score.c_str();
		string t_credit = "Credit: " + std::to_string(credit);
		if (credit >= 144) { t_credit = t_credit + " (graduated)"; }
		char const *m_char_2 = t_credit.c_str();
		if (scene == STACK_GAME_OVER) {
			if (window_height > 1.25f*window_width) { glRasterPos3f(0, 0.4f, 0.1f); }
			else { glRasterPos3f(0, 0.65f, 0.1f); }
			FontPrintf(Font_small, 0, m_char); /* <0�a�k�B=0�m���B>0�a�� */
			if (window_height > 1.25f*window_width) { glRasterPos3f(0, 0.3f, 0.1f); }
			else { glRasterPos3f(0, 0.55f, 0.1f); }
			FontPrintf(Font_small, 0, m_char_2);
		}
		else if (scene == STACK_PLAY || scene == STACK_START) {
			glRasterPos3f(-1.0f, -0.9f, 0.1f);
			FontPrintf(Font_small, 1, m_char); /* <0�a�k�B=0�m���B>0�a�� */
		}
	}
	glPopMatrix();
}

/* �D�e�e */
void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// �β{�b���M����񺡵e��
	glMatrixMode(GL_PROJECTION); glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f); glRotatef(yRot, 0.0f, 1.0f, 0.0f); // �઺����
	glMatrixMode(GL_MODELVIEW);
	switch (scene) {
	case STACK_START:
		stack_initial_scene(COUNT_DOWN);
		break;
	case STACK_PLAY:
		stack_playing_scene();
		break;
	case STACK_GAME_OVER:
		stack_gameover_scene(COUNT_DOWN);
		break;
	case STACK_TEXTURE_SELECTION_PAGE_1: case STACK_TEXTURE_SELECTION_PAGE_2:
		calculate_record(); stack_texture_scene();
		break;
	}
	glMatrixMode(GL_PROJECTION); glPopMatrix();
	if (scene == STACK_PLAY || scene == STACK_GAME_OVER) { render_score();/*�e����*/ }
	glutSwapBuffers();		// �����w�İ�(�e��)
}

/* �]�w�Ĥ@�Ӥ�����y�� */
void SetupCubioVertex() {
	/* 0. + + +*/
	face_vertex[0][0] = wx;	face_vertex[0][1] = hy; face_vertex[0][2] = wz;
	/* 1. + - + */
	face_vertex[1][0] = wx; face_vertex[1][1] = 0; face_vertex[1][2] = wz;
	/* 2. + - - */
	face_vertex[2][0] = wx; face_vertex[2][1] = 0; face_vertex[2][2] = -wz;
	/* 3. + + - */
	face_vertex[3][0] = wx; face_vertex[3][1] = hy; face_vertex[3][2] = -wz;
	/* 4. - + - */
	face_vertex[4][0] = -wx; face_vertex[4][1] = hy; face_vertex[4][2] = -wz;
	/* 5. - + + */
	face_vertex[5][0] = -wx; face_vertex[5][1] = hy; face_vertex[5][2] = wz;
	/* 6. - - + */
	face_vertex[6][0] = -wx; face_vertex[6][1] = 0; face_vertex[6][2] = wz;
	/* 1' - - - */
	face_vertex[7][0] = -wx; face_vertex[7][1] = 0; face_vertex[7][2] = -wz;

	face_texture[0][0] = 0.0f;	face_texture[0][1] = 0.0f;
	face_texture[1][0] = 1.0f;	face_texture[1][1] = 0.0f;
	face_texture[2][0] = 1.0f;	face_texture[2][1] = 1.0f;
	face_texture[3][0] = 0.0f;	face_texture[3][1] = 1.0f;
	for (int j = 0; j < 4; j++) { for (int i = 0; i < 2; i++) { face_texture_falling[j][i] = face_texture[j][i]; } }
	CUBOID cub(face_vertex, face_texture, CuboidColor);
	cubList.push_back(cub);
}

/* ��l�Ƥ@�� */
void SetupRC() {// ��l��
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	// ���F�K�Ϫ��s�b
	glEnable(GL_CULL_FACE);	// �Q�B���N���εe�F�A�ٮɶ��C
	srand(time(0));			// �ɶ��ؤl-->�C��`����
	SetupParameter();		// ����Ѽƪ�l��
	Font_big = FontCreate(wglGetCurrentDC(), FontName, 50, 0, 0);	// �j���r
	Font_small = FontCreate(wglGetCurrentDC(), FontName, 25, 0, 0);	// �p���r
	load_record();	// Ū���s��
	SetupLight();	// ��
	glClearColor((*ClearColorNow)[0], (*ClearColorNow)[1], (*ClearColorNow)[2], 0.0f);	// �I����
}

/* ��l�Ƥ���Ѽ� */
void SetupParameter() {
	wx = STACK_CUBOID_INITIAL_WIDTH;
	wz = STACK_CUBOID_INITIAL_WIDTH;
	for (int i = 0; i < 3; i++) {
		center[i] = 0;
		center_last[i] = 0;
		for (int j = 0; j < 8; j++) {
			face_vertex[j][i] = 0;
		}
	}
	osci_direct = Z_OSCILLATION;
	t = 0; dt = 0; ddt = skin / 100.0f;
	CuboidColor_H = rand() % 360;
	CuboidColor_S = rand() % 90; if (CuboidColor_S < 20) { CuboidColor_S = 90; }
	CuboidColor_V = rand() % 90; if (CuboidColor_V < 20) { CuboidColor_V = 90; }
	ColorCircle();
	SetupCubioVertex();
	if (random_skin) {
		do { // �b�w���꧹�N���A�U�H��
			skin = int((float(rand()) / float(RAND_MAX + 1)) * 20.0f); // �T�O�S���� 1/32767 �����v�]�X 20
		} while (!(skin*(records[skin])) || skin == 2 || skin == 3 || skin == 4);
	}
	scene = STACK_START;
}

/* ��l�ƿO */
void SetupLight() {
	/* �O�� */
	static GLfloat lightPosition[] = { -300.0f,50.0f,-300.0f,1.0f };	//������m
	static GLfloat ambientLight[] = { 0.4f,0.4f,0.4f,1.0f };	//�P�D��
	static GLfloat diffuseLight[] = { 0.5f,0.5f,0.5f,1.0f };	//���g��
	static GLfloat specular[] = { 0.8f,0.8f,0.8f,1.0f };		//�Ϯg��

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glEnable(GL_COLOR_MATERIAL);	// ����
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	// ��m�l��
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);		// ���A���M��L�`����V�@�_
	glMateriali(GL_FRONT, GL_SHININESS, 100);			// �o������
}

/* ���J�K�� */
void SetupTexture() {
	/* ������ */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// ���J bmp
	Texture = TextureLoad(Tex[skin], GL_FALSE, GL_LINEAR, GL_LINEAR, GL_REPEAT);
}

/* ���J�K�Ϥ��esample */
GLuint SetupTexture(int k) {
	GLuint Texture_s;
	Texture_s = TextureLoad(Tex[k], GL_FALSE, GL_LINEAR, GL_LINEAR, GL_REPEAT);
	return Texture_s;
}

/* ���ܤj�p�� */
void ChangeSize(int w, int h) {
	GLfloat nRange = 150;
	if (h == 0) { h = 1; }			//�קK�Q�Y��0
	window_width = w; window_height = h;
	w_bigger_than_h = window_width > window_height;
	ratio = (!w_bigger_than_h) *(GLfloat(window_height) / GLfloat(window_width))
		+ (w_bigger_than_h)*(GLfloat(window_width) / GLfloat(window_height));
	glViewport(0, 0, w, h);		//�]�w��v��m
	glMatrixMode(GL_PROJECTION); glLoadIdentity();

	if (w_bigger_than_h) { glOrtho(-nRange*ratio, nRange * ratio, -nRange, nRange, -3 * nRange, 3 * nRange); }//�p�G�O�e��
	else { glOrtho(-nRange, nRange, -nRange*ratio, nRange*ratio, -3 * nRange, 3 * nRange); }// �p�G�O����

	Font_big = FontCreate(wglGetCurrentDC(), FontName, h / 12, 0, 0);	// ���]�r��j�p
	Font_small = FontCreate(wglGetCurrentDC(), FontName, h / 24, 0, 0);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

/* ���ƹ� */
void Mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		ProcessSelection(x, y);
	}
}

/* �۰ʧ�s */
void update(int value) {
	t += dt;
	glutPostRedisplay();
	glutTimerFunc(30, update, 0);// ����30�@��A�s�@��update
}

/* ���ռҦ�
void testing_index(const char*variable, double value) {
system("clc");
cout << "oscillation:" << 0 << std::endl;
cout << "" << 0 << std::endl;
cout << "" << 0 << std::endl;
cout << "" << 0 << std::endl;
cout << "" << 0 << std::endl;
cout << "" << 0 << std::endl;
cout << "" << 0 << std::endl;
cout << "" << 0 << std::endl;
cout << "" << 0 << std::endl;
cout << "" << 0 << std::endl;
}
*/

/* ���ռҦ� */
void testing_index() {
	cout << "show_Falling:\t" << show_Falling << "\n";
	cout << "CubRotate:\t" << show_CubRotate << "\n";
	cout << "SlowDown:\t" << show_SlowDown << "\n";
	cout << "2OrderSys:\t" << show_2OrderSys << "\n";
	cout << "Buttom:\t" << show_Buttom << "\n\n";
}

/*=== Ū�s�� ===*/
bool GLASS = 0;
bool MIRROR = 0;
bool THERMODYNAMIC = 1;
bool METERIAL = 1;
bool FLUID = 1;
bool ENGMATH = 1;
bool ADVENGMATH = 1;
bool MECHANISM = 1;
bool EE = 1;
bool ED = 1;
bool MANUFACTURE = 1;
bool DESIGN = 1;
bool CONTROL = 1;
bool CPP = 1;
bool CG = 1;

/* �p��������C */
void calculate_record() {
	THERMODYNAMIC = METERIAL = FLUID = ENGMATH = ADVENGMATH = MECHANISM = EE = ED = MANUFACTURE = DESIGN = CONTROL = CPP = CG = 0;
	int max_ = max_score / 5; if (max_ > 11) { max_ = 11; }
	switch (max_) { // �G�N����break;
	case 11: CG = 1;
	case 10: CPP = 1;
	case 9:  CONTROL = 1;
	case 8:  DESIGN = 1;
	case 7:  MANUFACTURE = 1;
	case 6:  ED = 1;
	case 5:  EE = 1;
	case 4:  MECHANISM = 1;
	// default: break;
	}
	int cre_ = (credit - 15) / 5; if (cre_ > 5) { cre_ = 5; }
	switch (cre_) { // �G�N����break;
	case 5: ADVENGMATH = 1;
	case 4: ENGMATH = 1;
	case 3: FLUID = 1;
	case 2: METERIAL = 1;
	case 1: THERMODYNAMIC = 1;
	}
	records[STACK_TEXTURE_THERMODYNAMIC] = THERMODYNAMIC;
	records[STACK_TEXTURE_METERIAL] = METERIAL;
	records[STACK_TEXTURE_FLUID] = FLUID;
	records[STACK_TEXTURE_ENGMATH] = ENGMATH;
	records[STACK_TEXTURE_ADVENGMATH] = ADVENGMATH;
	records[STACK_TEXTURE_MECHANISM] = MECHANISM;
	records[STACK_TEXTURE_EE] = EE;
	records[STACK_TEXTURE_ED] = ED;
	records[STACK_TEXTURE_MANUFACTURE] = MANUFACTURE;
	records[STACK_TEXTURE_DESIGN] = DESIGN;
	records[STACK_TEXTURE_CONTROL] = CONTROL;
	records[STACK_TEXTURE_CPP] = CPP;
	records[STACK_TEXTURE_CG] = CG;

}

/* �R������ */
void delete_record() {
	max_score = 0;
	credit = 1;		// �񷹥x�n�C
	save_record();
	load_record();
}

/* �s���� */
void save_record() {
	calculate_record();
	std::ofstream of("../save/save.txt");
	of << "Max: " << max_score << "\n";					// record[0]
	of << "Credit: " << credit << "\n";					// 1
	of << "GLASS: " << 0 << "\n";
	of << "MIRROR: " << 0 << "\n";
	of << "RAMDOM: " << 1 << "\n";
	of << "STATIC: " << 1 << "\n";
	of << "DYNAMIC: " << 1 << "\n";
	of << "THERMODYNAMIC: " << THERMODYNAMIC << "\n";	// record[STACK_TEXTURE_THERMODYNAMIC]
	of << "METERIAL: " << METERIAL << "\n";
	of << "FLUID: " << FLUID << "\n";
	of << "ENGMATH: " << ENGMATH << "\n";
	of << "ADVENGMATH: " << ADVENGMATH << "\n";
	of << "MECHANISM: " << MECHANISM << "\n";
	of << "EE: " << EE << "\n";
	of << "ED: " << ED << "\n";
	of << "MANUFACTURE: " << MANUFACTURE << "\n";
	of << "DESIGN: " << DESIGN << "\n";
	of << "CONTROL: " << CONTROL << "\n";
	of << "CPP: " << CPP << "\n";
	of << "CG: " << CG << "\n";
	of.close();
	return;
}

/* ���J���� */
void load_record() {
	records.clear();
	std::ifstream ifs("../save/save.txt", std::ios::in);
	if (!ifs) { save_record(); }
	std::string junk;
	int record;
	while (ifs >> junk >> record) {
		records.push_back(record);
	}
	ifs.close();
	max_score = records[0];
	credit = records[1];
	THERMODYNAMIC = records[STACK_TEXTURE_THERMODYNAMIC];
	METERIAL = records[STACK_TEXTURE_METERIAL];
	FLUID = records[STACK_TEXTURE_FLUID];
	ENGMATH = records[STACK_TEXTURE_ENGMATH];
	ADVENGMATH = records[STACK_TEXTURE_ADVENGMATH];
	MECHANISM = records[STACK_TEXTURE_MECHANISM];
	EE = records[STACK_TEXTURE_EE];
	ED = records[STACK_TEXTURE_ED];
	MANUFACTURE = records[STACK_TEXTURE_MANUFACTURE];
	DESIGN = records[STACK_TEXTURE_DESIGN];
	CONTROL = records[STACK_TEXTURE_CONTROL];
	CPP = records[STACK_TEXTURE_CPP];
	CG = records[STACK_TEXTURE_CG];
	calculate_record();
	return;
}
