<?php

# PHP run in CLI mode needs to be told where PEAR lives
ini_set('include_path', '/opt/local/lib/php' . PATH_SEPARATOR . ini_get('include_path'));

# get the test framework bits and pieces
require 'test-more.php';

# get the PEAR HTTP::Request class to help with making http calls
require "HTTP/Request.php";

# there's just one test here
plan(1);

# make the actual HTTP call
$host = "localhost";
$port = 8529;
$phpinfo_url = "http://{$host}:{$port}/phpinfo.php";

$req =& new HTTP_Request($phpinfo_url);
if (PEAR::isError($req->sendRequest())) {
    # error status -- fail
    fail("phpinfo smoke test");
} else {
    $data=$req->getResponseBody();
    ok(stristr($data, 'phpinfo()'), 'phpinfo generated');
}

?>
