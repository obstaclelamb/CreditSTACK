// E14092013陳艾揚
#pragma once
class CUBOID;
// 繞著x軸轉動量
#define xRot 45.0f
// 繞著y軸轉動量
#define yRot 135.0f
// glut callback function
void SpecialKeys(int, int, int);
void NormalKeys(unsigned char, int, int);
void RenderScene(void);
void SetupRC();
void ChangeSize(int, int);
void Mouse(int, int, int, int);
void update(int);
void ProcessSelection(int, int);

// render sub-function
void CalCuboidCenter();
void CalCuboidVertex(float(*F)[3], float*C, float X, float Z);
void CutTheCuboid();
void renderCuboid();
void renderCuboidList();
void ColorCircle();
void SetupCubioVertex();
void renderFirstCub(int);
void renderCuboidEdge(float(*C)[3]);
void renderCuboidFaceAll(float(*F)[3]);
void renderCuboidFace_No_y(float(*F)[3]);
void renderCuboidFace_Texture_y(float(*F)[3]);
void renderCuboidFace_Texture_y(CUBOID &C);
void SpaceBar();

// 測試1、遊玩0
#if 0
#define SHOW_EVERYTHING FALSE
#else
#define SHOW_EVERYTHING TRUE
#endif
// 鍵盤操作項
#define KEY_EXIT 'e'
// 鍵盤測試項
#define KEY_SCENE_START 'a'
#define KEY_SCENE_PLAY 's'
#define KEY__SCENE_OVER 'd'
#define KEY_SELECT_1 'f'
#define KEY_SELECT_2 'g'

#define KEY_SAVE_RECORD 'q'
#define KEY_LOAD_RECORD 'w'

#define KEY_TEXTURE_PURE 'i'
#define KEY_TEXTURE_SKELE  'o'
#define KEY_TEXTURE_RANDOM 'p'

#define KEY_TEXTURE_NCKU  't'
#define KEY_TEXTURE_NCKUME 'l'
#define KEY_TEXTURE_NCKUMEVR 'r'

#define KEY_COLOR_RESTART 'z'
#define KEY_COLOR_NEXTSTEP 'x'
#define KEY_SHOW_ALL 'y'
#define KEY_SHOW_FALLING 'h'
#define KEY_SHOW_fall_ROTATE 'j'
#define KEY_SHOW_SLOW_FALL 'v'
#define KEY_SHOW_BUTTOM 'b'
#define KEY_SHOW_BOUNCING 'm'

void testing_index();
// void testing_index(const char*, double);
extern bool show_CubRotate;
// 方塊參數
#define X_OSCILLATION TRUE
#define Z_OSCILLATION FALSE
#define STACK_CUBOID_INITIAL_WIDTH 75.0f
void SetupParameter();
float transiet_respond(float t, float Ts, float PO);

// 動畫
#define RESET_TIME TRUE
#define COUNT_DOWN FALSE

// 場景
#define STACK_PLAY 1
#define STACK_START 2
#define STACK_GAME_OVER 3
#define STACK_TEXTURE_SELECTION_PAGE_1 4
#define STACK_TEXTURE_SELECTION_PAGE_2 5
void renderBackground();
void stack_playing_scene();
void stack_initial_scene(bool/*第一次呼叫*/);
void stack_gameover_scene(bool);
void stack_texture_scene();
void renderTapToSTART();

// 點擊影像
void stack_playing_buttom();
void stack_initial_buttom();
void stack_gameover_buttom();
void stack_texture_buttom();


// 點擊回傳
#define BUFFER_LENGTH 64
void stack_playing_feedback();
void stack_initial_feedback();
void stack_gameover_feedback();
void stack_texture_feedback();
void Translatef_Scalef(float, float, float);
void TextureSolidCube(float, int);

// 貼圖選項
#define STACK_TEXTURE_PURE_COLOR 0
#define STACK_TEXTURE_SKELETON 1
#define STACK_TEXTURE_GLASS 2	// 電腦能力不足
#define STACK_TEXTURE_MIRROR 3	// 自身能力不足
#define STACK_TEXTURE_RAMDOM 4
#define STACK_TEXTURE_STATIC 5
#define STACK_TEXTURE_DYNAMIC 6
#define STACK_TEXTURE_THERMODYNAMIC 7
#define STACK_TEXTURE_METERIAL 8
#define STACK_TEXTURE_FLUID 9
#define STACK_TEXTURE_ENGMATH 10
#define STACK_TEXTURE_ADVENGMATH 11
#define STACK_TEXTURE_MECHANISM 12
#define STACK_TEXTURE_EE 13
#define STACK_TEXTURE_ED 14
#define STACK_TEXTURE_MANUFACTURE 15
#define STACK_TEXTURE_DESIGN 16
#define STACK_TEXTURE_CONTROL 17
#define STACK_TEXTURE_CPP 18
#define STACK_TEXTURE_CG 19
#define STACK_TEXTURE_NCKU 20
#define STACK_TEXTURE_NCKUME 21
#define STACK_TEXTURE_NCKUMEVR 22
#define SAMPLE_SIZE 0.4f
void SetupTexture();
GLuint SetupTexture(int);
void SetupLight();
void calculate_record();
/* 貼圖位置 */
static char *Tex[] = {
	"../TEXTURE_bmp/0_PURECOLOR.bmp",
	"../TEXTURE_bmp/1_SKELETON.bmp",
	"../TEXTURE_bmp/2_GLASS.bmp",	// don't
	"../TEXTURE_bmp/3_MIRROR.bmp",	// don't
	"../TEXTURE_bmp/4_RANDOM.bmp",
	"../TEXTURE_bmp/5_STATIC.bmp",
	"../TEXTURE_bmp/6_DYNAMIC.bmp",
	"../TEXTURE_bmp/7_THERMODYNAMIC.bmp",
	"../TEXTURE_bmp/8_MECHANICS OF METERIAL.bmp",
	"../TEXTURE_bmp/9_FLUID DYNAMIC.bmp",
	"../TEXTURE_bmp/10_ENGMATH.bmp",
	"../TEXTURE_bmp/11_ADV_ENGMATH.bmp",
	"../TEXTURE_bmp/12_MECHANISM.bmp",
	"../TEXTURE_bmp/13_EE.bmp",
	"../TEXTURE_bmp/14_ELECTRONIC DEVICES.bmp",
	"../TEXTURE_bmp/15_MANUFACTURE.bmp",
	"../TEXTURE_bmp/16_DESIGN.bmp",
	"../TEXTURE_bmp/17_CONTROL.bmp",
	"../TEXTURE_bmp/18_COMPUTER.bmp",
	"../TEXTURE_bmp/19_CG.bmp",
	"../TEXTURE_bmp/NCKU.bmp",
	"../TEXTURE_bmp/NCKUME.bmp",
	"../TEXTURE_bmp/NCKUMEVR.bmp"
};

// 存讀檔案
void save_record();
void load_record();
void delete_record();

