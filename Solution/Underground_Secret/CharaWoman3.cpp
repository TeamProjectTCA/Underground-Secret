#include "CharaWoman3.h"
#include "Drawer.h"
#include "const.h"
#include "Debug.h"
#include "Infomation.h"
#include "Random.h"

const int MOVE_RATE_X = 4;
const int MOVE_RATE_Y = BLOCK_SIZE;
const int INFO_SHOWTIME = FPS * 5;
const int CHECK_WAIT_TIME = FPS * 3;
const float WAIT_PROBABILITY = 0.1f;
const float RIDE_PROBABILITY = 0.3f;
const float DOWN_SCALE = 0.5f;
const float HIT_SHUTTER_SCALE = 1.0f;
const int WAIT_TIME = 1;

CharaWoman3::CharaWoman3( MapPtr map, InfomationPtr info ) :
	Character( map, info->getInfo( CHARA_WOMAN3 ) ),
	_ride_elevator( false ),
	_ride_probability( RIDE_PROBABILITY ),
	_return_move( false ),
	_hit_shutter( false ),
	_elevator_down( false ),
	_judged_probability( false ),
	_looking_time( 0 ),
	_wait_count( 0 ),
	_wait_ani_time( 0 ) {
	_random = Random::getTask( );

	addAnim( Character::ANIM_WALK, "woman3_walk", 14 );
	addAnim( Character::ANIM_RIDE, "woman3_WaitElevator", 10 );
	addAnim( Character::ANIM_WAIT, "woman3_wait", 10 );
	addAnim( Character::ANIM_WAIT_ELEVATOR, "woman3_WaitElevator", 10 );
	setAnim( Character::ANIM_WALK );

	_dir = ( _random->getInt32( 0, 1 ) ? MOVE_RIGHT : MOVE_LEFT );

	setDistance( );
}

CharaWoman3::~CharaWoman3( ) {
}

void CharaWoman3::update( ) {
	setScroll( );

	if ( getAnimType( ) == Character::ANIM_WALK ) {
		walk( );
		fall( );
		checkCaughtCollider( );
	}

	if ( getAnimType( ) == Character::ANIM_WAIT ) {
		//�����~�܂�
		wait( );
	}

	// �G���x�[�^�[
	checkElevator( );

	// ���Ԍo�߂ŏ���\��
	countLooking( );
	if ( _looking_time % INFO_SHOWTIME == 0 ) {
		addShowInfoNum( );
	}

	// debug
	if ( _debug->isDebug( ) ) {
		_debug->addLog( "�Ď�����:" + std::to_string( _looking_time / FPS ) + "�b" );
	}
}

void CharaWoman3::walk( ) {
	if ( _dir == MOVE_DOWN ) {
		return;
	}

	//�����~�܂鏈��
	_wait_count++;
	if ( _wait_count >= CHECK_WAIT_TIME ) {
		if ( _random->getRealOne( ) <= WAIT_PROBABILITY ) {
			setAnim( Character::ANIM_WAIT );
		}
		_wait_count = 0;
	}

	// �i�s�����ɗ\�����W���Z�b�g
	setDistance( );

	checkCollider( );
}

void CharaWoman3::wait( ) {
	_wait_ani_time++;

	//2~4�b�o������Walk�ɖ߂�
	if ( _wait_ani_time >= WAIT_TIME * FPS ) {
		setAnim( Character::ANIM_WALK );
		_wait_ani_time = 0;
	}
}
void CharaWoman3::fall( ) {
	MOVE_DIRECTION past = _dir;
	_dir = MOVE_DOWN;

	// �i�s�����ɗ\�����W���Z�b�g
	setDistance( );

	// �������ɐi�s������߂�
	_dir = past;

	bool move_ok = true;

	int data = getMapData( getPos( ) + _distance );
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

void CharaWoman3::setDistance( ) {
	switch ( _dir ) {
		case MOVE_RIGHT: _distance = Vector( MOVE_RATE_X, 0 ); break;
		case MOVE_LEFT: _distance = Vector( -MOVE_RATE_X, 0 ); break;
		case MOVE_DOWN: _distance = Vector( 0, MOVE_RATE_Y ); break;
	}
}

void CharaWoman3::countLooking( ) {
	if ( isLooking( getPos( ) ) ) {
		_looking_time++;
	}
}

void CharaWoman3::checkCollider( ) {
	// �A�X�L�[�R�[�h���擾
	int data = getMapData( getPos( ) + _distance );

	// �G���x�[�^�[��������
	if ( data < 0 ) {
		return;
	}

	// �i�s�\�z�l���V���b�^�[
	if ( data == IDENTIFICATION_SHUTTER ) {
		// �����̂���ʒu���V���b�^�[�ł���ΐi�s����(���܂�̂����)
		if ( getMapData( getPos( ) ) != IDENTIFICATION_SHUTTER ) {
			_return_move = true;
			_hit_shutter = true;
		}
	}

	// �i�s�\���l�������蔻��ł�������
	if ( data == IDENTIFICATION_COLLIDER ) {
		// ����1�������
		data = getMapData( getPos( ) + _distance + Vector( 0, -BLOCK_SIZE ) );
		if ( data == IDENTIFICATION_COLLIDER ) {
			// 2��ڂ������蔻�肪��������
			_return_move = true;
			_hit_shutter = false;
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

void CharaWoman3::checkElevator( ) {
	// �A�X�L�[�R�[�h���擾
	char id = getElevatorId( );

	// �G���x�[�^�[�ȊO��������
	if ( id == 0x00 ) {
		_ride_elevator = false;
		_judged_probability = false;
		return;
	}

	// �A���ŏ��E���肷��̂�h��
	if ( _ride_elevator ) {
		return;
	}

	// �G���x�[�^�̏�Ԃ��擾
	ELEVATOR_STATE state = getElevatorState( );
	ELEVATOR_POS active_elevator = getActiveElevator( );
	ELEVATOR_POS destination = getDestination( );
	_draw_flag = true;

	switch ( state ) {
		case ELEVATOR_STATE_WAIT:
			if ( getAnimType( ) != Character::ANIM_WAIT_ELEVATOR ) {
				// �A���Ŕ��肷��̂�h��
				if ( _judged_probability ) {
					return;
				}

				if ( active_elevator == ELEVATOR_POS_UP ||
					( active_elevator == ELEVATOR_POS_CENTER && destination == ELEVATOR_POS_DOWN ) ) {
					_elevator_down = true;
				} else {
					_elevator_down = false;
				}

				//�G���x�[�^�[�ɏ��m������
				_ride_probability = RIDE_PROBABILITY;
				if ( _hit_shutter ) {
					_ride_probability += RIDE_PROBABILITY * HIT_SHUTTER_SCALE;
				}

				if ( isSpy( ) && _elevator_down ) {
					_ride_probability += RIDE_PROBABILITY * DOWN_SCALE;
				}

				_judged_probability = true;
				if ( _random->getRealOne( ) > _ride_probability ) {
					return;
				}

				setAnim( Character::ANIM_WAIT_ELEVATOR );
			}
			break;

		case ELEVATOR_STATE_COME:
		{
			Character::ANIM_TYPE anim = getAnimType( );
			if ( !isActiveElevatorPos( ) ) {
				break;
			}

			if ( anim != Character::ANIM_RIDE && anim == Character::ANIM_WAIT_ELEVATOR ) {
				setAnim( Character::ANIM_RIDE );
			}
			break;
		}

		case ELEVATOR_STATE_MOVE:
		{
			Character::ANIM_TYPE anim = getAnimType( );
			if ( anim == Character::ANIM_RIDE ) {
				_draw_flag = false;
			}
			break;
		}

		case ELEVATOR_STATE_ARRIVE:
			if ( getAnimType( ) != Character::ANIM_RIDE ) {
				break;
			}
			setElevatorPos( id );
			setAnim( Character::ANIM_WALK );
			_ride_elevator = true;
			break;

		default:
			break;
	}
}

void CharaWoman3::returnMove( ) {
	switch ( _dir ) {
		case MOVE_RIGHT: _dir = MOVE_LEFT; break;
		case MOVE_LEFT: _dir = MOVE_RIGHT; break;
		default: return;
	}
}

