#include "CharaDummy.h"
#include "Drawer.h"
#include "const.h"
#include "Debug.h"
#include "Infomation.h"

const int MOVE_RATE_X = 3;
const int MOVE_RATE_Y = BLOCK_SIZE;

CharaDummy::CharaDummy( MapPtr map, InfomationPtr info ) :
Character( map, info->getInfo( CHARA_DUMMY ) ) {
	_debug = Debug::getTask( );

	addAnim( Character::WALK, "CharaDummy_Walk", 2 );
	addAnim( Character::OPEN, "CharaDummy_Open", 2 );
	setAnim( Character::WALK );

	_dir = MOVE_RIGHT;
	_ride_elevator = false;
	setDistance( );
}

CharaDummy::~CharaDummy( ) {
}

void CharaDummy::update( ) {
	_dir = MOVE_RIGHT;
	walk( );
	fall( );
	countLooking( );
	checkCaughtCollider( );

	if ( _debug->isDebug( ) ) {
		_drawer->drawString( 10, 100, "�Ď�����" + std::to_string( _looking_time / FPS ) + "�b", RED );
	}
}

void CharaDummy::walk( ) {
	if ( _dir == MOVE_DOWN ) {
		return;
	}
	// �i�s�����ɗ\�����W���Z�b�g
	setDistance( );

	checkCollider( );
	checkElevator( );
}

void CharaDummy::fall( ) {
	_dir = MOVE_DOWN;

	// �i�s�����ɗ\�����W���Z�b�g
	setDistance( );
	bool move_ok = true;
	
	int data = getMapDataCollider( getPos( ) + _distance );
	if ( data < 0 ) {
		return;
	}
	if ( data != IDENTIFICATION_NONE ) {
		move_ok = false;
	}

	if ( !move_ok ) {
		return;
	}

	// �ړ�������
	setFallPos( getPos( ) + _distance );
}

void CharaDummy::setDistance( ) {
	switch ( _dir ) {
	case MOVE_RIGHT: _distance = Vector(  MOVE_RATE_X, 0 ); break;
	case MOVE_LEFT : _distance = Vector( -MOVE_RATE_X, 0 ); break;
	case MOVE_DOWN : _distance = Vector(  0, MOVE_RATE_Y ); break;
	}
}

void CharaDummy::countLooking( ) {
	if ( isLooking( getPos( ) ) ) {
		_looking_time++;
	}
}

void CharaDummy::checkCollider( ) {
	bool move_ok = true;

	// �A�X�L�[�R�[�h���擾
	int data = getMapDataCollider( getPos( ) + _distance );

	// �G���x�[�^�[��������
	if ( data < 0 ) {
		return;
	}

	// �i�s�\���l�������蔻��ł�������
	if ( data == IDENTIFICATION_COLLIDER ) {
		// ����1�������
		data = getMapDataCollider( getPos( ) + _distance + Vector( 0, -BLOCK_SIZE ) );
		if ( data == IDENTIFICATION_COLLIDER ) {
			// 2��ڂ������蔻�肪��������
			move_ok = false;
		} else {
			_distance += Vector( 0, -BLOCK_SIZE );
		}
	}

	if ( !move_ok ) {
		return;
	}

	// �ړ�������	
	move( _distance );
}

void CharaDummy::checkElevator( ) {
	// �A�X�L�[�R�[�h���擾
	int data = getMapDataElevator( getPos( ) );

	// �G���x�[�^�[�ȊO��������
	if ( data < 0 ) {
		_ride_elevator = false;
		return;
	}

	// �A���ŏ��̂�h��
	if ( _ride_elevator ) {
		return;
	}

	setElevatorPos( data );
	_ride_elevator = true;
}