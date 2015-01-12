import QtQuick 2.4
import QtQuick.Controls 1.3

ApplicationWindow {
    width: 270
    height: 130
    id: fillTable
    signal send(int quantity, int initialClasses)
    function reset() {
        quantityTextField.text = ""
        initialClassesTextField.text = ""
    }

    Text {
        id: text1
        x: 24
        y: 14
        text: qsTr("Count")
        font.pixelSize: 12
    }

    Text {
        id: text2
        x: 24
        y: 56
        text: qsTr("Initial Classes")
        font.pixelSize: 12
    }

    TextField {
        id: quantityTextField
        x: 148
        y: 14
        placeholderText: qsTr("")
    }

    TextField {
        id: initialClassesTextField
        x: 148
        y: 56
        placeholderText: qsTr("")
    }

    Button {
        id: sendButton
        x: 96
        y: 94
        text: qsTr("Send")
        onClicked: {
            fillTable.send(quantityTextField.text*1, initialClassesTextField.text*1);
            fillTable.close();
        }
    }
}

