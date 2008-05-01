<?php

# test that PDO works and can cannect to our DBs

/*$dl_prefix = (PHP_SHLIB_SUFFIX === 'dll') ? 'php_' : '';
if (!extension_loaded('pdo')) {
    dl($dl_prefix . 'pdo.' . PHP_SHLIB_SUFFIX);
}
if (!extension_loaded('pdo_mysql')) {
    dl($dl_prefix . 'pdo_mysql.' . PHP_SHLIB_SUFFIX);
}*/

require 'test-more.php';
plan(2);

try {
    $dbh = new PDO('mysql:host=127.0.0.1;port=3306;dbname=test',
            'test', 'test',
            array(
                PDO::ATTR_PERSISTENT => true,
                PDO::ERRMODE_EXCEPTION => true
            )
        );
    
    $affected = $dbh->exec("INSERT INTO Test (value) VALUES ('something');");

    $count = 0;
    foreach($dbh->query('SELECT * FROM Test;') as $row) {
        if(!$row) {
            fail("pdo master database connection setup");
            exit;
        }
        $count++;
    }
    $dbh = null;
    pass("pdo master database connection setup");
    
    
    // now look at a slave...
    sleep(1);
    
    $dbh = new PDO('mysql:host=127.0.0.1;port=3307;dbname=test',
            'test', 'test',
            array(
                PDO::ATTR_PERSISTENT => true,
                PDO::ERRMODE_EXCEPTION => true
            )
        );
    $slave_count = 0;
    foreach($dbh->query('SELECT * FROM Test;') as $row) {
        if(!$row) {
            fail("pdo slave database connection setup");
            exit;
        }
        $slave_count++;
    }
    $dbh = null;

    is($count, $slave_count, "slave follows master");
    
    
} catch (PDOException $e) {
    //print "Error!: " . $e->getMessage() . "<br/>";
    fail("pdo database connection setup");
}

?>
