<?php
    try {
        $pdo = new PDO(
            'mysql:dbname=acc_DB;host=localhost;charset=utf8',
            'select',
            '176520-Kz',
            array(
                PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
                PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
                PDO::ATTR_EMULATE_PREPARES => false
                )
            );

        // select from DB
        $stmt = $pdo->query(
            'SELECT maxAcc, minAcc, DATE_FORMAT(sensorTime, ''%H:%i:%S'')
            FROM acc_DB.sensorVal
            WHERE deviceID=10001
            ORDER BY sensorTime DESC LIMIT 100;'
            );

        $selectData = $stmt->fetchAll();
        $selectMaxAcc = array_column($selectData, 'maxAcc');
        $selectMinAcc = array_column($selectData, 'mixAcc');
        $selectTime = array_column($selectData, 'sensorTime');

        // check update time of latest data
        $stmt = $pdo->query(
            'SELECT sensorTime
            FROM acc_DB.sensorVal
            WHERE deviceID=10001
            ORDER BY sensorTime DESC LIMIT 1;'
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
    <canvas id="myChart" width="600" height="600"></canvas>

    <script>
    var maxAcc = <?php echo json_encode($selectMaxAcc, JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS | JSON_HEX_QUOT); ?>;
    var minAcc = <?php echo json_encode($selectMinAcc, JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS | JSON_HEX_QUOT); ?>;
    var time = <?php echo json_encode($selectTime, JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS | JSON_HEX_QUOT); ?>;
    var upTime = <?php echo json_encode($upTime, JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS | JSON_HEX_QUOT); ?>;
    <?php echo json_encode($upTime, JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS | JSON_HEX_QUOT); ?>

    //graph
    var ctx = document.getElementById("myChart").getContext('2d');
    var chartData = {
        labels: time,
        datasets : [
            {
                label: "Max Acc",
                fillColor: "rgba(220,220,220,0.2)",
                strokeColor: "rgba(220,220,220,1)",
                pointColor: "rgba(220,220,220,1)",
                pointStrokeColor: "#fff",
                pointHighlightFill: "#fff",
                pointHighlightStroke: "rgba(220,220,220,1)",
                data: maxAcc
            },
            {
                label: "Min Acc",
                fillColor: "rgba(151,187,205,0.2)",
                strokeColor: "rgba(151,187,205,1)",
                pointColor: "rgba(151,187,205,1)",
                pointStrokeColor: "#fff",
                pointHighlightFill: "#fff",
                pointHighlightStroke: "rgba(151,187,205,1)",
                data: minAcc
            }
        ]
    }
    var options = {
        scaleOverride : true,
        legendTemplate : "<% for (var i=0; i<datasets.length; i++){%><span style=\"background-color:<%=datasets[i].strokeColor%>\">&nbsp;&nbsp;&nbsp;</span>&nbsp;<%if(datasets[i].label){%><%=datasets[i].label%><%}%><br><%}%>"
    }

    var LineChart = new Chart(ctx, {
        type: 'line',
        data: data,
        options: options
    });

    // update graph every 10sec
    setInterval(process, 10000);

    function process() {
        document.write("Test");
        /*
        $.ajax({
            url: 'chartAjax.php',
            type: 'post',
            data: {
                'time' : upTime
            },
            datatype: 'json',
            timeout: 5000,
        }).done(function(response)) {
        }
        */
    }
    </script>
</body>
</html>
