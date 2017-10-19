<?php
header('Content-Type: application/json');

$time = $_POST['time'];

// check POST data
if(empty($time)) {
    echo 'Error Post';
    exit();
}

try {
    // connect DB
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


    // selsect data from DB
    $stmt = $pdo->prepare(
        "SELECT maxAcc, minAcc, DATE_FORMAT(sensorTime, '%H:%i:%S')
        FROM acc_DB.sensorVal
        WHERE deviceID=10001 AND sensorTime > :time
        LIMIT 100;"
        );

    $stmt->bindValue(':time', $time, PDO::PARAM_STR);
    $stmt->execute();

    $selectData = $stmt->fetchAll();
    $selectMaxAcc = array_column($selectData, "maxAcc");
    $selectMinAcc = array_column($selectData, "minAcc");
    $selectTime = array_column($selectData, "DATE_FORMAT(sensorTime, '%H:%i:%S')");

    // check update time of latest data
    $stmt = $pdo->query(
        "SELECT sensorTime
        FROM acc_DB.sensorVal
        WHERE deviceID=10001
        ORDER BY sensorTime DESC LIMIT 1;"
        );

    $latestData = $stmt->fetch();
    $upTime = $latestData[sensorTime];

    $jsonArr = array();
    $jsonArr["maxAcc"] = $selectMaxAcc;
    $jsonArr["minAcc"] = $selectMinAcc;
    $jsonArr["time"] = $selectTime;
    $jsonArr["upTime"] = $upTime;

    echo json_encode($jsonArr);

} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}

?>
