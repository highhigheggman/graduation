<?php

$deviceId = $_POST['deviceId'];
$maxAcc = $_POST['maxAcc'];
$minAcc = $_POST['minAcc'];
$sensorTime = $_POST['time'];

echo $deviceId, ' ', $maxAcc, ' ', $minAcc, ' ', $sensorTime;

// check POST data
if(empty($deviceId) || empty($maxAcc) || empty($minAcc) || empty($sensorTime)) {
    echo 'Error';
    exit();
}

try {

    // connect DB
    $pdo = new PDO(
        'mysql:dbname=acc_DB;host=localhost;charset=utf8mb4',
        'postManager',
        '176520-Kz',
        [
            PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
            PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
            PDO::ATTR_EMULATE_PREPARES => false,
        ]
    );

    // insert data to DB
    $stmt = $pdo->prepare('insert into acc_DB.sensorVal (deviceId, maxAcc, minAcc, sensorTime) values(:deviceId, :maxAcc, :minAcc, :sensorTime)');
    //$stmt->bindValue(':deviceId', $deviceId, PDO::PARAM_INT);
    //$stmt->bindValue(':maxAcc', $maxAcc, PDO::PARAM_STR);
    //$stmt->bindValue(':minAcc', $minAcc, PDO::PARAM_STR);
    //$stmt->bindValue(':sensorTime', $sensorTime, PDO::PARAM_STR);
    $stmt->bindValue(':deviceId', '12345', PDO::PARAM_INT);
    $stmt->bindValue(':maxAcc', '2345', PDO::PARAM_STR);
    $stmt->bindValue(':minAcc', '6789', PDO::PARAM_STR);
    $stmt->bindValue(':sensorTime', '2017-5-09 11:23:34', PDO::PARAM_STR);

    $stmt->execute();

} catch (PDOException $e) {
    echo 'SQL';
    echo 'Connection failed: ' . $e->getMessage();
}

?>
