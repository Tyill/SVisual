
#pragma once

class thrLoadData : public QObject {
	Q_OBJECT

public:
	thrLoadData(QStringList files);

signals:
	void finished(bool ok);
};