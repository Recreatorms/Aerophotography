import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6


Map {
    id: map
    anchors.centerIn: parent
    anchors.fill: parent
    plugin: Plugin {
        name: "osm"
    }
    center: QtPositioning.coordinate(55.751244, 37.618423) // Moscow
    zoomLevel: 10
    maximumTilt: 0

    //        MapCircle {
    ////            center {
    ////                latitude: 55.751244
    ////                longitude: 37.618423
    ////            }
    //            radius: 500000
    //            color: 'green'
    //            border.width: 3
    //        }
    property MapCircle circle
    Component.onCompleted: {

        circle = Qt.createQmlObject('import QtLocation 5.6; MapCircle {}', page)
        circle.center = QtPositioning.coordinate(55.751244, 37.618423)
        circle.radius = 500000.0
        circle.color = 'green'
        circle.border.width = 3
        map.addMapItem(circle)
    }
}

