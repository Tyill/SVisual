#pragma once

#include <QObject>
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

    bool loadSignalData(const QDateTime& from, const QDateTime& to);

private:
    std::unique_ptr<clickhouse::Client> newClient()const;

    MainWin* m_mainWin{};
};

