#pragma once

//スクリーンサイズ
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
//解像度
const int WIDTH = 1280;
const int HEIGHT = 720;
const float WIDTH_F = ( float )WIDTH;
const float HEIGHT_F = ( float )HEIGHT;

const double PI = 3.14; 
const int BLOCK_SIZE = 16;

enum BUTTON_STATE {
	NORMAL,
	CLICKING,
	BUTTON_STATE_MAX
};