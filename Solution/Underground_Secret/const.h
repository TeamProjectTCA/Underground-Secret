#pragma once

// スクリーンサイズ
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

// 解像度
const int WIDTH = 1280;
const int HEIGHT = 720;
const float WIDTH_F = ( float )WIDTH;
const float HEIGHT_F = ( float )HEIGHT;

// 色
const unsigned int RED     = 0xff0000;
const unsigned int GREEN   = 0x00ff00;
const unsigned int BLUE    = 0x0000ff;
const unsigned int YELLOW  = 0xffff00;
const unsigned int MAGENTA = 0xff00ff;
const unsigned int WATER   = 0x00ffff;
const unsigned int GRAY    = 0x3b3b3b;
const unsigned int WHITE   = 0xffffff;

// フレーム数
const int FPS = 60;
const int ANIM_WALK_FRAME = 2;

// 円周率
const double PI = 3.14; 

// マップブロックサイズ
const int BLOCK_SIZE = 16;
const int STAGE_MAX = 3;

// 情報の数
const int INFO_MAX = 7;

enum DIR {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_MAX
};

// .colデータと同じにすること
enum MAP_IDENTIFICATION {
	IDENTIFICATION_NONE,
	IDENTIFICATION_COLLIDER,
	IDENTIFICATION_SHUTTER,
	IDENTIFICATION_FIXEDPOINT_START,
	IDENTIFICATION_FIXEDPOINT_PLAY,
	IDENTIFICATION_FIXEDPOINT_END,
	IDENTIFICATION_ENDPOINT,
	MAP_IDENTIFICATION_MAX
};

enum CHARACTER {
	CHARA_A,
	CHARA_DUMMY,
	CHARA_MAX
};

enum SOUND {
//タイトル
	TITLE_BGM,
	BUTTON_SE,
//ゲーム中
	GAME_BGM,
	ELEVATOR_ARRIVE,
	BOARD_MOVE,
	ELEVATOR_COME_SE,
	ELEVATOR_ARRIVE_SE,
	WIN_SE,
	LOSE_SE,
	SOUND_MAX
};