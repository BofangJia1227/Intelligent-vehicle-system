import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

Page {
    visible: true
    id: page
    width: 1920
    height: 1080

    property real singal: 0

    property real iconPositionY: 300
    property real iconWidth: 25
    property real iconHeight: 25
    property real iconMargin: 50

  Text {
        id: textDisplay
        anchors.centerIn: parent
        text: "点击按钮以显示信息"
    }

    Row {
        id: bottomButtons
        width: parent.width
        height: 50
        spacing: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
}

     // 温度按钮
        Button {
            text: "温度"
            onClicked: {
                // 显示温度信息
                textDisplay.text = "当前温度: " + getTemperature() + " °C"
            }
        }

        // 湿度按钮
        Button {
            text: "湿度"
            onClicked: {
                // 显示湿度信息
                textDisplay.text = "当前湿度: " + getHumidity() + "%"
            }
        }

        // 空气质量按钮
        Button {
            text: "空气质量"
            onClicked: {
                // 显示空气质量信息
                textDisplay.text = "当前空气质量: " + getAirQuality()
            }
        }

        // 光强按钮
        Button {
            text: "光强"
            onClicked: {
                // 显示光强信息
                textDisplay.text = "当前光强: " + getLightIntensity() + " lux"
            }
        }


    Rectangle {
    id: centralRectangle
    x: (parent.width - width) / 2  // 水平居中
    y: (parent.height - height) / 2  // 垂直居中
    width: 200  // 宽度
    height: 100  // 高度
    color: "grey"  // 颜色

    Text {
        id: textInRectangle1
        x: 50  // 文本的局部位置
        y: 25
        width: 100
        height: 50
        text: ppm
        style: Text.Normal
        color: Qt.rgba(0,0,0,0)
        font.weight: Font.ExtraBold
        font.pixelSize: 25
        font.bold: true
        font.family: "Verdana"
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: textInRectangle2
        x: 50
        y: 50
        width: 100
        height: 50
       text: qwe
       style: Text.Normal
        color: Qt.rgba(0,0,0,0)
        font.weight: Font.ExtraBold
        font.pixelSize: 25
        font.bold: true
        font.family: "Verdana"
        horizontalAlignment: Text.AlignHCenter
    }

    // 应用动画效果
    NumberAnimation {
        target: centralRectangle
        property: "opacity"
        from: 0
        to: 1
        duration: 1000
    }
}

    Rectangle {
        id: root
        width: parent.width
        height: parent.height

        color:"black"

        Vehicle_rpm {
            id: vehicle_rpm
            x: (parent.width / 8* 5) + (parent.width/9.6)
            y: parent.height/10*3+20 + (parent.width/9.6)
            width: 0
            height: 0

            Text {
                id: p
                x: 157
                y: 168
                width: 89
                height: 44
                text: "P"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 60
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: rpm
                x: 30
                y: 130
                width: 89
                height: 44
                text: value_rpm.value
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 60
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: rpm1000
                x: 32
                y: 230
                width: 89
                height: 44
                text: "RPM*1000"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 16
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: rpm0
                x: 65
                y: 93
                width: 89
                height: 44
                text: "0"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 25
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: rpm1
                x: 130
                y: 58
                width: 89
                height: 44
                text: "1"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 25
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: rpm2
                x: 206
                y: 66
                width: 89
                height: 44
                text: "2"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 25
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: rpm3
                x: 262
                y: 110
                width: 89
                height: 44
                text: "3"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 25
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: rpm4
                x: 286
                y: 182
                width: 89
                height: 44
                text: "4"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 25
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: rpm5
                x: 263
                y: 255
                width: 89
                height: 44
                text: "5"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 25
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: rpm6
                x: 206
                y: 302
                width: 89
                height: 44
                text: "6"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 25
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: rpm7
                x: 130
                y: 310
                width: 89
                height: 44
                text: "7"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 25
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter

            }

            Rectangle {
                id: warn_rpm
                x: 140
                y: 140
                width: 120
                height: 120
                radius: 160
                color: Qt.rgba(0,0,0,0.0)
                border.width:5
                border.color: Qt.rgba(0,0,0,0.0)
            }
            DropShadow {
                id: shadow_rpm
                anchors.fill: warn_rpm
                horizontalOffset: 0
                verticalOffset: 0
                radius: 1
                samples: 10
                color: Qt.rgba(0,0,0,0.0)
                source: warn_rpm
            }
            ParallelAnimation {
                id: anmi_rpm
                NumberAnimation {
                    loops: 5
                    target: shadow_rpm
                    properties: "radius"
                    from: 50
                    to: 0
                    duration: 1000
                }
            }

        }
        Vehicle_speed {
            id: vehicle_speed
            x: (parent.width / 8 * 3 - parent.width/4.8) + (parent.width/9.6)
            y: parent.height/10*3+20 + (parent.width/9.6)
            width: 0
            height: 0
            Text {
                id: speed
                x: 157
                y: 130
                width: 89
                height: 44
                text: value_speed.value
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 60
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: km
                x: 157
                y: 220
                width: 89
                height: 44
                text: "Km/h"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 20
                font.bold: true
                font.italic: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed0
                x: 288
                y: 268
                width: 89
                height: 44
                text: "0"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed20
                x: 240
                y: 315
                width: 89
                height: 44
                text: "20"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed40
                x: 172
                y: 342
                width: 89
                height: 44
                text: "40"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed60
                x: 97
                y: 330
                width: 89
                height: 44
                text: "60"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed80
                x: 48
                y: 295
                width: 89
                height: 44
                text: "80"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed100
                x: 18
                y: 228
                width: 89
                height: 44
                text: "100"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed120
                x: 18
                y: 148
                width: 89
                height: 44
                text: "120"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed140
                x: 51
                y: 85
                width: 89
                height: 44
                text: "140"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed160
                x: 97
                y: 49
                width: 89
                height: 44
                text: "160"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed180
                x: 172
                y: 38
                width: 89
                height: 44
                text: "180"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed200
                x: 235
                y: 61
                width: 89
                height: 44
                text: "200"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: speed220
                x: 275
                y: 108
                width: 89
                height: 44
                text: "220"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 17
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }

        }

        Vehicle_oil {
            id: vehicle_oil
            x: (parent.width / 8* 6) + 223
            y: parent.height/10*3+20 + 340
            width: 0
            height: 0
            Text {
                id: oil0
                x: -3
                y: 240
                width: 89
                height: 44
                text: "0"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 15
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: oil1
                x: -10
                y: 7
                width: 89
                height: 44
                text: "1"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 15
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Vehicle_temp {
            id: vehicle_temp
            x: (parent.width / 8) - 51
            y: parent.height/10*3+20 + 340
            width: 0
            height: 0
            Text {
                id: temp50
                x: -16
                y: 240
                width: 89
                height: 44
                text: "50"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 15
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: temp130
                x: -5
                y: 3
                width: 89
                height: 44
                text: "130"
                style: Text.Normal
                color: Qt.rgba(0,0,0,0)
                font.weight: Font.ExtraBold
                font.capitalization: Font.MixedCase
                font.pixelSize: 15
                font.bold: true
                font.family: "Verdana"
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Rectangle {
            id: back_border
            width: parent.width
            height: parent.height/2.5
            x: 0
            y: height / 1.5
            color: Qt.rgba(0,0,0,0)
            Image {
                id: left_border
                height:parent.height -(parent.width/10)
                width:height/2.5
                x: parent.width / 2 - 200 - width
                y: (parent.height - height) / 1.5
                source: "./imgs/left.png"
                opacity: 0
            }
            Image {
                id: right_border
                height:parent.height -(parent.width/10)
                width:height/2.5
                x: parent.width / 2 + 200
                y: (parent.height - height) / 1.5
                source: "./imgs/right.png"
                opacity: 0
            }
        }

        Rectangle {
            id: cricle
            width: 200
            height: 200
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            radius: 100
            color: "black"
            border.width: 5
            border.color: "white"
            Image {
                id: logo
                anchors.centerIn: parent
                width: 200
                height: 200
                source: "C:\\Users\\admin\\PycharmProjects\\pythonProject1\\Sensor and inspection technology work\\V3.0\\imgs\\院徽改终(白底).jpg"
                opacity: 0.5
            }
        }

        DropShadow {
            id: shadow
            anchors.fill: cricle
            horizontalOffset: 0
            verticalOffset: 0
            radius: 20
            samples: 16
            color: "white"
            source: cricle
        }

        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: adaptivecurise
                anchors.fill: parent
                source: "./imgs/adaptivecurisey.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 + 1 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: ab
                anchors.fill: parent
                source: "./imgs/Ab.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 - 1 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: abs
                anchors.fill: parent
                source: "./imgs/absy.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 + 2 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: airsac
                anchors.fill: parent
                source: "./imgs/airsacr.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 - 2 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: alertlight
                anchors.fill: parent
                source: "./imgs/alertlighty.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 + 3 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: breakfluide
                anchors.fill: parent
                source: "./imgs/breakfluide.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 - 3 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: cardoor
                anchors.fill: parent
                source: "./imgs/cardoorr.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 + 4 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: collide
                anchors.fill: parent
                source: "./imgs/collider.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 - 4 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: cruisecontrol
                anchors.fill: parent
                source: "./imgs/cruisecontrolg.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 + 5 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: ebd
                anchors.fill: parent
                source: "./imgs/ebdy.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 - 5 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: eps
                anchors.fill: parent
                source: "./imgs/epsy.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 + 6 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: espoff
                anchors.fill: parent
                source: "./imgs/espoffy.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 - 6 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: esp
                anchors.fill: parent
                source: "./imgs/espy.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 + 7 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: foglamp
                anchors.fill: parent
                source: "./imgs/foglampg.png"
                opacity: 1
            }
        }
        Rectangle {
            width: iconWidth
            height: iconHeight
            x: (parent.width - width) / 2 - 7 * iconMargin
            y: iconPositionY
            radius: 25
            color: "black"
            Image {
                id: gearbox
                anchors.fill: parent
                source: "./imgs/gearboxr.png"
                opacity: 1
            }
        }

        SequentialAnimation {
            id: anmi
            ParallelAnimation {
                NumberAnimation {
                    target: logo
                    properties: "opacity"
                    from: 0
                    to: 1
                    duration: 1500
                }
                NumberAnimation {
                    loops: 2
                    target: shadow
                    properties: "radius"
                    from: 30
                    to: 0
                    duration: 1000
                }
            }
            ParallelAnimation {
                NumberAnimation {
                    loops: 2
                    target: shadow
                    properties: "radius"
                    from: 30
                    to: 0
                    duration: 1000
                }
                NumberAnimation {
                    target: left_border
                    properties: "opacity"
                    from: 0
                    to: 1
                    duration: 1500
                }
                NumberAnimation {
                    target: right_border
                    properties: "opacity"
                    from: 0
                    to: 1
                    duration: 1500
                }

                NumberAnimation {
                    target: left_border
                    properties: "height"
                    to: page.height/4
                    duration: 1000
                }

                NumberAnimation {
                    target: right_border
                    properties: "height"
                    to: page.height/4
                    duration: 1000
                }


                NumberAnimation {
                    target: left_border
                    properties: "x"
                    to:  page.width / 2 - 100 - left_border.width
                    duration: 1000
                }

                NumberAnimation {
                    target: right_border
                    properties: "x"
                    to: page.width / 2 + 80
                    duration: 1000
                }

            }
            ParallelAnimation {
                SequentialAnimation {
                    ParallelAnimation {
                        NumberAnimation {
                            target: cricle
                            properties: "width"
                            to: 30
                            duration: 1000
                        }

                        NumberAnimation {
                            target: cricle
                            properties: "height"
                            to: 30
                            duration: 1000
                        }

                        NumberAnimation {
                            target: left_border
                            properties: "height"
                            to: page.height/2.5
                            duration: 700
                        }

                        NumberAnimation {
                            target: right_border
                            properties: "height"
                            to: page.height/2.5
                            duration: 700
                        }


                        NumberAnimation {
                            target: left_border
                            properties: "x"
                            to:  page.width / 8 - page.width/25
                            duration: 700
                        }

                        NumberAnimation {
                            target: left_border
                            properties: "y"
                            to: (page.height/2.5 - left_border.height) / 3.5
                            duration: 700
                        }

                        NumberAnimation {
                            target: right_border
                            properties: "x"
                            to: page.width / 8 * 7 - right_border.width
                            duration: 700
                        }

                        NumberAnimation {
                            target: right_border
                            properties: "y"
                            to: (page.height/2.5 - right_border.height) / 3.5
                            duration: 700
                        }

                        NumberAnimation {
                            target: vehicle_rpm
                            properties: "width"
                            to: page.width/4.8
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_rpm
                            properties: "height"
                            to: page.width/4.8
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_rpm
                            properties: "x"
                            to:  page.width / 8 * 5
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_rpm
                            properties: "y"
                            to:page.height/10*3+20
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_speed
                            properties: "width"
                            to: page.width/4.8
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_speed
                            properties: "height"
                            to: page.width/4.8
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_speed
                            properties: "x"
                            to: page.width / 8 * 3 - page.width/4.8
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_speed
                            properties: "y"
                            to: page.height/10*3+20
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_oil
                            properties: "width"
                            to: 70
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_oil
                            properties: "height"
                            to: 300
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_oil
                            properties: "y"
                            to: page.height/10*3+20 + 90
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_temp
                            properties: "width"
                            to: 70
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_temp
                            properties: "height"
                            to: 300
                            duration: 1000
                        }

                        NumberAnimation {
                            target: vehicle_temp
                            properties: "y"
                            to: page.height/10*3+20 + 90
                            duration: 1000
                        }

                        NumberAnimation {
                            target: cricle
                            properties: "border.width"
                            to: 2
                            duration: 1000
                        }

                        NumberAnimation {
                            target: logo
                            properties: "opacity"
                            to: 0
                            duration: 1
                        }

                        // NumberAnimation {
                        //     target: cricle
                        //     properties: "x"
                        //     to: 100
                        //     duration: 1000
                        // }

                        NumberAnimation {
                            target: cricle
                            properties: "y"
                            to: 298
                            duration: 1000
                        }
                    }
                    ParallelAnimation{
                        NumberAnimation {
                            target: cricle
                            properties: "width"
                            to: 800
                            duration: 500
                            easing.type: "InCirc"
                        }
                        SequentialAnimation{
                            ColorAnimation {
                                target: p
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: rpm
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: rpm1000
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: rpm0
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: rpm1
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: rpm2
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: rpm3
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: rpm4
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: rpm5
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: rpm6
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: rpm7
                                properties: "color"
                                to: "red"
                                duration: 100
                            }
                        }
                        SequentialAnimation{
                            ColorAnimation {
                                target: oil0
                                properties: "color"
                                to: "red"
                                duration: 100
                            }
                            ColorAnimation {
                                target: oil1
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: temp50
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: temp130
                                properties: "color"
                                to: "red"
                                duration: 100
                            }
                        }
                        SequentialAnimation{
                            ColorAnimation {
                                target: speed
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: km
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed0
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed20
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed40
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed60
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed80
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed100
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed120
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed140
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed160
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed180
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed200
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                            ColorAnimation {
                                target: speed220
                                properties: "color"
                                to: "white"
                                duration: 100
                            }
                        }
                    }
                    ParallelAnimation {
                        NumberAnimation {
                            target: vehicle_rpm
                            properties: "currentValue"
                            from:0
                            to: value_rpm.value != 0 ? value_rpm.value : 80
                            duration: 700
                        }
                        NumberAnimation {
                            target: vehicle_speed
                            properties: "currentValue"
                            from:0
                            to: 220
                            duration: 700
                        }
                    }
                    ParallelAnimation {
                        NumberAnimation {
                            target: vehicle_rpm
                            properties: "currentValue"
                            from:80
                            to: 0
                            duration: 800
                        }
                        NumberAnimation {
                            target: vehicle_speed
                            properties: "currentValue"
                            from:220
                            to: 0
                            duration: 800
                        }
                    }
                }
            }
        }

        Timer {
            interval: 500
            running: true
            repeat: true
            onTriggered: {

                if(value_rpm.value != 0){
                    p.text = "D"
                    if(value_rpm.value > 65){
                        anmi_rpm.restart()
                        shadow_rpm.color = "red"
                        warn_rpm.border.color = "red"
                    }else{
                        anmi_rpm.stop()
                        shadow_rpm.color = Qt.rgba(0,0,0,0.0)
                        warn_rpm.border.color = Qt.rgba(0,0,0,0.0)
                    }
                }else{
                    p.text = "P"
                }
                if(singal == 0){
                    anmi.restart()
                    singal = 1
                }
            }
        }

        Text {
            id: time
            color: "white"
            font.pixelSize: 14
            anchors.centerIn: parent
        }

        Slider {
            id: value_rpm
             value: vehicle_rpm.currentValue  // 绑定到从串口读取的RPM数据
        }

        Slider {
            id: value_speed
             value: vehicle_speed.currentValue  // 绑定到从串口读取的速度数据
        }

        Slider {
            id: value_oil
             value: vehicle_oil.currentValue  // 绑定到从串口读取的油量数据
        }

        Slider {
            id: value_temp
            value: vehicle_temp.currentValue  // 绑定到从串口读取的温度数据
        }


    }
}
