<?php
include "../inc/config.inc";
$c["title"] = "Test";
head();

print "<pre>";
print "READING TEST DATA FROM MASTER\n";
$master = db_connect_master("test");

foreach($master->query("SELECT * FROM Test") as $row) {
    print_r($row);
    print "\n";
}
print "</pre>";



print "<pre>";
print "READING TEST DATA FROM SLAVE\n";
$slave = db_connect_slave("test");

foreach($slave->query("SELECT * FROM Test") as $row) {
    print_r($row);
    print "\n";
}
print "</pre>";

?>

Hello world.

<?php foot(); ?>