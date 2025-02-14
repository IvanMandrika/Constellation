#include "ConstellationWidget.h"
#include "DataReader.h"
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMetaType>
#include <QPushButton>
#include <QThread>
#include <QVBoxLayout>

const int32_t WIDTH = 1000;
const int32_t HEIGHT = 800;

int main(int argc, char *argv[]) {
  qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");

  QApplication app(argc, argv);
  QMainWindow mainWindow;

  auto *menuWidget = new QWidget(&mainWindow);
  auto *menuLayout = new QVBoxLayout(menuWidget);

  auto *btn32 = new QPushButton("Small", menuWidget);
  auto *btn128 = new QPushButton("Middle", menuWidget);
  auto *btn256 = new QPushButton("Big", menuWidget);
  auto *btnA16 = new QPushButton("Circle", menuWidget);

  menuLayout->addWidget(btn32);
  menuLayout->addWidget(btn128);
  menuLayout->addWidget(btn256);
  menuLayout->addWidget(btnA16);

  mainWindow.setCentralWidget(menuWidget);

  auto *widget = new ConstellationWidget();

  auto startReadingForFile = [widget, &mainWindow](const QString &filePath) {
    auto *reader = new DataReader();
    const auto readerThread = new QThread();

    reader->setFilePath(filePath);
    reader->moveToThread(readerThread);

    QObject::connect(readerThread, &QThread::started, reader,
                     &DataReader::startReading);
    QObject::connect(reader, &DataReader::dataReady, widget,
                     &ConstellationWidget::updateData);
    QObject::connect(readerThread, &QThread::finished, reader,
                     &QObject::deleteLater);

    mainWindow.setCentralWidget(widget);
    mainWindow.resize(WIDTH, HEIGHT);

    readerThread->start();
    qDebug() << "Data stream started!";
  };

  QObject::connect(btn32, &QPushButton::clicked, [=]() {
    widget->setReferenceFile(QCoreApplication::applicationDirPath() +
                             "/32.xml");
    startReadingForFile("iq_qam32.bin");
  });
  QObject::connect(btn128, &QPushButton::clicked, [=]() {
    widget->setReferenceFile(QCoreApplication::applicationDirPath() +
                             "/128.xml");
    startReadingForFile("iq_qam32.bin");
  });
  QObject::connect(btn256, &QPushButton::clicked, [=]() {
    widget->setReferenceFile(QCoreApplication::applicationDirPath() +
                             "/256.xml");
    startReadingForFile("iq_qam32.bin");
  });
  QObject::connect(btnA16, &QPushButton::clicked, [=]() {
        widget->setReferenceFile(QCoreApplication::applicationDirPath() + "/A16.xml");
        startReadingForFile("iq_qam32.bin");
    });
    mainWindow.show();
    return QApplication::exec();
}