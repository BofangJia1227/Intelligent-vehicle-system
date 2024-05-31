import QtQuick 2.9

Rectangle {
    id: control

    color: Qt.rgba(0,0,0,0.0)

    property real currentValue: 6

    onCurrentValueChanged: {
        canvas.requestPaint()
    }

    Canvas {
        id: canvas

        antialiasing: true
        width: parent.width
        height: parent.height
        onPaint: {
            var ctx = getContext("2d")
            ctx.reset()

            ctx.lineWidth = 3
            ctx.strokeStyle ="white"
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(40, 40)
            ctx.lineTo(31, 40)
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(41, 70)
            ctx.lineTo(27, 70)
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(42, 100)
            ctx.lineTo(33, 100)
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(43, 130)
            ctx.lineTo(29, 130)
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(44, 160)
            ctx.lineTo(35, 160)
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(45, 190)
            ctx.lineTo(31, 190)
            ctx.stroke()
            ctx.restore()
            ctx.strokeStyle ="red"
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(46, 220)
            ctx.lineTo(37, 220)
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 5
            var gradient = context.createLinearGradient(55,0,62,250)
            gradient.addColorStop(0.0, "white")
            gradient.addColorStop(1.0, "red")
            ctx.strokeStyle = gradient
            ctx.beginPath()
            ctx.moveTo(62, 250)
            ctx.lineTo(62-control.currentValue, 250-(30*control.currentValue))
            ctx.stroke()

            ctx.save()
            ctx.lineWidth = 10
            ctx.strokeStyle = "black"

            for(var i = 0; i < 7;i++ ){
                ctx.save()
                ctx.beginPath()
                ctx.moveTo(60-i,220-(30*i )+ 3)
                ctx.lineTo(60-i,220-(30*i )- 3)
                ctx.stroke()
                ctx.restore()
            }

            ctx.save()
            context.drawImage("C:\Users\admin\PycharmProjects\pythonProject1\Sensor and inspection technology work\imgs\院徽改终.png",8-(control.currentValue),235-(control.currentValue*29),25,20)
            ctx.restore()

        }

    }

}
