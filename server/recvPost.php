<?php

$deviceId = $_POST['deviceId'];
$maxAcc = $_POST['maxAcc'];
$minAcc = $_POST['minAcc'];
$sensorTime = $_POST['time'];


// check POST data
if(empty($deviceId) || empty($maxAcc) || empty($minAcc) || empty($sensorTime)) {
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
        )
    );

    // calc k-near
    $cmd ='/home/b1014185/.pyenv/versions/anaconda3-4.1.1/bin//python /var/www/cgi-bin/k-nearest.py'.' '.(string)$maxAcc.' '.(string)$minAcc;
    exec($cmd, $out, $status);
    $kNear = $out[0];

    // check calc res
    if(!ctype_digit($kNear)) {
        $kNear = 100;
    }

    // insert data to DB
    $stmt = $pdo->prepare('INSERT INTO acc_DB.sensorVal (deviceId, maxAcc, minAcc, kNear, sensorTime) VALUES(:deviceId, :maxAcc, :minAcc, :kNear, :sensorTime)');
    $stmt->bindValue(':deviceId', $deviceId, PDO::PARAM_INT);
    $stmt->bindValue(':maxAcc', $maxAcc, PDO::PARAM_STR);
    $stmt->bindValue(':minAcc', $minAcc, PDO::PARAM_STR);
    $stmt->bindValue(':kNear', $kNear, PDO::PARAM_INT);
    $stmt->bindValue(':sensorTime', $sensorTime, PDO::PARAM_STR);

    $stmt->execute();

    echo '[success]: ', $deviceId, ' ', $maxAcc, ' ', $minAcc, ' ', $kNear, ' ', $sensorTime;

} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}


?>
