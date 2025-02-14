#include "ConstellationWidget.h"
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QXmlStreamReader>
#include <algorithm>
#include <random>
#include <unordered_map>

ConstellationWidget::ConstellationWidget(QWidget *parent) : QWidget(parent) {
  loadReferenceMarkers();
}

void ConstellationWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QVector<QPointF> limitedPoints = points;
  if (limitedPoints.size() > 16000) {
    std::ranges::sample(limitedPoints, limitedPoints.begin(), 16000,
                        std::mt19937{std::random_device{}()});
    limitedPoints.resize(16000);
  }

  painter.setPen(QPen(Qt::black, 2));
  for (const QPointF &point : limitedPoints) {
    QPointF mappedPoint = mapToWidget(point);

    painter.drawPoint(mappedPoint);
  }

  painter.setPen(QPen(Qt::red, 1));
  for (const QPointF &marker : referenceMarkers) {
    QPointF mappedMarker = mapMarkToWidget(marker);
    painter.drawLine(mappedMarker + QPointF(-5, 0),
                     mappedMarker + QPointF(5, 0));
    painter.drawLine(mappedMarker + QPointF(0, -5),
                     mappedMarker + QPointF(0, 5));
  }
}

void ConstellationWidget::updateData(const QVector<QPointF> &newPoints) {
  points = newPoints;
  update();
}
void ConstellationWidget::setReferenceFile(const QString &filePath) {
  referenceFilePath = filePath;
  loadReferenceMarkers();
}
void ConstellationWidget::loadReferenceMarkers() {
  if (referenceFilePath.isEmpty()) {
    return;
  }

  QFile file(referenceFilePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Error with opening xml file" << referenceFilePath;
    return;
  }

  referenceMarkers.clear();
  QXmlStreamReader xml(&file);

  while (!xml.atEnd() && !xml.hasError()) {
    if (const QXmlStreamReader::TokenType token = xml.readNext();
        token == QXmlStreamReader::StartElement && xml.name() == "Point") {
      bool okX, okY;
      const double x = xml.attributes().value("X").toDouble(&okX);
      const double y = xml.attributes().value("Y").toDouble(&okY);
      if (okX && okY) {
        referenceMarkers.append(QPointF(x, y));
      }
    }
  }

  file.close();
  update();
}

QPointF ConstellationWidget::mapToWidget(const QPointF &point) const {
  constexpr double minX = -9766, maxX = 9769;
  constexpr double minY = -9812, maxY = 9799;

  const double scaleX = width() / (maxX - minX);
  const double scaleY = height() / (maxY - minY);

  const double mappedX = (point.x() - minX) * scaleX;
  const double mappedY = height() - (point.y() - minY) * scaleY;

  return {mappedX, mappedY};
}

QPointF ConstellationWidget::mapMarkToWidget(const QPointF &point) const {
  QStringList tmp = referenceFilePath.split("/");
  QString name = tmp[tmp.length() - 1]; // cut .xml, not magic number =)
  name = name.left(name.length() - 4);

  std::unordered_map<QString, double> mapping = {{QString("32"), 10.0},
                                                 {QString("128"), 18.0},
                                                 {QString("256"), 30.0},
                                                 {QString("A16"), 10.0}};
  const double scale = mapping[name];
  const double scaleX = static_cast<float>(width()) / scale;
  const double scaleY = static_cast<float>(height()) / scale;

  const double centerX = static_cast<float>(width()) / 2;
  const double centerY = static_cast<float>(height()) / 2;

  return {centerX + point.x() * scaleX, centerY - point.y() * scaleY};
}
