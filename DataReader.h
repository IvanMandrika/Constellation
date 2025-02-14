#ifndef DATAREADER_H
#define DATAREADER_H

#include <QObject>
#include <QVector>
#include <QPointF>

class DataReader final : public QObject {
    Q_OBJECT
public:
    explicit DataReader(QObject *parent = nullptr);
    void setFilePath(const QString &path);
    public slots:
        void startReading();
    signals:
        void dataReady(const QVector<QPointF> &points);
private:
    QString filePath;
};

#endif // DATAREADER_H
