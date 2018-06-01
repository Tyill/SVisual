//=============================================================================
// SVisual
// proj: SVClient - для отправки данных
// ver: 1.0.0.1
//=============================================================================

#pragma once

#ifdef _WIN32
    #ifdef SVCLDLL_EXPORTS
        #define SVCL_API extern "C" __declspec(dllexport)
    #else
        #define SVCL_API extern "C" __declspec(dllimport)
    #endif
#else
    #define SVCL_API extern "C"
#endif

namespace SV_Cl {

// connect of server
    // moduleName - module (leng max 24)
    // ipAddrServ - ip
    SVCL_API bool sv_connect(const char *moduleName, const char *ipAddrServ, int portServ);

// disconnect of server
    SVCL_API void sv_disconnect();

// add value for rec
// name - value name (leng max 24)
    SVCL_API bool sv_addBoolValue(const char *name, bool value, bool onlyFront = false);

// add value for rec
// name - value name (leng max 24)
    SVCL_API bool sv_addIntValue(const char *name, int value);

// add value for rec
// name - value name (leng max 24)
    SVCL_API bool sv_addFloatValue(const char *name, float value);

// frecuence rec
    SVCL_API bool sv_setParam(int cycleRecMs, int packetSz);
}