#include "CharacterManager.h"
#include "Manager.h"

CharacterManagerPtr CharacterManager::getTask( ) {
	return std::dynamic_pointer_cast< CharacterManager >( Manager::getInstance( )->getTask( getTag( ) ) );
}

std::string CharacterManager::getTag( ) {
	return "CHARACTER_MANAGER";
}

CharacterManager::CharacterManager( ) {
}

CharacterManager::~CharacterManager( ) {
}

void CharacterManager::initialize( ) {
}

void CharacterManager::finalize( ) {
}

void CharacterManager::update( ) {
}