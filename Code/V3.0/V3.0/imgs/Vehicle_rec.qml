import QtQuick 2.9

Rectangle {
    id: control
    // ... 其他属性和设置 ...
    color: Qt.rgba(0,0,0,0.0)

    property real airQuality: 75   // 假设的空气质量值
    property real lightIntensity: 100  // 假设的光照强度值

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

            // 绘制中央矩形
            var rectWidth = 200;
            var rectHeight = 150;
            var rectX = (width - rectWidth) / 2;
            var rectY = (height - rectHeight) / 2;
            ctx.fillStyle = "white";
            ctx.fillRect(rectX, rectY, rectWidth, rectHeight);
             ctx.save()
            ctx.lineWidth = 5
            // 设置文本样式
            ctx.fillStyle = "black";
            ctx.font = "20px Arial";
            ctx.textAlign = "center";


            // 绘制空气质量文本
            ctx.fillText("空气质量: " + controls.airQuality, rectX + rectWidth / 2, rectY + 90);

            // 绘制光照强度文本
            ctx.fillText("光照强度: " + controls.lightIntensity, rectX + rectWidth / 2, rectY + 120);

             ctx.save()
            context.drawImage("C:\Users\admin\PycharmProjects\pythonProject1\Sensor and inspection technology work\imgs\院徽改终.png",8-(control.currentValue),235-(control.currentValue*29),25,20)
            ctx.restore()
        }
    }
}
