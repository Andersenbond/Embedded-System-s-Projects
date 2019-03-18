import QtQuick 2.5

Rectangle {

    width: 360
    height: 360
    property alias leftbutton: leftbutton
    property alias downbutton: downbutton
    property alias upbutton: upbutton
    property alias rightbutton: rightbutton

    MouseArea {
        id: upbutton
        x: 72
        y: 81
        width: 54
        height: 48
    }

    MouseArea {
        id: downbutton
        x: 72
        y: 183
        width: 54
        height: 48
    }

    MouseArea {
        id: rightbutton
        x: 129
        y: 129
        width: 48
        height: 54
    }

    MouseArea {
        id: leftbutton
        x: 21
        y: 129
        width: 48
        height: 54
    }

    Image {
        id: image4_left
        x: 21
        y: 129
        width: 48
        height: 54
        source: "file:///home/nuno/Desktop/GUI/ESRG/left.jpg"
    }

    Image {
        id: image4_left1
        x: 75
        y: 78
        width: 48
        height: 54
        rotation: 90
        source: "file:///home/nuno/Desktop/GUI/ESRG/left.jpg"
    }

    Image {
        id: image4_left2
        x: 129
        y: 129
        width: 48
        height: 54
        source: "file:///home/nuno/Desktop/GUI/ESRG/left.jpg"
        rotation: 180
    }

    Image {
        id: image4_left3
        x: 75
        y: 180
        width: 48
        height: 54
        source: "file:///home/nuno/Desktop/GUI/ESRG/left.jpg"
        rotation: -90
    }

}
