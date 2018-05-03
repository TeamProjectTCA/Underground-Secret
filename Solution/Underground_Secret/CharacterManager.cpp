#include "CharacterManager.h"
#include "Manager.h"
#include "CharaA.h"

CharacterManager::CharacterManager( ) {
	_charaA = CharaAPtr( new CharaA( ) );
}

CharacterManager::~CharacterManager( ) {
}

void CharacterManager::initialize( ) {
}

void CharacterManager::finalize( ) {
}

void CharacterManager::update( ) {
	_charaA->update( );
}