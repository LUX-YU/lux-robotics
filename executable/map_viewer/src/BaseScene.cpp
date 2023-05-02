#include "BaseScene.h"
#include "ArcGISTiledElevationSource.h"
#include "SceneGraphicsView.h"
#include "ElevationSourceListModel.h"
#include "Surface.h"

BaseScene::BaseScene(Esri::ArcGISRuntime::BasemapStyle style, const QUrl& server_url, QWidget* parent)
    :Esri::ArcGISRuntime::Scene(style, parent)
{
    using namespace Esri::ArcGISRuntime;

    auto _elevation_source = new ArcGISTiledElevationSource(server_url, this);

    // add the elevation source to the scene to display elevation
    baseSurface()->elevationSources()->append(_elevation_source);
}

BaseScene::~BaseScene() = default;

void BaseScene::setSceneView(Esri::ArcGISRuntime::SceneGraphicsView* sceneView)
{
    sceneView->setArcGISScene(this);
}
