#include "LuaInterface.h"
#include <iostream>
using namespace std;

// Allocating and initializing CLuaInterface's static data member.  
// The pointer is allocated but not the object's constructor.
CLuaInterface *CLuaInterface::s_instance = 0;

CLuaInterface::CLuaInterface()
	: theLuaState(NULL)
{
}


CLuaInterface::~CLuaInterface()
{
}


// Initialisation of the Lua Interface Class
bool CLuaInterface::Init(const std::string _filename)
{
	bool result = false;

	// 1. Create lua state
	theLuaState = lua_open();

	if (theLuaState)
	{
		// 2. Load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		// 3. Load lua script
		luaL_dofile(theLuaState, _filename.c_str());

		result = true;
	}

	return result;
}

// Run the Lua Interface Class
void CLuaInterface::Run()
{
	if (theLuaState == NULL)
		return;

	// 4. Read the variables
	// lua_getglobal(lua_State*, const char*)
	lua_getglobal(theLuaState, "title");
	// extract value, index -1 as variable is already retrieved using lua_getglobal
	const char *title = lua_tostring(theLuaState, -1);

	lua_getglobal(theLuaState, "width");
	int width = lua_tointeger(theLuaState, -1);

	lua_getglobal(theLuaState, "height");
	int height = lua_tointeger(theLuaState, -1);

	// Display on screen
	cout << title << endl;
	cout << "---------------------------------------" << endl;
	cout << "Width of screen : " << width << endl;
	cout << "Height of screen : " << height << endl;

	lua_getglobal(theLuaState, "keyFORWARD");

	size_t len;
	const char* cstr = lua_tolstring(theLuaState, -1, &len);
	keyFORWARD = cstr[0];
	//std::string str(cstr, len);
}

// Get an integer value through the Lua Interface Class
int CLuaInterface::getIntValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return lua_tointeger(theLuaState, -1);
}
// Get a float value through the Lua Interface Class
float CLuaInterface::getFloatValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return (float)lua_tonumber(theLuaState, -1);
}

char CLuaInterface::getCharValue(const char * varName)
{
	lua_getglobal(theLuaState, varName);

	size_t len;
	const char* cstr = lua_tolstring(theLuaState, -1, &len);

	//Not empty, return value
	if (len == 1)
	{
		return cstr[0];
	}
	//More than 1 char, return first char
	else if (len > 1)
	{
		return cstr[0];
	}
	//Cannot read/Nothing, return default value
	else
	{
		return ' ';
	}

	return 0;
}

Vector3 CLuaInterface::getVec3Value(const char * varName)
{
	lua_getglobal(theLuaState, varName);

	lua_rawgeti(theLuaState, -1, 1);
	int x = lua_tonumber(theLuaState, lua_gettop(theLuaState));
	lua_pop(theLuaState, 1);

	lua_rawgeti(theLuaState, -1, 2);
	int y = lua_tonumber(theLuaState, lua_gettop(theLuaState));
	lua_pop(theLuaState, 1);
	
	lua_rawgeti(theLuaState, -1, 3);
	int z = lua_tonumber(theLuaState, lua_gettop(theLuaState));
	lua_pop(theLuaState, 1);

	return Vector3(x, y, z);
}

float CLuaInterface::getDistanceSquare(const char * varName, Vector3 src, Vector3 dest)
{
	lua_getglobal(theLuaState, varName);

	lua_pushnumber(theLuaState, src.x);
	lua_pushnumber(theLuaState, src.y);
	lua_pushnumber(theLuaState, src.z);
	lua_pushnumber(theLuaState, dest.x);
	lua_pushnumber(theLuaState, dest.y);
	lua_pushnumber(theLuaState, dest.z);

	lua_call(theLuaState, 6, 1);

	float distSq = (float)lua_tonumber(theLuaState, 
		lua_gettop(theLuaState));

	lua_pop(theLuaState, 1);

	return distSq;
}

int CLuaInterface::getVariableVal(const char * varName, int & min, int & max, int & ave, int & n)
{
	lua_getglobal(theLuaState, varName);

	//Hardcoded values
	lua_pushnumber(theLuaState, 1000);
	lua_pushnumber(theLuaState, 2000);
	lua_pushnumber(theLuaState, 3000);
	lua_pushnumber(theLuaState, 4000);

	lua_call(theLuaState, 4, 4);
	
	//Get the results
	min = lua_tonumber(theLuaState, lua_gettop(theLuaState));
	lua_pop(theLuaState, 1);

	max = lua_tonumber(theLuaState, lua_gettop(theLuaState));
	lua_pop(theLuaState, 1);

	ave = lua_tonumber(theLuaState, lua_gettop(theLuaState));
	lua_pop(theLuaState, 1);

	n = lua_tonumber(theLuaState, lua_gettop(theLuaState));
	lua_pop(theLuaState, 1);

	std::cout << min << "," << max << "," << ave << "," << n << std::endl;

	return 1;
}

// Save an integer value through the Lua Interface Class
void CLuaInterface::saveIntValue(const char* varName, 
								const int value, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	//sprintf(outputString, "%s = %d\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0); cout << "....................";
}
// Save a float value through the Lua Interface Class
void CLuaInterface::saveFloatValue(const char* varName, 
								const float value, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	//sprintf(outputString, "%s = %6.4f\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
}

// Drop the Lua Interface Class
void CLuaInterface::Drop()
{
	if (theLuaState)
	{
		// Close lua state
		lua_close(theLuaState);
	}
}
