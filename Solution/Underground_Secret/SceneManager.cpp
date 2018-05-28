#include "SceneManager.h"
#include "Manager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneStageSelect.h"

SceneManagerPtr SceneManager::getTask( ) {
	return std::dynamic_pointer_cast< SceneManager >( Manager::getInstance( )->getTask( getTag( ) ) );
}

std::string SceneManager::getTag( ) {
	return "SCENE_MANAGER";
}

SceneManager::SceneManager( ) {
}

SceneManager::~SceneManager( ) {
}

void SceneManager::initialize( ) {
	_scene = SceneTitlePtr( new SceneTitle( ) );
}

void SceneManager::finalize( ) {
}

void SceneManager::update( ) {
	_scene->update( );

	changeScene( );
}

void SceneManager::changeScene( ) {
	if ( _scene->getNextScene( ) == CONTINUE ) {
		return;
	}

	switch ( _scene->getNextScene( ) ) {
	case TITLE:
		_scene = SceneTitlePtr( new SceneTitle( ) );
		break;

	case STAGESELECT:
		_scene = SceneStageSelectPtr( new SceneStageSelect( ) );
		break;

	case GAME:
		{
			SceneStageSelectPtr stage = std::dynamic_pointer_cast< SceneStageSelect >( _scene );
			_scene = SceneGamePtr( new SceneGame( stage->getStage( ) ) );
			break;
		}
	}
}
