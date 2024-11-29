import QtQuick
import QtQuick.Controls

Item {
    visible: true
    width: 640
    height: 480
    y: -170

    property real temp_gaugeValue: 0 // Dinamik yeşil yay değeri (0 ile 1 arasında)
    property real hum_gaugeValue: 0

    Rectangle {
        width: parent.width
        height: 760
        color: "#2a2a2a" // Arka plan rengi
    }

    Canvas {
        id: backgroundCanvas
        width: 300
        height: 150
        anchors.centerIn: parent
        Text {
            x: parent.width / 2 - width / 2  // Metni yatayda ortalar
            y: parent.height / 2 + 40
            text:  temperature_value.temperatureValue + "°C" // temperature_value burada doğrudan double değeri olacak
            font.pixelSize: 25
            font.bold: true
            color: "green"
        }

        onPaint: {
            var ctx = backgroundCanvas.getContext("2d")
            ctx.clearRect(0, 0, width, height)

            // Çeyrek dairelerin başlangıç açısı
            var startAngle = Math.PI; // Başlangıç açısı (sol üst)
            var sliceAngle = Math.PI / 3; // Her dilimin açısı (120 derece)

            // Beyaz dilim
            ctx.beginPath();
            ctx.arc(width / 2, height, 120, startAngle-1, startAngle + sliceAngle+1, false);
            ctx.fillStyle = "#ADD8E6";  // Beyaz
            ctx.fill();

            // Sarı dilim
            startAngle += sliceAngle;  // Sarı dilim için açıyı güncelle
            ctx.beginPath();
            ctx.arc(width / 2, height, 120, startAngle, startAngle + sliceAngle+1, false);
            ctx.fillStyle = "yellow";  // Sarı
            ctx.fill();

            // Kırmızı dilim
            startAngle += sliceAngle;  // Kırmızı dilim için açıyı güncelle
            ctx.beginPath();
            ctx.arc(width / 2, height, 120, startAngle, startAngle + sliceAngle+1, false);
            ctx.fillStyle = "red";  // Kırmızı
            ctx.fill();


            // Gri İç Daire
            ctx.beginPath()
            ctx.arc(width / 2, height, 112, Math.PI, 2 * Math.PI, false)
            ctx.fillStyle = "#505050"  // Gri renkte iç kısım
            ctx.fill()

            // İç Çemberin Kenarı (daha koyu gri)
            ctx.beginPath()
            ctx.arc(width / 2, height, 80, Math.PI, 2 * Math.PI, false)
            ctx.fillStyle = "#2a2a2a"  // Koyu gri kenar çemberi
            ctx.fill()

        }

        Component.onCompleted: backgroundCanvas.requestPaint()
    }

    // Dinamik yeşil alan
    Canvas {
        id: gaugeCanvas
        width: 300
        height: 150
        anchors.centerIn: parent

        onPaint: {
            var ctx = gaugeCanvas.getContext("2d")
            ctx.clearRect(0, 0, width, height)

            // Dinamik yeşil yay çizimi (gri alana oturacak şekilde ayarlandı)
            ctx.beginPath()
            ctx.arc(width / 2, height, 96, Math.PI, Math.PI + temp_gaugeValue * Math.PI, false)
            ctx.lineWidth = 30
            ctx.strokeStyle = "green"
            ctx.stroke()
        }

        Component.onCompleted: gaugeCanvas.requestPaint()
    }

    Connections {
        target: temperature_value // C++'tan gelen context property
        function onTemperatureValueChanged(newValue) {
            console.log("Updated temperature: " + temperature_value.temperatureValue);
            temp_gaugeValue = Math.max(0, Math.min(1, (temperature_value.temperatureValue ) / 100)) // 0-100 arası normalize
            gaugeCanvas.requestPaint() // Canvas'ı yeniden çiz
        }
    }


    // Nem için Canvas
    Canvas {
        id: humidityCanvas
        width: parent.width
        height: 280
        anchors.top: backgroundCanvas.bottom

        Text {
            x: parent.width / 2 - width / 2  // Metni yatayda ortalar
            y: parent.height / 2 + 100
            text: "% " + humidity_value.humidityValue  // temperature_value burada doğrudan double değeri olacak
            font.pixelSize: 25
            font.bold: true
            color: "green"
        }

        onPaint: {
            var ctx = humidityCanvas.getContext("2d")
            ctx.clearRect(0, 0, width, height)

            // Nem grafiği: Çeyrek daire
            var startAngle = Math.PI
            var sliceAngle = Math.PI / 3

            // Beyaz dilim (nem göstergesi)
            ctx.beginPath();
            ctx.arc(width / 2, height, 120, startAngle-1, startAngle + sliceAngle+1, false);
            ctx.fillStyle = "#ADD8E6";  // Beyaz
            ctx.fill();

            // Sarı dilim
            startAngle += sliceAngle;  // Sarı dilim için açıyı güncelle
            ctx.beginPath();
            ctx.arc(width / 2, height, 120, startAngle, startAngle + sliceAngle+1, false);
            ctx.fillStyle = "yellow";  // Sarı
            ctx.fill();

            // Kırmızı dilim
            startAngle += sliceAngle;  // Kırmızı dilim için açıyı güncelle
            ctx.beginPath();
            ctx.arc(width / 2, height, 120, startAngle, startAngle + sliceAngle+1, false);
            ctx.fillStyle = "red";  // Kırmızı
            ctx.fill();


            // Gri İç Daire
            ctx.beginPath()
            ctx.arc(width / 2, height, 112, Math.PI, 2 * Math.PI, false)
            ctx.fillStyle = "#505050"  // Gri renkte iç kısım
            ctx.fill()

            // İç Çemberin Kenarı (daha koyu gri)
            ctx.beginPath()
            ctx.arc(width / 2, height, 80, Math.PI, 2 * Math.PI, false)
            ctx.fillStyle = "#2a2a2a"  // Koyu gri kenar çemberi
            ctx.fill()
        }

        Component.onCompleted: humidityCanvas.requestPaint()
    }

    // Dinamik yeşil alan
    Canvas {
        id: hum_gaugeCanvas
        width: 300
        height: 710
        anchors.centerIn: parent

        onPaint: {
            var ctx = hum_gaugeCanvas.getContext("2d")
            ctx.clearRect(0, 0, width, height)

            var startAngle = Math.PI // Başlangıç açısı (sol üst)
            var sliceAngle = Math.PI / 3 // Her renk diliminin açısı (120 derece)

            // Yeşil dilim
            ctx.beginPath()
            ctx.arc(width / 2, height, 96, Math.PI, Math.PI + hum_gaugeValue * Math.PI, false)
            ctx.lineWidth = 30
            ctx.strokeStyle = "green"
            ctx.stroke()
        }

        Component.onCompleted: hum_gaugeCanvas.requestPaint()
    }

    Connections {
        target: humidity_value // C++'tan gelen context property
        function onHumidityValueChanged(newValue) {
            console.log("Updated humidity: " + humidity_value.humidityValue);
            hum_gaugeValue = Math.max(0, Math.min(1, (humidity_value.humidityValue ) / 100)) // 0-100 arası normalize
            console.log("hum_gaugeValue: " + hum_gaugeValue);
            hum_gaugeCanvas.requestPaint() // Canvas'ı yeniden çiz
        }
    }
}
