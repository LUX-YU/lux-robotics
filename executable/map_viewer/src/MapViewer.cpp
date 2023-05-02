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

// Other headers
#include "MapViewer.h"

// C++ API headers
#include "Map.h"
#include "MapQuickView.h"
#include "Point.h"
#include "SpatialReference.h"
#include "TaskWatcher.h"
#include "Viewpoint.h"
#include "ArcGISTiledElevationSource.h"
#include "ElevationSourceListModel.h"
#include "MapTypes.h"
#include "Scene.h"
#include "SceneGraphicsView.h"
#include "Surface.h"
#include "Graphic.h"
#include "GraphicListModel.h"
#include "GraphicsOverlay.h"
#include "GraphicsOverlayListModel.h"
#include "PolylineBuilder.h"
#include "PolygonBuilder.h"
#include "SimpleFillSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleMarkerSymbol.h"
#include "SymbolTypes.h"


#include <QUrl>

using namespace Esri::ArcGISRuntime;

MapViewer::MapViewer(QWidget* parent /*=nullptr*/):
    QMainWindow(parent)
{
    // Create a scene using the ArcGISStreets BasemapStyle
    Scene* scene = new Scene(BasemapStyle::ArcGISImagery, this);

    // create a new elevation source from Terrain3D rest service
    ArcGISTiledElevationSource* elevationSource = new ArcGISTiledElevationSource(
        QUrl("https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer"), this);

    // add the elevation source to the scene to display elevation
    scene->baseSurface()->elevationSources()->append(elevationSource);

    // Create a scene view, and pass in the scene
    m_sceneView = new SceneGraphicsView(scene, this);

    GraphicsOverlay* overlay = new GraphicsOverlay(this);
    // createGraphics(overlay);
    // Create a point
    const Point dume_beach(-118.80657463861, 34.0005930608889, SpatialReference::wgs84());
    // Create symbols for the point
    SimpleLineSymbol* point_outline = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor("blue"), 3, this);
    SimpleMarkerSymbol* point_symbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor("red"), 10, this);
    point_symbol->setOutline(point_outline);

    // Create a graphic to display the point with its symbology
    Graphic* point_graphic = new Graphic(dume_beach, point_symbol, this);
    // Add point graphic to the graphics overlay
    overlay->graphics()->append(point_graphic);

    m_sceneView->graphicsOverlays()->append(overlay);

    // set the sceneView as the central widget
    setCentralWidget(m_sceneView);
}

// destructor
MapViewer::~MapViewer()
{
}
