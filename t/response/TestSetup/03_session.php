<?php

# test that session management is properly enabled

require 'test-more.php';
plan(1);

# create a session
session_start();

# populate data
$_SESSION["test"] = "Stuff goes here";

# done with it
session_write_close();

pass('session writing');
?>
