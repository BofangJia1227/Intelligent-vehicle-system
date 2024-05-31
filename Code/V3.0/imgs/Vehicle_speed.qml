import QtQuick 2.9

Rectangle {
    id: control

    color: Qt.rgba(0,0,0,0.0)

    property real radius: control.width > control.height ? control.height/2-10 : control.width/2-22
    property real currentValue: 0

    onCurrentValueChanged: {
        canvas.requestPaint()
    }

    Canvas {
        id: canvas
        antialiasing: true
        anchors.fill: parent
        width: parent.width
        height: parent.height
        onPaint: {
            var ctx = getContext("2d")
            ctx.reset()



            ctx.lineWidth = 3
            ctx.fillStyle = Qt.rgba(0,0,0,0.0)
            ctx.strokeStyle = Qt.rgba(0,0,0,0.0)
            ctx.translate(width/2, height/2)

            ctx.save()
            ctx.fillStyle = Qt.rgba(0,0,0,0.0)
            ctx.strokeStyle ="#333333"
            ctx.beginPath()
            ctx.arc(0, 0, (parent.radius/2), 0, Math.PI*2)
            ctx.closePath()
            ctx.fill()
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 3
            ctx.strokeStyle = "white"
            ctx.fillStyle = "white"
            ctx.rotate(-Math.PI/3)
            ctx.beginPath()
            for (var i = 0; i <= 220/2; i++)
            {
                if( i != 25 && i != 15){
                    ctx.moveTo(0, control.radius-5)
                    ctx.lineTo(0, control.radius-10)
                    ctx.save()
                    ctx.translate(0, control.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/3)/220*2)
            }
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 3
            ctx.strokeStyle = "#6880E6"
            ctx.fillStyle = "#6880E6"
            ctx.rotate(-Math.PI/3)
            ctx.beginPath()
            for (var i = 0; i <= control.currentValue/2; i++)
            {
                if( i != 25 && i != 15){
                    ctx.moveTo(0, control.radius-5)
                    ctx.lineTo(0, control.radius-10)
                    ctx.save()
                    ctx.translate(0, control.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/3)/220*2)
            }
            ctx.stroke()
            ctx.restore()


            ctx.save()
            ctx.lineWidth = 4
            ctx.strokeStyle = "white"
            ctx.fillStyle = "white"
            ctx.rotate(-Math.PI/3)
            ctx.beginPath()
            for (var i = 0; i <= 220/2; i++)
            {
                if(i % 5 == 0 && i != 25 && i != 15){
                    ctx.moveTo(0, control.radius-5)
                    ctx.lineTo(0, control.radius-13)
                    ctx.save()
                    ctx.translate(0, control.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/3)/220*2)
            }
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 4
            ctx.strokeStyle = "red"
            ctx.fillStyle = "red"
            ctx.rotate(-Math.PI/3)
            ctx.beginPath()
            for (var i = 0; i <= 220/2; i++)
            {
                if(i == 25 || i == 15){
                    ctx.moveTo(0, control.radius-5)
                    ctx.lineTo(0, control.radius-15)
                    ctx.save()
                    ctx.translate(0, control.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/3)/220*2)
            }
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.strokeStyle = "#6880E6"
            ctx.beginPath()
            ctx.arc(0, 0, control.radius, (30/180*Math.PI), (330/180*Math.PI))
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.strokeStyle = "#6880E6"
            ctx.beginPath()
            ctx.arc(0, 0, control.radius+20, (330/180*Math.PI), (30/180*Math.PI))
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.strokeStyle = "#333333"
            ctx.beginPath()
            ctx.arc(0, 0, control.radius, (330/180*Math.PI), (30/180*Math.PI))
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 10
            ctx.strokeStyle = "#9BABEF"
            ctx.beginPath()
            ctx.arc(0, 0, control.radius/1.9, (30/180*Math.PI), ((control.currentValue+22)/132*Math.PI))
            ctx.stroke()

            ctx.save()
            ctx.lineWidth = 5
            ctx.strokeStyle = "white"
            ctx.fillStyle = "white"
            ctx.rotate(-Math.PI/3)
            ctx.rotate(((Math.PI*2-Math.PI/3)/220)*((control.currentValue)) )
            ctx.beginPath()
            ctx.lineTo(0,control.radius-88)
            ctx.lineTo(1,control.radius-88)
            ctx.lineTo(0,control.radius-65)
            ctx.lineTo(-1,control.radius-88)
            ctx.closePath()
            ctx.fill()
            ctx.stroke()
            ctx.restore()

        }
    }
}
