#pragma once
#include <QMainWindow>
#include <vector>
#include <lux/driver/gps/GPSBase.hpp>

namespace Ui {
    class MainWindow;
} // namespace Ui

namespace Esri::ArcGISRuntime {
    class GraphicsOverlay;
    class GlobeCameraController;
    class PointCollection;
} // namespace Esri::ArcGISRuntime

class BaseScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

private:
    void initialize_connection();

    void handle_positon(Esri::ArcGISRuntime::PointCollection* positions);

    using GOverLay = Esri::ArcGISRuntime::GraphicsOverlay;
    using CameraController = Esri::ArcGISRuntime::GlobeCameraController;

    Ui::MainWindow*     _ui;
    BaseScene*          _scene;
    GOverLay*           _overlay;
};
