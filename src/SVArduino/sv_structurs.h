//=============================================================================
// SVisual client for MK Arduino
//=============================================================================

#pragma once

#include SVClient.h

#define SV_NAMESZ 24

namespace svisual{

enum class valueType{
	tBool = 0,
	tInt = 1,	
	tFloat = 2,
};

union value
{
	bool tBool;
	int tInt;
#ifdef SV_FLOAT_ENA
    float tFloat;	
#endif
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
	long int vals[SV_PACKETSZ];
};		
}