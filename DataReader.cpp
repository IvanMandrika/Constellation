#include "DataReader.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>

DataReader::DataReader(QObject *parent) : QObject(parent) {}

void DataReader::setFilePath(const QString &path) { filePath = path; }

void DataReader::startReading() {
  const QString filePath =
      QCoreApplication::applicationDirPath() + "/iq_qam32.bin";
  QFile file(filePath);

  if (!file.open(QIODevice::ReadOnly)) {
    qWarning() << "Error: cannot open bin file" << filePath;
    return;
  }

  QDataStream in(&file);
  in.setByteOrder(QDataStream::LittleEndian);

  QVector<QPointF> iq_qam32;
  while (!in.atEnd()) {
    qint16 x, y;
    in >> x >> y;
    iq_qam32.append(QPointF(x, y));
  }

  file.close();

  emit dataReady(iq_qam32);
}
