//=============================================================================
// SVisual client for MK Arduino
//=============================================================================

#pragma once

#define SV_NAMESZ 24

namespace svisual{

enum class valueType{
	tBool = 0,
	tInt = 1,	
	tFloat = 2,
};

union value{
	bool tBool;
	int tInt;
    float tFloat;
};

struct valueRec{
	bool isActive;
	bool isOnlyFront;
	char* name;	
	valueType type;
	value vals[SV_PACKETSZ];
};

struct dataRef{
	char name[SV_NAMESZ];
	long int type;  // tBool = 0, tInt = 1, tFloat = 2
	value vals[SV_PACKETSZ];
};		
}