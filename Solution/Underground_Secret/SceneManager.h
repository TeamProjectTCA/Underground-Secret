#pragma once
#include "Base.h"
#include <string>

PTR( SceneManager );
PTR( Scene );

class SceneManager : public Base {
public:
	static SceneManagerPtr getTask( );
	static std::string getTag( );

public:
	SceneManager( );
	virtual ~SceneManager( );

public:
	void initialize( );
	void finalize( );
	void update( );

private:
	void changeScene( );

private:
	ScenePtr _scene;
};

