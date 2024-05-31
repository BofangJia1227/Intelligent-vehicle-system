import QtQuick 2.9

Rectangle {
    id: controls

    color: Qt.rgba(0,0,0,0.0)

    property real radius: controls.width > controls.height ? controls.height/2-10 : controls.width/2-10
    property real currentValue: 0

    onCurrentValueChanged: {
        canvas.requestPaint()
    }

    Timer {
        id: randomValueTimer
        interval: 1000  // 每1000毫秒（1秒）更新一次
        repeat: true    // 重复执行
        running: true   // 启动定时器
        onTriggered: {
            controls.currentValue = Math.random() * 100;  // 设置currentValue为0到100之间的随机数
        }
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
            ctx.beginPath()
            ctx.arc(0, 0, controls.radius, 0, Math.PI*2, true)
            ctx.closePath()
            ctx.fill()
            ctx.stroke()
            ctx.restore()

            ctx.save()
            var gradient = context.createRadialGradient(0,300,50,100,200,230)
            gradient.addColorStop(0.0, "white")
            gradient.addColorStop(0.7, "#2F75B6")
            gradient.addColorStop(1.0, "#2F65AC")
            ctx.fillStyle = "#6880E6"
            ctx.strokeStyle ="white"
            ctx.beginPath()
            ctx.arc(0, 0, controls.radius/3, 0, Math.PI*2, true)
            ctx.closePath()
            ctx.fill()
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 2
            ctx.strokeStyle = "white"
            ctx.beginPath()
            ctx.arc(0, 0, controls.radius/3-5, 0, Math.PI*2, true)
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 4
            ctx.strokeStyle = "red"
            ctx.beginPath()
            ctx.arc(0, 0, controls.radius, (0.75*Math.PI), (1.25*Math.PI))
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 2
            ctx.strokeStyle = "white"
            ctx.beginPath()
            ctx.arc(0, 0, controls.radius-15, (1.25*Math.PI), (0.75*Math.PI))
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.strokeStyle = "white"
            ctx.beginPath()
            ctx.arc(0, 0, controls.radius-30, (1.25*Math.PI), (0.75*Math.PI))
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 4
            ctx.strokeStyle = "white"
            ctx.fillStyle = "white"
            ctx.rotate(Math.PI/1.33)
            ctx.beginPath()
            for (var i = 0; i <= 240; i++)
            {
                if(i % 30 == 0){
                    ctx.moveTo(0, controls.radius-15)
                    ctx.lineTo(0, controls.radius-30)
                    ctx.save()
                    ctx.translate(0, controls.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/2)/240)
            }
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 2
            ctx.strokeStyle = "white"
            ctx.fillStyle = "white"
            ctx.rotate(Math.PI/1.33)
            ctx.beginPath()
            for (var i = 0; i <= 240; i++)
            {
                if(i > 180 && i < 195){
                    ctx.strokeStyle = "white"
                    ctx.moveTo(0, controls.radius-17)
                    ctx.lineTo(0, controls.radius-28)
                    ctx.save()
                    ctx.translate(0, controls.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/2)/240)
            }
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.lineWidth = 4
            ctx.strokeStyle = "red"
            ctx.rotate(Math.PI/1.33)
            ctx.beginPath()
            for (var i = 0; i <= 240; i++)
            {
                if(i >= 195 && i < 210){
                    ctx.moveTo(0, controls.radius-16)
                    ctx.lineTo(0, controls.radius-29)
                    ctx.save()
                    ctx.translate(0, controls.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/2)/240)
            }
            ctx.stroke()
            ctx.restore()
            ctx.save()
            ctx.lineWidth = 2
            ctx.strokeStyle = "red"
            ctx.rotate(Math.PI/1.33)
            ctx.beginPath()
            for (var i = 0; i <= 160; i++)
            {
                if(i >= 140 && i < 160){
                    ctx.moveTo(0, controls.radius-17)
                    ctx.lineTo(0, controls.radius-28)
                    ctx.save()
                    ctx.translate(0, controls.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/2)/160)
            }
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 3
            ctx.strokeStyle = "white"
            ctx.rotate(Math.PI/1.33)
            ctx.beginPath()
            for (var i = 0; i <= 60; i++)
            {
                if(i % 10 == 0){
                    ctx.moveTo(0, controls.radius-35)
                    ctx.lineTo(0, controls.radius-48)
                    ctx.save()
                    ctx.translate(0, controls.radius-25)
                    ctx.restore()
                }else if(i % 5 == 0){
                    ctx.moveTo(0, controls.radius-35)
                    ctx.lineTo(0, controls.radius-40)
                    ctx.save()
                    ctx.translate(0, controls.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/2)/80)
            }
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 3
            ctx.strokeStyle = "red"
            ctx.rotate(-Math.PI/34)
            ctx.beginPath()
            for (var i = 65; i <= 70; i++)
            {
                if(i % 10 == 0){
                    ctx.moveTo(0, controls.radius-35)
                    ctx.lineTo(0, controls.radius-48)
                    ctx.save()
                    ctx.translate(0, controls.radius-25)
                    ctx.restore()
                }else if(i % 5 == 0){
                    ctx.moveTo(0, controls.radius-35)
                    ctx.lineTo(0, controls.radius-40)
                    ctx.save()
                    ctx.translate(0, controls.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/2)/80)
            }
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.strokeStyle = "white"
            ctx.beginPath()
            ctx.arc(0, 0, controls.radius-85, (1.25*Math.PI), (0.75*Math.PI))
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.strokeStyle = "white"
            ctx.beginPath()
            ctx.arc(0, 0, controls.radius-110, (1.25*Math.PI), (0.75*Math.PI))
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 15
            var gradient = context.createRadialGradient(0,10,10,10,20,230)
            gradient.addColorStop(0.0, "green")
            gradient.addColorStop(0.7, "orange")
            gradient.addColorStop(1.0, "red")
            ctx.strokeStyle = gradient
            ctx.beginPath()
            if(currentValue < 40){
                ctx.arc(0, 0, controls.radius-23,(1.25*Math.PI), ((currentValue+66.7)/53.36*Math.PI))
            }else{
                ctx.arc(0, 0, controls.radius-23,(1.25*Math.PI), ((currentValue-40)/53.3*Math.PI))
            }

            ctx.stroke()

            ctx.save()
            ctx.lineWidth = 2
            ctx.strokeStyle = "white"
            ctx.rotate(Math.PI/1.33)
            ctx.beginPath()
            for (var i = 0; i <= currentValue; i++)
            {
                if(i < 65){
                    ctx.moveTo(0, controls.radius-35)
                    ctx.lineTo(0, controls.radius-45)
                    ctx.save()
                    ctx.translate(0, controls.radius-25)
                    ctx.restore()
                    ctx.rotate((Math.PI*2-Math.PI/2)/80)
                }

            }
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 2
            ctx.strokeStyle = "red"
            ctx.rotate(Math.PI/1.33)
            ctx.beginPath()
            for (var i = 0; i <= currentValue; i++)
            {
                if(i >= 65){
                    ctx.moveTo(0, controls.radius-35)
                    ctx.lineTo(0, controls.radius-45)
                    ctx.save()
                    ctx.translate(0, controls.radius-25)
                    ctx.restore()
                }
                ctx.rotate((Math.PI*2-Math.PI/2)/80)

            }
            ctx.stroke()
            ctx.restore()

            ctx.save()
            ctx.lineWidth = 5
            ctx.strokeStyle = "red"
            ctx.fillStyle = "red"
            ctx.rotate(Math.PI/1.33)
            ctx.rotate(((Math.PI*2-Math.PI/2)/80)*((controls.currentValue)))
            ctx.beginPath()
            ctx.lineTo(0,controls.radius/3-5)
            ctx.lineTo(4,controls.radius/3-5)
            ctx.lineTo(0,controls.radius-55)
            ctx.lineTo(-4,controls.radius/3-5)
            ctx.closePath()
            ctx.fill()
            ctx.stroke()
            ctx.restore()

        }
    }
}
