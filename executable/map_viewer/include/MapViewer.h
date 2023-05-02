// Copyright 2023 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QMainWindow>
#include <QUrl>
#include <Scene.h>

namespace Esri::ArcGISRuntime {
    class Scene;
    class SceneGraphicsView;
    class Map;
    class MapQuickView;
    class GraphicsOverlay;
} // namespace Esri::ArcGISRuntime

class MapViewer : public QMainWindow
{
    Q_OBJECT
public:
    using Scene     = Esri::ArcGISRuntime::Scene;
    using SceneView = Esri::ArcGISRuntime::SceneGraphicsView;

    explicit MapViewer(QUrl server_url, QWidget* parent = nullptr);
    ~MapViewer() override;

private:
    Scene*      _scene = nullptr;
    SceneView*  _scene_view = nullptr;

    QUrl        _server_url;
};

#endif // MAPVIEWER_H
