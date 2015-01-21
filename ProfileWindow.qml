import QtQuick 2.4
import QtQuick.Controls 1.3
import jbQuick.Charts 1.0
import QtQuick.Layouts 1.1

ApplicationWindow {
    width: 700
    height: 500
    visible: false
    property var colors: [
        {
            colorName: "red",
            rgb: "rgba(255,0,0"
        },
        {
            colorName: "lime",
            rgb: "rgba(0,255,0"
        },
        {
            colorName: "blue",
            rgb: "rgba(0,0,255"
        },
        {
            colorName: "yellow",
            rgb: "rgba(255,255,0"
        },
        {
            colorName: "cyan",
            rgb: "rgba(0,255,255"
        },
        {
            colorName: "magenta",
            rgb: "rgba(255,0,255"
        }
    ]
    property var valuesCopy: null
    function initialize(values) {
        valuesCopy = values;
        chart.chart = null;
        var labels = [];
        for(var name in values)
        {
            for(var i = 0; i < values[name].length; i++)
            {
                labels.push(i.toString());
            }

            break;
        }
        var datasets = [];
        i = 0;
        repeaterModel.clear();
        for(name in values)
        {
            var dataset = {};
            dataset['label'] = name;
            dataset['fillColor'] = colors[i].rgb.concat(",0.2)");
            dataset['strokeColor'] = colors[i].rgb.concat(",1)");
            var data = [];
            for(var j = 0; j < values[name].length; j++)
            {
                data.push(values[name][j]);
            }
            // Legende.
            dataset['data'] = data;
            datasets.push(dataset);
            var temp = {};
            temp['name'] = name;
            temp['colorName'] = colors[i].colorName;
            temp['values'] = data;
            repeaterModel.append(temp);
            i++;
        }
        chart.chartData = {
            labels: labels,
            datasets: datasets
        }
        chart.repaint();
    }
    ListModel {
        id: repeaterModel
    }
    RowLayout {
        RowLayout {
            anchors.top: parent.top
            Repeater {
                model: repeaterModel
                Column {
                    spacing: 4
                    Text {
                        width: 120
                        text: "Name: " + name + " "
                    }
                    Repeater {
                        model: valuesCopy[name].length
                        Text {
                            text: valuesCopy[name][index]
                        }
                    }
                }
            }
        }
        ColumnLayout {
            Chart {
                id: chart;
                width: 600
                height: 400;
                chartType: Charts.ChartType.BAR;
                chartOptions: {}
                property int aufloesung: 20
                Component.onCompleted: {
                }
            }
            Repeater {
                id: repeater
                model: repeaterModel
                Row {
                    spacing: 5
                    Text {
                        text: "█"
                        color: colorName
                    }
                    Text {
                        text: "Name: " + name
                    }

                }
            }
        }
    }
}

