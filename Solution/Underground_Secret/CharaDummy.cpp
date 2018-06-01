#include "CharaDummy.h"
#include "Drawer.h"
#include "const.h"
#include "Debug.h"
#include "Infomation.h"
#include "Random.h"

const int MOVE_RATE_X = 3;
const int MOVE_RATE_Y = BLOCK_SIZE;
const float RANDOM_PROBABILITY = 0.3f;
const int INFO_SHOWTIME = FPS * 5;

CharaDummy::CharaDummy( MapPtr map, InfomationPtr info ) :
Character( map, info->getInfo( CHARA_DUMMY ) ),
_ride_elevator( false ),
_return_move( false ),
_looking_time( 0 ) {
	_random = Random::getTask( );

	addAnim( Character::ANIM_WALK, "CharaDummy_Walk", 2 );
	addAnim( Character::ANIM_OPEN, "CharaDummy_Open", 2 );
	setAnim( Character::ANIM_WALK );

	_dir = ( _random->getInt32( 0, 1 ) ? MOVE_RIGHT : MOVE_LEFT );

	setDistance( );
}

CharaDummy::~CharaDummy( ) {
}

void CharaDummy::update( ) {
	setScroll( );
	walk( );
	fall( );
	countLooking( );
	checkCaughtCollider( );

	// ���Ԍo�߂ŏ���\��
	if ( _looking_time % INFO_SHOWTIME == 0 ) {
		addShowInfoNum( );
	}

	// debug
	if ( _debug->isDebug( ) ) {
		_debug->addLog( "�Ď�����:" + std::to_string( _looking_time / FPS ) + "�b" );
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
	MOVE_DIRECTION past = _dir;
	_dir = MOVE_DOWN;

	// �i�s�����ɗ\�����W���Z�b�g
	setDistance( );

	// �������ɐi�s������߂�
	_dir = past;

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
	// �A�X�L�[�R�[�h���擾
	int data = getMapDataCollider( getPos( ) + _distance );

	// �G���x�[�^�[��������
	if ( data < 0 ) {
		return;
	}

	// �i�s�\�z�l���V���b�^�[
	if ( data == IDENTIFICATION_SHUTTER ) {
		_return_move = true;
	}

	// �i�s�\���l�������蔻��ł�������
	if ( data == IDENTIFICATION_COLLIDER ) {
		// ����1�������
		data = getMapDataCollider( getPos( ) + _distance + Vector( 0, -BLOCK_SIZE ) );
		if ( data == IDENTIFICATION_COLLIDER ) {
			// 2��ڂ������蔻�肪��������
			_return_move = true;
		} else {
			_distance += Vector( 0, -BLOCK_SIZE );
		}
	}

	// �����]��
	if ( _return_move ) {
		returnMove( );
		setDistance( );
		_return_move = false;
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

	// ��邩���Ȃ����̒��I
	if ( _random->getRealOne( ) > RANDOM_PROBABILITY && !_ride_elevator ) {
		_ride_elevator = true;
	}

	// �A���ŏ��̂�h��
	if ( _ride_elevator ) {
		return;
	}

	setElevatorPos( data );
	_ride_elevator = true;
}

void CharaDummy::returnMove( ) {
	switch ( _dir ) {
	case MOVE_RIGHT: _dir = MOVE_LEFT ; break;
	case MOVE_LEFT : _dir = MOVE_RIGHT; break;
	default: return;
	}
}