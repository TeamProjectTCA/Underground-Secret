#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include <string>

PTR( EditorManager );
PTR( Keyboard );
PTR( ImageLoad );
PTR( Table );

class EditorManager : public Base {
private:
	enum COMMAND {
		COMMAND_IMAGE_LOAD,
		COMMAND_DATA_LOAD,
		COMMAND_DATA_EXPORT,
		COMMAND_DATA_SAVE,
		COMMAND_MAX,
		COMMAND_NONE,
	};

public:
	static EditorManagerPtr getTask( );
	static std::string getTag( );

public:
	EditorManager( );
	virtual ~EditorManager( );

public:
	void initialize( );
	void finalize( );
	void update( );

private:
	COMMAND _command;

	KeyboardPtr _keyboard;
	ImageLoadPtr _image_load;
	TablePtr _table;
};

