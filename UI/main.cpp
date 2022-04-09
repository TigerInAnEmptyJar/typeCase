#include "parameter_models/AlgorithmModel.hpp"
#include "parameter_models/AllData.hpp"
#include "parameter_models/DetectorModel.hpp"
#include "parameter_models/MaterialModel.hpp"
#include "parameter_models/RunModel.hpp"
#include "parameter_models/ShapeModel.hpp"

#include "provideShapes.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QSettings>

int main(int argc, char* argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);
  app.setOrganizationName("TypeCase Inc.");
  app.setOrganizationDomain("typecase.com");
  app.setApplicationName("TypeCase");

  QQmlApplicationEngine engine;

  auto& shapeFactory = ShapeFactory::getInstance();
  Shape::addShapesToFactory(shapeFactory);

  UI::AllData allData;
  models::RunModel runs(allData, {});
  models::ShapeModel shapes(allData);
  models::DetectorModel detectors(allData);
  models::MaterialModel materials(allData);
  models::AlgorithmModel algorithms(allData);

  auto context = engine.rootContext();
  context->setContextProperty("runModel", &runs);
  context->setContextProperty("shapeModel", &shapes);
  context->setContextProperty("detectorModel", &detectors);
  context->setContextProperty("materialModel", &materials);
  context->setContextProperty("algorithmModel", &algorithms);
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
