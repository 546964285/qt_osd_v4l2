import QtQuick 1.0

Rectangle {
    id: mainWindow
    property bool imageLoading: false

    width: 640; height: 480; color: "#d5d6d8"

    state: "summary"
    states: [
        State { name: "summary"; },
        State { name: "details"; }
    ]
    transitions: [
        Transition { from: "summary"; to: "details";
            SequentialAnimation {
                PropertyAction { target: animationRect; property: "visible"; value: true; }
                PropertyAction { target: detailsView; property: "focus"; value: true; }
                PropertyAction { target: summaryView; property: "focus"; value: false; }
//                ParallelAnimation {
//                    NumberAnimation { target: animationRect; properties: "x,y"; to: 0; duration: 200; }
//                    NumberAnimation { target: animationRect; property: "width"; to: 400; duration: 200; }
//                    NumberAnimation { target: animationRect; property: "height"; to: 200; duration: 200; }
//                }
                PropertyAction { target: detailsView; property: "visible"; value: true; }
                PropertyAction { target: summaryView; property: "visible"; value: false; }
                PropertyAction { target: animationRect; property: "visible"; value: false; }
            }
        },
        Transition { from: "details"; to: "summary";
            id: shrinkTransition
            property variant destRect: {"x": 0, "y": 0, "width": 0, "height": 0}

            SequentialAnimation {
                PropertyAction { target: summaryView; property: "visible"; value: true; }
                PropertyAction { target: animationRect; property: "visible"; value: true; }
                PropertyAction { target: detailsView; property: "visible"; value: false; }
//                ParallelAnimation {
//                    NumberAnimation { target: animationRect; property: "x"; to: shrinkTransition.destRect.x; duration: 200; }
//                    NumberAnimation { target: animationRect; property: "y"; to: shrinkTransition.destRect.y; duration: 200; }
//                    NumberAnimation { target: animationRect; property: "width"; to: shrinkTransition.destRect.width; duration: 200; }
//                    NumberAnimation { target: animationRect; property: "height"; to: shrinkTransition.destRect.height; duration: 200; }
//                }
                PropertyAction { target: animationRect; property: "visible"; value: false; }
                PropertyAction { target: detailsView; property: "focus"; value: false; }
                PropertyAction { target: summaryView; property: "focus"; value: true; }
            }
        }
    ]

    Rectangle {
        id: animationRect
        z: 1
        color: "gray"
        visible: false

        function positionOverSummary(summaryRect) {
            x = summaryRect.x; y = summaryRect.y;
            width = summaryRect.width; height = summaryRect.height;
        }

        function prepareForShrinkingTo(summaryRect) {
            x = 0; y = 0;
            width = 400; height = 400;
            shrinkTransition.destRect = summaryRect;
        }
    }

    Component {
        id: appDelegate

        Item {
            // These are needed for getDelegateInstanceAt() below.
            objectName: "summaryDelegate"
            property int index: model.index

            //color: "lightgray"
            width: 160; height: 160

            Image {
                id: myIcon
                y: 20; anchors.horizontalCenter: parent.horizontalCenter
                width: 100; height: 100
                source: "photos/"+modelData+".jpg"
            }
            Text {
                anchors { top: myIcon.bottom; horizontalCenter: parent.horizontalCenter }
                font.pixelSize: 30
                font.bold: true
                text: modelData
                color: "black"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var delegateRect = mapToItem(mainWindow, x, y);
                    delegateRect.width = width; delegateRect.height = height;

                    animationRect.positionOverSummary(delegateRect);
                    // detailsView.positionViewAtIndex(index, ListView.Beginning);
                    mainWindow.state = "details";
                    detailsView.currentImage=modelData;
                    detailsView.index=index;
                }
            }
            Keys.onPressed:{
                if (event.key == Qt.Key_Return) {
                    console.log("opps!!");
                    var delegateRect = mapToItem(mainWindow, x, y);
                    delegateRect.width = width; delegateRect.height = height;

                    animationRect.positionOverSummary(delegateRect);
                    // detailsView.positionViewAtIndex(index, ListView.Beginning);
                    mainWindow.state = "details";
                    detailsView.currentImage=modelData;
                    detailsView.index=index;
                }
            }
        }
    }

    Component {
        id: appHighlight
        Rectangle { width: 160; height: 160; color: "lightsteelblue" }
    }

    GridView {
        id: summaryView
//        width: 100; height: 100
        anchors.fill: parent
        cellWidth: 160; cellHeight: 160
        highlight: appHighlight
        focus: true
        model: myModel
//        delegate: Rectangle {
//            height: 100
//            width: 100
//            Image {
//                height: 100
//                width: 100
//                source: modelData
//            }
//        }
        delegate:appDelegate

        // Uses black magic to hunt for the delegate instance with the given
        // index.  Returns undefined if there's no currently instantiated
        // delegate with that index.
        function getDelegateInstanceAt(index) {
            for(var i = 0; i < contentItem.children.length; ++i) {
                var item = contentItem.children[i];
                // We have to check for the specific objectName we gave our
                // delegates above, since we also get some items that are not
                // our delegates here.
                if (item.objectName == "summaryDelegate" && item.index == index)
                    return item;
            }
            return undefined;
        }
    }

    Rectangle{
        id: detailsView
        property string currentImage;
        property int index:0
        anchors.fill: parent
        color: "#d5d6d8"
        visible: false
        focus: false
        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 480; height: 480;
            source: "photos/"+detailsView.currentImage+".jpg"
        }

        MouseArea {
            anchors.fill: parent
            onClicked:{
                summaryView.positionViewAtIndex(detailsView.index, GridView.Visible);

                var delegateInstance = summaryView.getDelegateInstanceAt(detailsView.index);
                var delegateRect = mainWindow.mapFromItem(summaryView,
                    delegateInstance.x - summaryView.contentX,
                    delegateInstance.y - summaryView.contentY
                );
                delegateRect.width = delegateInstance.width;
                delegateRect.height = delegateInstance.height;

                animationRect.prepareForShrinkingTo(delegateRect);
                mainWindow.state = "summary";
            }
        }

        Keys.onPressed:{
            if (event.key == Qt.Key_Return) {
                console.log("opps!");
                summaryView.positionViewAtIndex(detailsView.index, GridView.Visible);

                var delegateInstance = summaryView.getDelegateInstanceAt(detailsView.index);
                var delegateRect = mainWindow.mapFromItem(summaryView,
                    delegateInstance.x - summaryView.contentX,
                    delegateInstance.y - summaryView.contentY
                );
                delegateRect.width = delegateInstance.width;
                delegateRect.height = delegateInstance.height;

                animationRect.prepareForShrinkingTo(delegateRect);
                mainWindow.state = "summary";
            }
        }
    }
}

