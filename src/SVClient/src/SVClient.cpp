
#include "stdafx.h"
#include "SVClient/SVClient.h"
#include "client.h"

client clientObj;

namespace SV_Cl {

    bool sv_connect(const char *moduleName, const char *ipAddr, int port) {

        return clientObj.connect(moduleName, ipAddr, port);
    }

    void sv_disconnect() {

        clientObj.disconnect();
    }

    bool sv_addBoolValue(const char *name, bool value_in, bool onlyPosFront) {

        SV_Cng::value val;
        val.tBool = value_in;

        return clientObj.addValue(name, SV_Cng::valueType::tBool, val, onlyPosFront);
    }

    bool sv_addIntValue(const char *name, int value_in) {

        SV_Cng::value val;
        val.tInt = value_in;

        return clientObj.addValue(name, SV_Cng::valueType::tInt, val, false);
    }

    bool sv_addFloatValue(const char *name, float value_in) {

        SV_Cng::value val;
        val.tFloat = value_in;

        return clientObj.addValue(name, SV_Cng::valueType::tFloat, val, false);
    }

    bool sv_setParam(int cycleRecMs, int packetSz) {

        clientObj.setConfig(client::config(cycleRecMs, packetSz));

        return true;
    }
}