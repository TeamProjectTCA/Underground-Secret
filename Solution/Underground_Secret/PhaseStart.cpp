#include "PhaseStart.h"
#include "Character.h"
#include "Keyboard.h"
#include "Map.h"
#include "const.h"
#include "SpecialScroll.h"

const float MOVE_FRAME = FPS * 2;
const int PERFORMANCE_TIME = FPS * 2;

PhaseStart::PhaseStart( std::list< CharacterPtr > &chara, MapPtr map, SpecialScrollPtr scroll ) :
_chara( chara ),
_map( map ),
_count( 0 ),
_scroll( scroll ) {
	_keyboard = Keyboard::getTask( );

	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( ite; ite != _chara.end( ); ite++ ) {
		( *ite )->setFixedpoint( PHASE_START );
		( *ite )->setAnimTime( ANIM_WALK_FRAME );
	}

	_endpoint = _map->getFixedpointBeta( PHASE_START );
	_run_idx = 0;
	_run_chara = ( *_chara.begin( ) );

	const float SPEED = ( float )( _endpoint - _run_chara->getPos( ) ).getLength( ) * ( 1 / MOVE_FRAME );
	_move = ( _endpoint - _run_chara->getPos( ) ).normalize( ) * SPEED;

	Vector start_pos = _map->getFixedpointAlpha( PHASE_START );
	_scroll->setScroll( Vector( -start_pos.x, 0 ) );
}

PhaseStart::~PhaseStart( ) {
}

void PhaseStart::update( ) {
	// �S�L�����̉��o�I���őJ��
	if ( _run_idx >= ( int )_chara.size( ) ) {
		setPhase( PHASE_PLAY );
		return;
	}

	_count++;

	// �ړ�
	if ( _count < MOVE_FRAME ) {
		_run_chara->move( _move );
		_run_chara->setScroll( );
	}

	// �I�[�v���A�j���[�V�������Z�b�g
	if ( _count == MOVE_FRAME ) {
		_run_chara->setAnim( Character::ANIM_RIDE );
	}

	// �A�j���[�V�����I��
	if ( _count == MOVE_FRAME + PERFORMANCE_TIME ) {
		// �A�j���[�V�����ƈʒu��ݒ�
		_run_chara->setFixedpoint( PHASE_PLAY );
		_run_chara->setAnim( Character::ANIM_WALK );
		_run_chara->setAnimTime( ANIM_WALK_FRAME );

		// ���̉��o�L�����ɐ؂�ւ���
		_run_idx++;
		changeRunCharacter( );

		// �J�E���g�����Z�b�g
		_count = 0;
	}
}

void PhaseStart::draw( ) const {
	_run_chara->draw( );
}

void PhaseStart::changeRunCharacter( ) {
	if ( _run_idx >= ( int )_chara.size( ) ) {
		return;
	}

	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( int i = 0; i < _run_idx; i++ ) {
		ite++;
	}

	_run_chara = ( *ite );
}