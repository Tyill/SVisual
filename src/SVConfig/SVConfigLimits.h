
#pragma once

//#define SV_EN                         ///< eng интерфейс
#define SV_MODULE_MAX_CNT 8           ///< макс кол-во модулей
#define SV_VALUE_MAX_CNT 2048         ///< макс кол-во сигналов (на все модули)
#define SV_TRIGGER_MAX_CNT 256        ///< макс кол-во триггеров (на все модули)

#define SV_CYCLEREC_MS cng.cycleRecMs ///< период записи сигналов, мс
#define SV_NAMESZ 24                  ///< имя переменной
#define SV_COMMENTSZ 48               ///< комментарий
#define SV_PACKETSZ cng.packetSz      ///< пакет отправки - кол-во циклов записи
#define SV_CYCLESAVE_MS (cng.cycleRecMs * cng.packetSz)   ///< период отправки данных, мс






