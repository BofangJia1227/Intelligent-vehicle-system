import QtQuick 2.9

Rectangle {
    id: control

    color: Qt.rgba(0,0,0,0.0)

    property real currentValue: 90

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
            ctx.strokeStyle ="red"
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(26, 40)
            ctx.lineTo(35, 40)
            ctx.stroke()
            ctx.restore()
            ctx.strokeStyle ="white"
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(25, 70)
            ctx.lineTo(39, 70)
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(24, 100)
            ctx.lineTo(33, 100)
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.lineWidth = 4
            ctx.beginPath()
            ctx.moveTo(23, 130)
            ctx.lineTo(37, 130)
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.lineWidth = 3
            ctx.beginPath()
            ctx.moveTo(22, 160)
            ctx.lineTo(31, 160)
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(21, 190)
            ctx.lineTo(35, 190)
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.beginPath()
            ctx.moveTo(20, 220)
            ctx.lineTo(29, 220)
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 5
            var gradient = context.createLinearGradient(5, 250, 12, 10)
            gradient.addColorStop(0.0, "blue")
            gradient.addColorStop(0.5, "white")
            gradient.addColorStop(1.0, "red")
            ctx.strokeStyle = gradient
            ctx.beginPath()
            ctx.moveTo(5, 250)
            ctx.lineTo(5+((control.currentValue-50)/10), 250-(30*((control.currentValue-50)/10)))
            ctx.stroke()

            ctx.save()
            ctx.lineWidth = 10
            ctx.strokeStyle = "black"

            for(var i = 0; i < 7;i++ ){
                ctx.save()
                ctx.beginPath()
                ctx.moveTo(6+i,220-(30*i )+ 3)
                ctx.lineTo(6+i,220-(30*i )- 3)
                ctx.stroke()
                ctx.restore()
            }

            ctx.save()
            context.drawImage("./imgs/watertemw.png",35+((control.currentValue-50)/10),220-((control.currentValue-50)/10*27.5),25,25)
            ctx.restore()

        }

    }

}
