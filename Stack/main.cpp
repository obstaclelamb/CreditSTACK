// E14092013���㴭
//==========   ���a�覡   ==========
//�ť�	�C�����|�B����->�_�l�B���->�_�l
//e	�j���(�|�x�s����)
//
//a	����:�_�l
//f	����:���1
//g	����:���2
//
//����	�_�l->���1->���2->�_�l
//����	�_�l->���2->���1->�_�l
//
//========== �K�ϸ������ ==========
// �C�|10�h credit + 1
// �ä[�ϥ�: PURE COLOR
//			 SKELETON
//			 RAMDOM
//			 STATIC
//			 DYNAMIC
// credit > 20	Thermodynamics
// credit > 25	Mechanics of materials
// credit > 30	Munson's fluid mechanics
// credit > 35	Advanced engineering mathmatics
// credit > 40	Complex analysis: A first course with Application 
// max > 20		Modern Mechanisms
// max > 25		Electrical Engineering
// max > 30		Electronic Devices
// max > 35		Manufacturing Engineering and Techology
// max > 40		Fundaments of Machine Component Design
// max > 45		Control Systems Engineering
// max > 50		Ivor Hortom's Beginning ANSI C++ (�p���A)
// max > 55		Computer Graphics and Geometric modeling for engineers
 
#include<Windows.h>
#include<gl\GLU.H>
#include<gl\GLAUX.H>
#include<gl\GLUT.H>
#include "render.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glut32.lib")
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(400,600);	// ������l�j�p
	glutInitWindowPosition(0, 0);	// ������l��m
	glutCreateWindow("STACK");		// �������D�Ӥw
	glutReshapeFunc(ChangeSize);	// �����j�p�ܤƮɰ��Ǥ���
	glutSpecialFunc(SpecialKeys);	// ����L�ɰ��Ǥ��� (�W�U���k)
	glutKeyboardFunc(NormalKeys);	// ����L�ɰ��Ǥ��� (�r��)
	glutMouseFunc(Mouse);			// �ƹ��I���ɰ��Ǥ���
	glutDisplayFunc(RenderScene);	// �e��
	SetupRC();						// ��l�ƩҦ�
	glutTimerFunc(30, update, 0);	// ���s�I�s
	glutMainLoop();					// �L������

	return 0;
}