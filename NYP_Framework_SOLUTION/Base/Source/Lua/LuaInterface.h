#pragma once

// includes the lua headers
#include <lua.hpp>
#include <string>

#include "Vector3.h"

class CLuaInterface
{
protected:
	static CLuaInterface *s_instance;
	CLuaInterface();

public:
	static CLuaInterface *GetInstance()
	{
		if (!s_instance)
			s_instance = new CLuaInterface;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			// Drop the Lua Interface Class
			s_instance->Drop();

			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	virtual ~CLuaInterface();

	// Initialisation of the Lua Interface Class
	bool Init(const std::string _filename = "Image//DM2240.lua");

	// Run the Lua Interface Class
	void Run();

	// Drop the Lua Interface Class
	void Drop();

	// Get an integer value through the Lua Interface Class
	int getIntValue(const char* varName);
	// Get a float value through the Lua Interface Class
	float getFloatValue(const char* varName);
	// Get a char value through the Lua Interface Class
	char getCharValue(const char* varName);
	// Get a char value through the Lua Interface Class
	Vector3 getVec3Value(const char* varName);
	// Get distance square vlaue
	float getDistanceSquare(const char* varName, Vector3 src, Vector3 dest);
	// Get variable number of values
	int getVariableVal(const char* varName, int &a, int &b, int &c, int &d);

	// Save an integer value through the Lua Interface Class
	void saveIntValue(const char* varName, const int value, const bool bOverwrite = NULL);
	// Save a float value through the Lua Interface Class
	void saveFloatValue(const char* varName, const float value, const bool bOverwrite = NULL);

	// Pointer to the Lua State
	lua_State *theLuaState;

	// Key to move forward
	char keyFORWARD;
};

