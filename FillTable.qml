import QtQuick 2.4
import QtQuick.Controls 1.3

ApplicationWindow {
    width: 270
    height: 170
    id: fillTable
    signal send(int quantity, int initialClasses)
    function onProgress(progress) {
        progressBar.setValue(progress);
    }

    function reset() {
        progressBar.setValue(0);
        quantityTextField.text = ""
        initialClassesTextField.text = ""
    }

    Text {
        id: text1
        x: 24
        y: 54
        text: qsTr("Count")
        font.pixelSize: 12
    }

    Text {
        id: text2
        x: 24
        y: 96
        text: qsTr("Initial Classes")
        font.pixelSize: 12
    }

    TextField {
        id: quantityTextField
        x: 148
        y: 54
        placeholderText: qsTr("")
    }

    TextField {
        id: initialClassesTextField
        x: 148
        y: 96
        placeholderText: qsTr("")
    }

    Button {
        id: sendButton
        x: 96
        y: 134
        width: 67
        height: 27
        text: qsTr("Send")
        onClicked: {
            fillTable.send(quantityTextField.text*1, initialClassesTextField.text*1);
        }
    }

    ProgressBar {
        id: progressBar
        x: 35
        y: 15
        value: 0
        maximumValue: 100
    }
}

