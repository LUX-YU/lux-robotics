#pragma once

#include <QObject>
#include <Scene.h>

namespace Esri::ArcGISRuntime {
    class Scene;
    class SceneGraphicsView;
    class Map;
    class MapQuickView;
    class GraphicsOverlay;
} // namespace Esri::ArcGISRuntime

class BaseScene : public Esri::ArcGISRuntime::Scene
{
    Q_OBJECT
public:

    BaseScene(Esri::ArcGISRuntime::BasemapStyle style, const QUrl& server_url, QWidget* parent = nullptr);

    ~BaseScene() override;

    void setSceneView(Esri::ArcGISRuntime::SceneGraphicsView* sceneView);
};
