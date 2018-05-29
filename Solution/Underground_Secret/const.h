#pragma once

// �X�N���[���T�C�Y
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

// �𑜓x
const int WIDTH = 1280;
const int HEIGHT = 720;
const float WIDTH_F = ( float )WIDTH;
const float HEIGHT_F = ( float )HEIGHT;

// �F
const unsigned int RED     = 0xff0000;
const unsigned int GREEN   = 0x00ff00;
const unsigned int BLUE    = 0x0000ff;
const unsigned int YELLOW  = 0xffff00;
const unsigned int MAGENTA = 0xff00ff;
const unsigned int WATER   = 0x00ffff;
const unsigned int GRAY    = 0x3b3b3b;
const unsigned int WHITE   = 0xffffff;

// �t���[����
const int FPS = 60;

// �~����
const double PI = 3.14; 

// �}�b�v�u���b�N�T�C�Y
const int BLOCK_SIZE = 16;

// ���̐�
const int INFO_MAX = 7;

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

// .col�f�[�^�Ɠ����ɂ��邱��
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