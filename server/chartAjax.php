<?php
header('Content-Type: application/json');

$time = $_POST['time'];


// check POST data
if(empty($sensorTime)) {
    echo 'Error Post';
    exit();
}

try {
    // connect DB
    $pdo = new PDO(
        'mysql:dbname=acc_DB;host=localhost;charset=utf8',
        'postManager',
        '176520-Kz',
        array(
            PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
            PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
            PDO::ATTR_EMULATE_PREPARES => false
        )
    );


    // selsect data from DB
    // select from DB
    $stmt = $pdo->prepare(
        'SELECT maxAcc, minAcc, DATE_FORMAT(sensorTime, ''%H:%i:%S'')
        FROM acc_DB.sensorVal
        WHERE deviceID=10001 AND sensorTime > :time
        ORDER BY sensorTime DESC LIMIT 100;'
        );

    $stmt->bindValue(':time', $time, PDO::PARAM_STR);
    $stmt->execute();

    $selectData = $stmt->fetchAll();
    $selectMaxAcc = array_column($selectData, 'maxAcc');
    $selectMinAcc = array_column($selectData, 'mixAcc');
    $selectTime = array_column($selectData, 'sensorTime');

} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}

?>
