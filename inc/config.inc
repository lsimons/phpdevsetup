<?php

define("DEBUG", 1);
define("APP_NAME", "doodle");
if(DEBUG) {
    apc_clear_cache("user");
}

// load_list takes a json file and turns it into a global object cached by APC
//   (modified from http://toys.lerdorf.com/archives/
//   38-The-no-framework-PHP-MVC-framework.html)
function load_json($name) {
    global $$name;
    if(!$$name = apc_fetch($name)) {
        $filename = dirname(__FILE__) . DIRECTORY_SEPARATOR . $name . '.json';
        $$name = json_decode(file_get_contents($filename));
    }
    apc_store($name,$$name);
}

openlog(APP_NAME, LOG_PID | LOG_CONS, LOG_USER);

function warn($message) {
    global $c;
    syslog(LOG_WARNING, $message);
}
function fatal($message) {
    global $c;
    syslog(LOG_ERR, $message);
    die($message);
}

// per-page contextual data
$c = array();
$c["title"] = "Unnamed page";
$c["debug_footer"] = "";

// per-webapp configuration
load_json('settings');

// libraries
require "head.inc";
require "foot.inc";
require "cache.inc";
require "mysql.inc";

// debugging support
if(DEBUG) {
    $c["debug_footer"] .= '<hr style="clear:both" />';
    $c["debug_footer"] .= '<pre>';
    $c["debug_footer"] .= "CONSTANTS\n";
    $constants = get_defined_constants(true);
    foreach($constants["user"] as $k => $v) {
        $c["debug_footer"] .= "  {$k}: {$v}\n";
    }
    $c["debug_footer"] .= '</pre>';

    $c["debug_footer"] .= '<pre>';
    $c["debug_footer"] .= "SETTINGS\n";
    $c["debug_footer"] .= print_r($settings, TRUE);
    $c["debug_footer"] .= '</pre>';
}
?>
