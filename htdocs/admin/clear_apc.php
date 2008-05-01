<?php

header("Content-Type: text/plain");

if(apc_clear_cache("user")) {
    print "success";
} else {
    print "failure";
}

?>