<?php
    try {
        $pdo = new PDO(
            'mysql:dbname=acc_DB;host=localhost;charset=utf8',
            'web',
            '176520-Kz',
            array(
                PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
                PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
                PDO::ATTR_EMULATE_PREPARES => false
                )
            );

        // select from DB
        $stmt = $pdo->query(
            "SELECT maxAcc, minAcc, DATE_FORMAT(sensorTime, '%H:%i:%S')
            FROM acc_DB.sensorVal
            WHERE deviceID=10001
            ORDER BY sensorTime DESC LIMIT 100;"
            );

        $selectData = $stmt->fetchAll();
        // get data and conver acc
        $selectMaxAcc = array_reverse(array_column($selectData, "maxAcc"));
        $selectMinAcc = array_reverse(array_column($selectData, "minAcc"));
        $selectTime = array_reverse(array_column($selectData, "DATE_FORMAT(sensorTime, '%H:%i:%S')"));

        // check update time of latest data
        $stmt = $pdo->query(
            "SELECT sensorTime
            FROM acc_DB.sensorVal
            WHERE deviceID=10001
            ORDER BY sensorTime DESC LIMIT 1;"
            );

        $latestData = $stmt->fetch();
        $upTime = $latestData[sensorTime];

    } catch (PDOException $e) {
        echo 'Connection failed: ' . $e->getMessage();
    }

?>
<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="UTF-8">
    <title>Chart</title>
</head>
<body>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
    <script src="./Chart.min.js"></script>
    <canvas id="lineChart" width="800" height="450"></canvas>

    <script>

    var maxAcc = <?php echo json_encode($selectMaxAcc, JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS | JSON_HEX_QUOT); ?>;
    var minAcc = <?php echo json_encode($selectMinAcc, JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS | JSON_HEX_QUOT); ?>;
    var time = <?php echo json_encode($selectTime, JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS | JSON_HEX_QUOT); ?>;
    var upTime = <?php echo json_encode($upTime, JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS | JSON_HEX_QUOT); ?>;

    //document.writeln(maxAcc);
    //document.writeln(minAcc);
    //document.writeln(time);
    //document.writeln(upTime);

    //graph
    var chartData = {
        labels: time,
        datasets : [
            {
                data: maxAcc,
                label: "Max Acc",
                fill: false,
                backgroundColor: "rgb(255, 99, 132)",
                borderColor: "rgb(255, 99, 132)"
            },
            {
                data: minAcc,
                label: "Min Acc",
                fill: false,
                backgroundColor: "rgb(54, 162, 235)",
                borderColor: "rgb(54, 162, 235)"
            }
        ]
    }
    var ctx = document.getElementById("lineChart").getContext('2d');
    var options = {
        scaleOverride : true,
        legendTemplate : "<% for (var i=0; i<datasets.length; i++){%><span style=\"background-color:<%=datasets[i].strokeColor%>\">&nbsp;&nbsp;&nbsp;</span>&nbsp;<%if(datasets[i].label){%><%=datasets[i].label%><%}%><br><%}%>"
        /*
        scales:{
            yAxes: [
                {
                ticks: {
                    beginAtZero: true,
                    min: 0,
                    max: 3.0
                    }
                }
            ]
        }
        */
    }

    var lineChart = new Chart(ctx, {
            type: 'line',
            data: chartData,
            options: options
    });

    // update graph every 10sec
    setInterval(process, 10000);
    function process() {
        document.write("Test");
        $.ajax({
            url: 'chartAjax.php',
            type: 'post',
            data: {
                'time' : upTime
            },
            datatype: 'json',
            timeout: 5000,
        }).done(function(response)) {
            document.write('aaa');
        }
    }

    </script>
</body>
</html>
