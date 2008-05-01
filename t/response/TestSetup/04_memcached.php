<?php

# test that we can connect to memcache from php

$dl_prefix = (PHP_SHLIB_SUFFIX === 'dll') ? 'php_' : '';
if (!extension_loaded('memcache')) {
    try {
        dl($dl_prefix . 'memcache.' . PHP_SHLIB_SUFFIX);
    }
    catch(Exception $e) {
        print "Cannot load memcache: " + $e->getMessage();
    }
}

require 'test-more.php';
plan(3);

$memcache1 = new Memcache;
$memcache1->connect('127.0.0.1', 11211) or die ("Could not connect to 11211");

$memcache2 = new Memcache;
$memcache2->connect('127.0.0.1', 11212) or die ("Could not connect to 11212");

is($memcache1->getVersion(), $memcache2->getVersion(), 'same memcache versions on nodes');

$key = "doodle_test_04_memcached_k1";
$value = "testdata";
$memcache1->set($key, $value, false, 2);

$result = $memcache1->get($key);
is($result, $value, 'got data back from memcache');
sleep(3);

$result = $memcache1->get($key);
ok(!$result, 'data expired from memcache');

?>
