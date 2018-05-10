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
const unsigned int RED = 0xff0000;
const unsigned int GREEN = 0x00ff00;
const unsigned int BLUE = 0x0000ff;

// フレーム数
const int ONE_SECOND_FRAME = 60;

// 円周率
const double PI = 3.14; 

// マップブロックサイズ
const int BLOCK_SIZE = 16;

enum BUTTON_STATE {
	NORMAL,
	CLICKING,
	BUTTON_STATE_MAX
};

enum DIR {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_MAX
};

enum MAP_IDENTIFICATION {
	IDENTIFICATION_NONE,
	IDENTIFICATION_COLLIDER,
	IDENTIFICATION_SHUTTER,
	IDENTIFICATION_FIXEDPOINT_START,
	IDENTIFICATION_FIXEDPOINT_PLAY,
	IDENTIFICATION_FIXEDPOINT_END,
	MAP_IDENTIFICATION_MAX
};