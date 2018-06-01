
#pragma once

#include <string>
#include <vector>
#include <stdint.h>

namespace SV_Aux {

// тек дата %Y%m%d
    std::string CurrDateS();

// тек дата-время %Y%m%d_%H%M
    std::string CurrDateTimeEx();

// тек дата-время %Y-%m-%d %H:%M:%S
    std::string CurrDateTime();

    // тек дата-время %Y-%m-%d %H:%M:%S:%MS
    std::string CurrDateTimeMs();

    // тек дата-время %Y-%m-%d %H:%M:%S
    std::string CurrDateTimeSQL();

    uint64_t CurrDateTimeSinceEpochMs();

    int HourOffsFromUTC();

    std::vector<std::string> split(std::string str, const char *sep);

// автосоздание субдиректорий
    bool CreateSubDirectory(std::string strDirs);

    void SleepMs(int ms);
}