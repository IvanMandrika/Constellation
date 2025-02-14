#ifndef CONSTELLATIONWIDGET_H
#define CONSTELLATIONWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>

class ConstellationWidget final : public QWidget {
    Q_OBJECT

public:
    explicit ConstellationWidget(QWidget *parent = nullptr);
    void updateData(const QVector<QPointF> &newPoints);
    void setReferenceFile(const QString &filePath);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString referenceFilePath;
    QVector<QPointF> points;
    QVector<QPointF> referenceMarkers;

    void loadReferenceMarkers();
    [[nodiscard]] QPointF mapToWidget(const QPointF &point) const;
    [[nodiscard]] QPointF mapMarkToWidget(const QPointF&point)const;
};

#endif // CONSTELLATIONWIDGET_H
