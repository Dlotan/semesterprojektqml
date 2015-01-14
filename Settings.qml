import QtQuick 2.4

Item {
    visible: false
    id: settings
    property ListModel distributionModel: distributionmodel
    ListModel {
        id: distributionmodel
        ListElement {text: "cauchy"}
        ListElement {text: "cosine"}
        ListElement {text: "weibull"}
        ListElement {text: "normal"}
        ListElement {text: "logNormal"}
        ListElement {text: "parabolic"}
        ListElement {text: "extremeValue"}
        ListElement {text: "exponential"}
        ListElement {text: "doubleLog"}
    }
}

