#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "BaseScene.h"

// ARCGIS RUNTIME
#include "SceneGraphicsView.h"
#include "MapTypes.h"
#include "Graphic.h"
#include "Point.h"
#include "Multipoint.h"
#include "MultipointBuilder.h"
#include "PointCollection.h"
#include "SpatialReference.h"
#include "GraphicListModel.h"
#include "GraphicsOverlay.h"
#include "GraphicsOverlayListModel.h"
#include "SimpleFillSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleMarkerSymbol.h"
#include "SymbolTypes.h"
#include "TaskWatcher.h"
#include "LocationToScreenResult.h"
#include "Camera.h"
#include "CameraController.h"
#include "GlobeCameraController.h"

// QT dependency
#include "QFileDialog"

// dataset parser
#include <lux/dataset/KITTI.hpp>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent), _ui(new Ui::MainWindow{})
{
	using namespace Esri::ArcGISRuntime;
	_ui->setupUi(this);

	QUrl server_url("https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer");

	_scene = new BaseScene(BasemapStyle::ArcGISImagery, server_url, this);
	_scene->setSceneView(_ui->scene_graphics_view);

	_overlay = new GraphicsOverlay(this);
	_ui->scene_graphics_view->graphicsOverlays()->append(_overlay);

	initialize_connection();
}

MainWindow::~MainWindow()
{
	delete _ui;
}

void MainWindow::initialize_connection()
{
	using namespace ::lux::robotics;
	using namespace ::Esri::ArcGISRuntime;

	connect(_ui->pushButton, &QPushButton::clicked,
		[this](bool checked) {
			auto file_name = QFileDialog::getExistingDirectory(this);
			if (file_name.isEmpty()) return;
			lux::robotics::OXTSLoader loader(std::filesystem::path{ file_name.toStdString() });
			
			if (!loader.isOpened())
			{
				return;
			}

			std::vector<OXTSFormat> load_list;
			if (loader.loadAll(load_list) == 0)
				return;
			// convert to gps data
			Esri::ArcGISRuntime::PointCollection gps_datas(SpatialReference::wgs84());
			for (const auto& oxts_point : load_list)
			{
				gps_datas.addPoint(oxts_point.lon, oxts_point.lat);
			}
			handle_positon(&gps_datas);
		}
	);
}

void MainWindow::handle_positon(Esri::ArcGISRuntime::PointCollection* positions)
{
	using namespace ::lux::robotics;
	using namespace ::Esri::ArcGISRuntime;
	
	int elem_size = positions->size();
	if (elem_size == 0) return;

	double average_lon = 0;
	double average_lat = 0;
	
	for(auto iter = positions->begin(); iter != positions->end(); iter++)
	{
		auto aaaa = *iter;
		average_lon += ((*iter).x() / elem_size);
		average_lat += ((*iter).y() / elem_size);
	}

	MultipointBuilder point_builder(SpatialReference::wgs84());
	point_builder.setPoints(positions);

	SimpleLineSymbol*   point_outline = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor("blue"), 3, this);
	SimpleMarkerSymbol* point_symbol  = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor("red"), 10, this);
	point_symbol->setOutline(point_outline);

	Graphic* point_graphic = new Graphic(point_builder.toMultipoint(), point_symbol, this);

	_overlay->graphics()->append(point_graphic);

	Camera camera(Point{average_lon, average_lat}, 10.0f, 0,0,0);
	_ui->scene_graphics_view->setViewpointCameraAndWait(camera);
}
