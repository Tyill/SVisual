
#pragma once

#include <vector>
#include "auxFunc.h"

namespace SV_Aux {

/// Таймер задержки однопоточный
    class TimerDelay {

    public:

        TimerDelay() {

            prevCycTm = CurrDateTimeSinceEpochMs();
            cycleTm = 0;

            tmSz = 0;
        }

// обновить тек время цикла
        void UpdateCycTime() {

            uint64_t ct = CurrDateTimeSinceEpochMs();
            cycleTm = ct - prevCycTm;
            prevCycTm = ct;

            // посмотрим, использовались ли таймеры в пред цикле
            // если нет, обнуляем таймеры
            for (int i = 0; i < tmSz; i++) {

                if (!tmrs[i].tmActiv) tmrs[i].tmCnt = 0;

                // сброс активности таймера для след цикла
                tmrs[i].tmActiv = false;
            }

            // импульсы времени
            time_t t = time(nullptr);
            tm* lct = localtime(&t);
            hourOnc_ = (lct->tm_hour != prevTm.tm_hour);
            minOnc_ = (lct->tm_min != prevTm.tm_min);
            secOnc_ = (lct->tm_sec != prevTm.tm_sec);
            prevTm = *lct;
        }

// таймер ondelay сек
        bool OnDelTmSec(bool start, int delay, int id) {

            bool res = false;

            if (id >= tmSz){
                tmrs.resize(id + 1, TmBase{0, false});
                tmSz = id + 1;
            }

            if (start) {
                tmrs[id].tmCnt += cycleTm;

                if (tmrs[id].tmCnt >= delay * 1000) res = true;
            } else tmrs[id].tmCnt = 0;

            // таймер активен в тек цикле
            tmrs[id].tmActiv = true;

            return res;
        }

// таймер offdelay сек
        bool OffDelTmSec(bool start, int delay, int id) {

            bool res = false;

            if (id >= tmSz){
                tmrs.resize(id + 1, TmBase{0, false});
                tmSz = id + 1;
            }

            if (start) tmrs[id].tmCnt = delay * 1000;
            else {

                if (tmrs[id].tmCnt > 0) {
                    res = true;
                    tmrs[id].tmCnt -= cycleTm;
                }
            }

            // таймер активен в тек цикле
            tmrs[id].tmActiv = true;

            return (start || res);
        }

// таймер ondelay мс
        bool OnDelTmMS(bool start, int delay, int id) {

            bool res = false;

            if (id >= tmSz){
                tmrs.resize(id + 1, TmBase{0, false});
                tmSz = id + 1;
            }

            if (start) {
                tmrs[id].tmCnt += cycleTm;

                if (tmrs[id].tmCnt >= delay) res = true;
            } else tmrs[id].tmCnt = 0;

            // таймер активен в тек цикле
            tmrs[id].tmActiv = true;

            return res;
        }

// таймер offdelay мс
        bool OffDelTmMS(bool start, int delay, int id) {

            bool res = false;

            if (id >= tmSz){
                tmrs.resize(id + 1, TmBase{0, false});
                tmSz = id + 1;
            }

            if (start) tmrs[id].tmCnt = delay;
            else {

                if (tmrs[id].tmCnt > 0) {
                    res = true;
                    tmrs[id].tmCnt -= cycleTm;
                }
            }

            // таймер активен в тек цикле
            tmrs[id].tmActiv = true;

            return (start || res);
        }

        // секундный импульс
        bool SecOnc() {

            return secOnc_;
        }

        // минутный импульс
        bool MinOnc() {

            return minOnc_;

        }

        // часовой импульс
        bool HourOnc() {

            return hourOnc_;
        }

    private:

        struct TmBase{
            uint64_t tmCnt;						// счетчик времени
            bool tmActiv;						// таймер был активен в тек цикле
        };

        std::vector<TmBase> tmrs; int tmSz;		// общий список запущенных таймеров

        uint64_t prevCycTm;						// время в пред цикле, мс
        uint64_t cycleTm;						// время цикла цпу, мс

        tm prevTm;

        bool secOnc_;							// секундный испульс
        bool minOnc_;							// минутный испульс
        bool hourOnc_;							// часовой импульс
    };
}