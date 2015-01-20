import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: virusTable
    width: 250
    height: 230
    signal send(string distribution, int quantity, int initialClasses, bool insert)
    function reset() {
        progressBar.value = 0;
        quantityTextField.text = "";
        initialClassesTextField.text = "";
    }
    function onProgress(progress) {
        progressBar.setValue(progress);
    }
    Settings {
        id: settings
    }

    ProgressBar {
        id: progressBar
        x: 25
        y: 11
        maximumValue: 100
    }

    Text {
        id: text1
        x: 20
        y: 41
        text: qsTr("Distribution")
        font.pixelSize: 12
    }

    ComboBox {
        id: virusComboBox
        x: 118
        y: 41
        model: settings.distributionModel
    }

    Text {
        id: text3
        x: 20
        y: 84
        text: qsTr("Count per try")
        font.pixelSize: 12
    }

    TextField {
        id: quantityTextField
        x: 118
        y: 84
        placeholderText: qsTr("")
    }

    Text {
        id: text2
        x: 20
        y: 129
        text: qsTr("Initial Classes")
        font.pixelSize: 12
    }

    TextField {
        id: initialClassesTextField
        x: 118
        y: 129
        placeholderText: qsTr("")
    }

    RowLayout {
        x: 20
        y: 168
        width: 205
        height: 20
        ExclusiveGroup {
            id: virusType
        }
        Text {
            text: "Type: "
        }

        RadioButton {
            id: insertRadioButton
            text: "Insert"
            checked: true
            exclusiveGroup: virusType
        }
        RadioButton {
            text: "Update"
            exclusiveGroup: virusType
        }
    }

    Button {
        id: virusButton
        x: 93
        y: 199
        text: qsTr("Virus")
        onClicked: {
            var comboBoxText = settings.distributionModel.get(virusComboBox.currentIndex).text
            var insert = insertRadioButton.checked
            virusTable.send(comboBoxText, quantityTextField.text*1,
                            initialClassesTextField.text*1, insert);
        }
    }
}
