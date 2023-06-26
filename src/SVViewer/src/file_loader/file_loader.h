#pragma once

#include <QObject>

class MainWin;
class QMainWindow;

class FileLoader : public QObject
{
    Q_OBJECT
public:
    FileLoader(QMainWindow* mainWin, QObject* parent = nullptr);

    bool preloadFiles(const QStringList& files);
    bool loadSignalData(const QString& sign);

signals:
    void finished(bool ok);

private:
    MainWin* m_mainWin{};

    bool loadFile(const QString& path);
};

