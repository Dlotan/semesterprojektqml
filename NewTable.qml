import QtQuick 2.4
import QtQuick.Controls 1.3

ApplicationWindow {
    id: newTable
    width: 250
    height: 150
    signal send(string tableName, string distribution, bool hasIndex)
    function reset() {
        indexCheckBox.checked = true;
        nameTextfield.text = "";
    }
    Settings {
        id: settings
    }

    Text {
        x: 20
        y: 27
        text:"Name"
    }

    Text {
        id: text1
        x: 20
        y: 61
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
        y: 61
        model: settings.distributionModel
    }

    Text {
        id: text2
        x: 20
        y: 94
        text: qsTr("Inedex")
        font.pixelSize: 12
    }

    CheckBox {
        id: indexCheckBox
        x: 157
        y: 94
        text: qsTr("")
        checked: true
    }

    Button {
        id: newTableButton
        x: 87
        y: 121
        text: qsTr("New Table")
        onClicked: {
            var comboBoxText = settings.distributionModel.get(distributionComboBox.currentIndex).text
            newTable.send(nameTextfield.text, comboBoxText, indexCheckBox.checked);
            newTable.hide()
        }
    }

}

