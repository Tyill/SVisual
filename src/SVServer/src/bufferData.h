
#pragma once

#include "SVConfig/SVConfigData.h"
#include "SVConfig/SVConfigLimits.h"
#include <mutex>

/// циклический буфер данных
class bufferData
{
public:

    struct config{
        int cycleRecMs;           ///< период записи - задает пользователь
        int packetSz;             ///< размер пакета - задает пользователь

        config(int cycleRecMs_ = 100, int packetSz_ = 10) :cycleRecMs(cycleRecMs_), packetSz(packetSz_){}
    };

    bufferData(bufferData::config);

	/// входная переменная
	struct inputData{
		bool isActive = false;               ///< активна
		std::string name;            ///< имя
		std::string module;          ///< модуль
		SV_Cng::valueType type;      /// тип
        SV_Cng::recData data;        /// данные
	};

	/// обновить данные буфера
	/// \param in новые данные
	/// \param bTm отметка времени, мс
	/// \return true - ok
	void updDataSignals(const std::string& in, uint64_t bTm);

	/// вернуть данные по текущей позиции чтения
    /// \return
	inputData getDataByReadPos();

    /// инкремент позиции чтения
	void incReadPos();

    /// размер буфера
    /// \return
	int getBuffSize();

	private:

    /// запись
    struct valueData{
        char name[SV_NAMESZ];
        SV_Cng::valueType type;
        SV_Cng::value* vals;

        valueData() : vals(NULL){};
    };

    config cng;

    /// данные
    static const int buffSz_ = SV_VALUE_MAX_CNT * 10; // 10 сек - запас
    inputData buffer_[buffSz_];

    int buffReadPos_ = 0;  ///< тек позиция чтения
	int buffWritePos_ = 0; ///< тек позиция записи

	std::mutex mtx_;
};
