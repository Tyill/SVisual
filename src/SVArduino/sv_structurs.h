//=============================================================================
// SVisual client for MK Arduino
//=============================================================================

#pragma once

#define SV_CYCLEREC_MS 100 
#define SV_NAMESZ 24
#define SV_PACKETSZ 10
#define SV_VALS_MAX_CNT 128

namespace svisual{

enum class valueType{
	tBool = 0,
	tInt = 1,	
};

union value
{
	bool tBool;
	int tInt;	
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