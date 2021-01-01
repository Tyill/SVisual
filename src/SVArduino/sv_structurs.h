#pragma once

#define SV_NAMESZ 24

namespace svisual{

enum class ValueType{
  tBool = 0,
  tInt = 1,  
  tFloat = 2,
};

union Value{
  bool tBool;
  int tInt;
    float tFloat;
};

struct ValueRec{
  bool isActive;
  bool isOnlyFront;
  char* name;  
  ValueType type;
  Value vals[SV_PACKETSZ];
};

struct DataRef{
  char name[SV_NAMESZ];
  long int type;  // tBool = 0, tInt = 1, tFloat = 2
  Value vals[SV_PACKETSZ];
};    
}