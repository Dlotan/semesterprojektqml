import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import custom 1.0
import jbQuick.Charts 1.0

ApplicationWindow {
    id: main
    title: qsTr("Semesterprojekt")
    width: 1000
    height: 500
    visible: true
    menuBar: MenuBar {
        Menu {
            title: qsTr("&Bla")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: {}
            }
        }
    }

    Database {
        id: database
        Component.onCompleted: {
            database.onError.connect(showError);
        }
        function showError(errorMsg) {
            errorDialog.text = errorMsg;
            fillTable.hide();
            virusTable.hide();
            errorDialog.open();
        }
    }

    MessageDialog {
        id: errorDialog
        title: "ERROR"
        text: ""
        onAccepted: close()
    }

    RowLayout{
        anchors.fill: parent
        ColumnLayout {
            anchors.top: parent.top
            id: buttonColumn
            spacing: 1
            Button {
                id: resetButton
                text: qsTr("Reset DB")
                onClicked: {
                    database.resetDatabase();
                    Qt.quit();
                }
            }
            Button {
                id: newButton
                text: "New Table"
                onClicked: {
                    newTable.reset();
                    newTable.show();
                }
            }
            Button {
                id: valueAddButton
                text: "Fill"
                onClicked: {
                    fillTable.reset()
                    fillTable.show();
                }
            }
            Button {
                id: profileButton
                text: "Profile"
                onClicked: {
                    var result = database.profile();
                    profileWindow.initialize(result);
                    profileWindow.show();
                }
            }

            Button {
                id: profileRangeButton
                text: qsTr("Profile Range")
            }

            Button {
                id: deleteButton
                x: 8
                y: 106
                text: qsTr("Delete")
                onClicked: {
                    database.deleteTable(tableView.model.getName(tableView.currentRow));
                    tableView.refresh();
                }

            }
            Button {
                id: virusInsertButton
                x: 17
                y: 125
                text: "Parasite"
                onClicked: {
                    virusTable.reset();
                    virusTable.show();
                }
            }
        }

        TableView {
            id: tableView
            Layout.fillWidth: true
            Layout.fillHeight: true
            function refresh() {
                var component = Qt.createQmlObject("import custom 1.0; SqlQueryModel{}", tableView);
                component.setQuery("SELECT * FROM dist_tables");
                tableView.model = component;
            }
            onCurrentRowChanged: chart.reset()
            Component.onCompleted: refresh()
            TableViewColumn{ role: "id"  ; title: "ID" ; width: 30 }
            TableViewColumn{ role: "table_name"  ; title: "Name" ; width: 60 }
            TableViewColumn{ role: "own_distribution" ; title: "Own distribution" ; width: 80 }
            TableViewColumn{ role: "own_count" ; title: "Own count" ; width: 60 }
            TableViewColumn{ role: "virus_distribution" ; title: "Virus distribution" ; width: 80 }
            TableViewColumn{ role: "virus_count" ; title: "Virus count" ; width: 60 }
            TableViewColumn{ role: "status" ; title: "Status" ; width: 60 }
            TableViewColumn{ role: "has_index" ; title: "Index" ; width: 60 }
            TableViewColumn{ role: "min" ; title: "Min" ; width: 60 }
            TableViewColumn{ role: "max" ; title: "Max" ; width: 60 }
        }

        Chart {
            id: chart;
            width: 400
            height: 400;
            chartType: Charts.ChartType.BAR;
            property int aufloesung: 20
            Component.onCompleted: {
                var labels = new Array(aufloesung);
                for(var i = 0; i < aufloesung; i++)
                {
                    labels[i] = i.toString();
                }
                chartData = {
                    labels: labels,
                    datasets: [
                        {
                            fillColor: "rgba(170,187,205,1)",
                            strokeColor: "rgba(170,187,205,1)",
                            highlightFill: "rgba(170,187,205,1)",
                            highlightStroke: "rgba(170,187,205,1)",
                            data: []
                        }
                    ]
                }
            }

            function reset() {
                chart.chart = null;
                if(tableView.currentRow === -1)
                {
                    return;
                }
                var values = database.getNumbers(tableView.model.getName(tableView.currentRow));
                if(values.length === 0)
                {
                    chartData.datasets[0].data = [];
                    chart.repaint();
                    return
                }
                var min = values[0];
                var max = values[0];
                for(var i = 0; i < values.length; i++)
                {
                    if(values[i] < min)
                    {
                        min = values[i];
                    }
                    if(values[i] > max)
                    {
                        max = values[i];
                    }
                }
                var result = new Array(aufloesung);
                for(i = 0; i < result.length; i++)
                {
                    result[i] = 0;
                }
                var aufloesungRange = max / aufloesung;
                for(i = 0; i < values.length - 1; i++)
                {
                    var rangeClass = Math.floor(values[i] / aufloesungRange);
                    rangeClass = Math.min(rangeClass, aufloesung - 1)
                    result[rangeClass] = result[rangeClass] + 1;
                }
                var labels = new Array(aufloesung);
                for(i = 0; i < aufloesung; i++)
                {
                    labels[i] = i.toString();
                }
                chartData.datasets[0].data = result;
                chart.repaint();
            }
        }
    }

    Component.onCompleted: {
        newTable.send.connect(createTable);
        fillTable.send.connect(fillTableFunc);
        database.onProgress.connect(fillTable.onProgress);
        database.onProgress.connect(virusTable.onProgress);
        database.onFillFinished.connect(fillTable.hide);
        database.onFillFinished.connect(virusTable.hide);
        database.onFillFinished.connect(tableView.refresh);
        virusTable.send.connect(virusTablefunc);
    }

    NewTable {
        id: newTable
        visible: false
    }

    function createTable(tableName, distribution, hasIndex) {
        database.createTable(tableName, distribution, hasIndex);
        tableView.refresh();
    }

    VirusTable {
        id: virusTable
        visible: false
    }

    function virusTablefunc(distribution, quantity, initialClasses, insert) {
        var tableName = tableView.model.getName(tableView.currentRow);
        if(insert)
        {
            database.virusInsert(tableName, distribution, quantity, initialClasses);
        }
        else
        {
            database.virusUpdate(tableName, distribution, quantity, initialClasses);
        }
        tableView.refresh()
    }

    FillTable {
        id: fillTable
        visible: false
    }

    function fillTableFunc(quantity, initialClasses) {
        var tableName = tableView.model.getName(tableView.currentRow);
        database.fillTable(tableName, quantity, initialClasses);
    }

    ProfileWindow {
        id: profileWindow
    }


}
