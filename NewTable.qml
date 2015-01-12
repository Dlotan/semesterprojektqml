import QtQuick 2.4
import QtQuick.Controls 1.3

ApplicationWindow {
    id: newTable
    width: 250
    height: 150
    signal send(string tableName, string distribution)
    function reset() {
        nameTextfield.text = "";
    }

    Text {
        x: 20
        y: 27
        text:"Name"
    }

    Text {
        id: text1
        x: 20
        y: 77
        text: qsTr("Distribution")
        font.pixelSize: 12
    }

    TextField {
        id: nameTextfield
        x: 118
        y: 27
        placeholderText: qsTr("")
    }

    ComboBox {
        id: distributionComboBox
        x: 118
        y: 77
        model: ListModel {
            id: distributionComboBoxItems
            ListElement {text: "cauchy"}
            ListElement {text: "cosine"}
            ListElement {text: "weibull"}
            ListElement {text: "normal"}
            ListElement {text: "logNormal"}
        }
    }

    Button {
        id: newTableButton
        x: 87
        y: 114
        text: qsTr("New Table")
        onClicked: {
            var comboBoxText = distributionComboBoxItems.get(distributionComboBox.currentIndex).text
            newTable.send(nameTextfield.text, comboBoxText);
            newTable.hide()
        }
    }
}

