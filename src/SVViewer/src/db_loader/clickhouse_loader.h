#pragma once

#include "SVBase/base.h"

#include <QObject>
#include <QMap>
#include <memory>

namespace clickhouse{
class Client;
}

class MainWin;
class QMainWindow;

class DbClickHouseLoader : public QObject
{
    Q_OBJECT
public:
    DbClickHouseLoader(QMainWindow* mainWin, QObject* parent = nullptr);

    bool loadSignalNames();

    bool loadSignalData(const QString& sname, const QDateTime& from, const QDateTime& to);

private:
    std::unique_ptr<clickhouse::Client> newClient()const;

    MainWin* m_mainWin{};

    QMap<int, SV_Base::Value*> m_signalValueBuff;

    QMap<int, QPair<qint64, qint64>> m_timeDiapMem;
};

