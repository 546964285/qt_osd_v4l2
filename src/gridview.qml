/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//import QtQuick 1.0
import Qt 4.7

Rectangle {
    id:mainMenu
    width: 640; height: 480

    signal buttonClicked(int gridPos)
//    onButtonClicked: console.log("Mouse was clicked " + gridPos);

    Keys.onPressed:{
        if (event.key == Qt.Key_Return) {
            //console.log("opps!!");
            console.log("currentIndex at "+gridview.currentIndex);
            mainMenu.buttonClicked(gridview.currentIndex);
        }
    }

    Rectangle{
        id: topBar
        x: 0
        y: 0
        z:1
        width: 640
        height: 80
        color: "#2b3746"
        Image {
            x: 0
            y: 0
            width: 80
            height: 80
            source: "images/settings1.png"
        }
        Text{
            wrapMode:Text.WordWrap
            font.pixelSize: 60
//            font.italic: true
            font.bold: true
            text: qsTr("Settings")
            color:"white"
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Rectangle{
        id: contralWindow
        anchors.top: topBar.bottom
        width: 640
        height: 320
        color: "#1a1d24"

        ListModel {
            id: appModel
            ListElement {
                name: QT_TR_NOOP("backplay")
                icon: "images/AudioPlayer_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("time")
                icon: "images/VideoPlayer_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("language")
                icon: "images/Camera_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("vedio")
                icon: "images/DateBook_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("color")
                icon: "images/EMail_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("standby")
                icon: "images/TodoList_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("restore")
                icon: "images/AddressBook_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("storage")
                icon: "images/AddressBook_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("reserved")
                icon: "images/AddressBook_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("reserved")
                icon: "images/AddressBook_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("reserved")
                icon: "images/AddressBook_48.png"
            }
            ListElement {
                name: QT_TR_NOOP("version")
                icon: "images/AddressBook_48.png"
            }

        }

        Component {
            id: appDelegate

            Item {
                width: 160; height: 160
                //property int index: model.index
                Image {
                    id: myIcon
                    y: 20; anchors.horizontalCenter: parent.horizontalCenter
                    width: 100; height: 100
                    source: icon
                }
                Text {
                    anchors { top: myIcon.bottom; horizontalCenter: parent.horizontalCenter }
                    font.pixelSize: 30
                    font.bold: true
                    text: qsTr(name)
                    color: "white"
                }
            }
        }

        Component {
            id: appHighlight
            Rectangle { width: 160; height: 160; color: "lightsteelblue" }
        }

        GridView {
            id: gridview
//            interactive: false
            anchors.fill: parent
            cellWidth: 160; cellHeight: 160
            highlight: appHighlight
            focus: true
            model: appModel
            delegate: appDelegate
//            clip: true
//            snapMode: GridView.SnapToRow
//            flow: GridView.TopToBottom
        }
    }

    Rectangle{
        id: bottomBar
        anchors.top: contralWindow.bottom
        width: 640
        height: 80
        z:1
        color: "#2b3746"
//        color: "white"
    }


}
