// E14092013陳艾揚
//==========   操縱方式   ==========
//空白	遊玩堆疊、結束->起始、選擇->起始
//e	強制結束(會儲存紀錄)
//
//a	場景:起始
//f	場景:選擇1
//g	場景:選擇2
//
//→↑	起始->選擇1->選擇2->起始
//←↓	起始->選擇2->選擇1->起始
//
//========== 貼圖解鎖條件 ==========
// 每疊10層 credit + 1
// 永久使用: PURE COLOR
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
// max > 50		Ivor Hortom's Beginning ANSI C++ (計概乙)
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
	glutInitWindowSize(400,600);	// 視窗初始大小
	glutInitWindowPosition(0, 0);	// 視窗初始位置
	glutCreateWindow("STACK");		// 視窗標題而已
	glutReshapeFunc(ChangeSize);	// 視窗大小變化時做些什麼
	glutSpecialFunc(SpecialKeys);	// 按鍵盤時做些什麼 (上下左右)
	glutKeyboardFunc(NormalKeys);	// 按鍵盤時做些什麼 (字母)
	glutMouseFunc(Mouse);			// 滑鼠點擊時做些什麼
	glutDisplayFunc(RenderScene);	// 畫圖
	SetupRC();						// 初始化所有
	glutTimerFunc(30, update, 0);	// 重新呼叫
	glutMainLoop();					// 無限重複

	return 0;
}