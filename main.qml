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
                text: "Add"
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
                id: deleteButton
                x: 8
                y: 106
                text: qsTr("Delete")
                onClicked: {
                    database.deleteTable(tableView.model.getName(tableView.currentRow));
                    tableView.refresh();
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
            TableViewColumn{ role: "own_count" ; title: "Own count" ; width: 80 }
            TableViewColumn{ role: "virus_distribution" ; title: "Virus distribution" ; width: 80 }
            TableViewColumn{ role: "virus_count" ; title: "Virus count" ; width: 80 }
            TableViewColumn{ role: "status" ; title: "Status" ; width: 60 }
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

    NewTable {
        id: newTable
        visible: false
    }
    function createTable(tableName, distribution)
    {
        database.createTable(tableName, distribution);
        tableView.refresh();
    }

    FillTable {
        id: fillTable
        visible: false
    }

    Component.onCompleted: {
        newTable.send.connect(createTable)
        fillTable.send.connect(fillTableFunc)
    }

    function fillTableFunc(quantity, initialClasses) {
        var tableName = tableView.model.getName(tableView.currentRow);
        database.fillTable(tableName, quantity, initialClasses);
        tableView.refresh();
    }

    ProfileWindow {
        id: profileWindow
    }
}
