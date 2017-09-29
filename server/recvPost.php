<?php

$deviceId = $_POST['deviceId'];
$maxAcc = $_POST['maxAcc'];
$minAcc = $_POST['minAcc'];
$sensorTime = $_POST['time'];

echo 'hello';

// check POST data
if(empty($deviceId) || empty($maxAcc) || empty($minAcc) || empty($sensorTime)) {
    echo 'error';
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
        ]
    );

    // use Prepares
    $pdo->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);

    $stmt = $pdo->prepare('insert into acc_DB.sensorVal (deviceID, maxAcc, minAcc, sensorTime) values(:deviceId, :maxAcc, :minAcc, :sensorTime)');
    $stmt->bindValue(':deviceId', $deviceId, PDO::PARAM_INT);
    $stmt->bindValue(':maxAcc', $maxAcc, PDO::PARAM_STR);
    $stmt->bindValue(':minAcc', $minAcc, PDO::PARAM_STR);
    $stmt->bindValue(':sensorTime', $sensorTime, PDO::PARAM_STR);

    $stmt->execute();

} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}

?>
