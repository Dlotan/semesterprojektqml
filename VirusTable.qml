import QtQuick 2.4
import QtQuick.Controls 1.3

ApplicationWindow {
    id: virusTable
    width: 250
    height: 230
    signal send(string distribution, int quantity, int initialClasses)
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
        y: 53
        text: qsTr("Distribution")
        font.pixelSize: 12
    }

    ComboBox {
        id: virusComboBox
        x: 118
        y: 53
        model: settings.distributionModel
    }

    Text {
        id: text3
        x: 20
        y: 96
        text: qsTr("Count per try")
        font.pixelSize: 12
    }

    TextField {
        id: quantityTextField
        x: 118
        y: 96
        placeholderText: qsTr("")
    }

    Text {
        id: text2
        x: 20
        y: 141
        text: qsTr("Initial Classes")
        font.pixelSize: 12
    }

    TextField {
        id: initialClassesTextField
        x: 118
        y: 141
        placeholderText: qsTr("")
    }

    Button {
        id: virusButton
        x: 93
        y: 187
        text: qsTr("Virus")
        onClicked: {
            var comboBoxText = settings.distributionModel.get(virusComboBox.currentIndex).text
            virusTable.send(comboBoxText, quantityTextField.text*1, initialClassesTextField.text*1);
        }
    }
}
