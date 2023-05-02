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

namespace Esri::ArcGISRuntime {
    class SceneGraphicsView;
    class Map;
    class MapQuickView;
    class GraphicsOverlay;
} // namespace Esri::ArcGISRuntime

#include <QMainWindow>

class MapViewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit MapViewer(QWidget* parent = nullptr);
    ~MapViewer() override;

    Esri::ArcGISRuntime::SceneGraphicsView*     m_sceneView = nullptr;
};

#endif // MAPVIEWER_H
