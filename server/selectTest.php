<?php
    echo 'aaa';
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

        echo 'bbb';
        // select from DB
        $stmt = $pdo->query("SELECT maxAcc, minAcc, DATE_FORMAT(sensorTime, '%H:%i:%S') FROM acc_DB.sensorVal WHERE deviceID=10001 ORDER BY sensorTime DESC LIMIT 100;");
        echo 'ccc';

        //$selectData = $stmt->fetchAll();
        $selectMaxAcc = $stmt->fetchAll(PDO::FETCH_COLUMN, 0);
        $selectMinAcc = $stmt->fetchAll(PDO::FETCH_COLUMN, 1);
        $selectTime = $stmt->fetchAll(PDO::FETCH_COLUMN, 2);

        // check update time of latest data
        $stmt = $pdo->query("SELECT sensorTime FROM acc_DB.sensorVal WHERE deviceID=10001 ORDER BY sensorTime DESC LIMIT 1;");
        echo 'eee';

        $latestData = $stmt->fetch();
        $upTime = $latestData[sensorTime];
        echo $upTime;

        //echo $upTime;
    } catch (PDOException $e) {
        echo 'Connection failed: ' . $e->getMessage();
    }
?>

