import QtQuick 2.0
import QtQml 2.0
 import QtQuick.Window 2.0
 import QtLocation 5.6
 import QtPositioning 5.6

MapItemView {
     width: 512
     height: 512
     visible: true

     Plugin {
         id: mapPlugin
         name: "esri" // "mapboxgl", "esri", ...
         // specify plugin parameters if necessary
         // PluginParameter {
         //     name:
         //     value:
         // }
     }

     Map {
         anchors.fill: parent
         plugin: mapPlugin
         center: QtPositioning.coordinate(55.751244, 37.618423) // Oslo
         zoomLevel: 10
         maximumTilt: 0
     }

 }
