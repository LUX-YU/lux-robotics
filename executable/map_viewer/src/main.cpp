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

// Qt headers
#include <QApplication>
#include <QMessageBox>

#include <QSurfaceFormat>
#include <QSslSocket>

#include <ArcGISRuntimeEnvironment.h>
#include <MapQuickView.h>
#include <LicenseResult.h>
#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "MapViewer.h"

using namespace Esri::ArcGISRuntime;


int main(int argc, char *argv[])
{
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    // Linux requires 3.2 OpenGL Context
    // in order to instance 3D symbols
    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
    fmt.setVersion(3, 2);
    QSurfaceFormat::setDefaultFormat(fmt);
#endif
    QApplication application(argc, argv);
    // Use of Esri location services, including basemaps and geocoding, requires
    // either an ArcGIS identity or an API key. For more information see
    // https://links.esri.com/arcgis-runtime-security-auth.

    // 1. ArcGIS identity: An ArcGIS named user account that is a member of an
    // organization in ArcGIS Online or ArcGIS Enterprise.

    // 2. API key: A permanent key that gives your application access to Esri
    // location services. Create a new API key or access existing API keys from
    // your ArcGIS for Developers dashboard (https://links.esri.com/arcgis-api-keys).

    const QString apiKey = QString("AAPK2a73953b32f249c5848606b3e3fc26c7l_FSIVwNMq4AjLu_m0rUmfgowKee7ZmNEe-QA0wCoagC3koXLwPmBNEkZwz1y_Si");
    if (apiKey.isEmpty())
    {
        qWarning() << "Use of Esri location services, including basemaps, requires" <<
            "you to authenticate with an ArcGIS identity or set the API Key property.";
    }
    else
    {
        ArcGISRuntimeEnvironment::setApiKey(apiKey);
    }

    // Production deployment of applications built with ArcGIS Maps SDK requires you to
    // license ArcGIS Maps SDK functionality. For more information see
    // https://links.esri.com/arcgis-runtime-license-and-deploy.

    ArcGISRuntimeEnvironment::setLicense("runtimelite,1000,rud5023021368,none,NKMFA0PL4P2SPF002112");

    //  use this code to check for initialization errors
    // QObject::connect(ArcGISRuntimeEnvironment::instance(), &ArcGISRuntimeEnvironment::errorOccurred, [](const auto& error){
    //   QMessageBox msgBox;
    //   msgBox.setText(error.message);
    //   msgBox.exec();
    // });

    //  if (ArcGISRuntimeEnvironment::initialize() == false)
    //  {
    //    application.quit();
    //    return 1;
    //  }

    MapViewer applicationWindow;
    applicationWindow.setMinimumWidth(800);
    applicationWindow.setMinimumHeight(600);
    applicationWindow.show();

    return application.exec();
}
