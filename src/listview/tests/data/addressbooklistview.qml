import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
Rectangle {
    id: container
    signal openAddressBookEditor(string str)
    signal createNewEntry()
    signal removeEntry(string str)
    property variant emailDataInfoCurrent;
    property variant nameDataInfoCurrent;

    width: 500
    height: 360
    Menu {
        id: mouseMenu
        title: qsTr("Test")
        MenuItem {
            text: qsTr("Cut");
            shortcut: "Ctrl+X"
        }
        MenuSeparator {
        }
        MenuItem {
            text: qsTr("Open Edit AddressBook");
            onTriggered: {
                container.openAddressBookEditor(nameDataInfoCurrent);
                console.log("Open Edit AddressBook" + nameDataInfoCurrent);
            }
        }
        MenuSeparator {
        }
        MenuItem {
            text: qsTr("Create entry...");
            onTriggered: {
                container.createNewEntry();
            }
        }
        MenuSeparator {
        }
        MenuItem {
            text: qsTr("Remove entry...");
            onTriggered: {
                container.removeEntry(nameDataInfoCurrent);
            }
        }
    }

    Component {
        id: sectionHeading
        Rectangle {
            width: container.width
            height: childrenRect.height
            //color: "lightsteelblue"

            Text {
                text: section
                font.bold: true
                font.pixelSize: 24
            }
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            width: container.width;
            height: 50
            color: "#FFFF88"
            y: view.currentItem.y;
        }
    }
    ListView {
        id: view
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width
        model: addressModel
        delegate: addressBookDelegate

        focus: true
        highlight: highlightBar
        section.property: "addresssection"
        section.criteria: ViewSection.FullString
        section.delegate: sectionHeading
    }
    Component {
        id: addressBookDelegate
        Item {
            id: wrapper
            property variant emailDataInfo: addressemail
            property variant nameDataInfo: addressname

            width: container.width;
            height: 55
            GridLayout {
                rows: 3
                columns: 2
//                Rectangle {
//                    color: "red"
//                    Layout.column: 0
//                    Layout.rowSpan: 1
//                    width: 50
//                    height: 30
//                }

                Image {
                    id: image
                    source: addressicon ? addressicon :  "qrc:/images/images/akregator.svg"
                    //source: addressicon
                }
                Text {
                    id: addressNameId
                    text: addressname;
                    Layout.column: 1
                    Layout.row: 0
                }
                Text {
                    text: addressemail;
                    Layout.column: 1
                    Layout.row: 1
                }
            }
            MouseArea {
                //Use different cursor when we select section or not.
                cursorShape: Qt.IBeamCursor
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onDoubleClicked: {
                    console.log("Double clicked");
                }

                onClicked: {
                    if (mouse.button == Qt.LeftButton) {
                        wrapper.ListView.view.currentIndex = index
                    } else if (mouse.button == Qt.RightButton) {
                        emailDataInfoCurrent = addressemail;
                        nameDataInfoCurrent = addressname;
                        mouseMenu.popup();
                    }
                }
            }
        }
    }
}
