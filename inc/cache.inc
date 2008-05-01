<?php

/**
 * Connect to all memcache servers.
 */
function cache_connect_all() {
    global $cache_servers, $settings;
    __init_globals();

    foreach($settings->memcache_servers as $name => $server) {
        if(array_key_exists($name, $cache_servers)) continue;
        
        $m = new Memcache;
        $m->connect($server->host, $server->port)
            or warn("Could not connect to {$server->host}:{$server->port}");
        $cache_servers[$name] = $m;
    }
    
    // may not have all servers
    return $cache_servers;
}

/**
 * Connect to one memcache server by name.
 */
function cache_connect($name) {
    global $cache_servers, $settings;
    __init_globals();

    if(!array_key_exists($name, $cache_servers)) {
        $server = $settings->memcache_servers[$name];
        $m = new Memcache;
        $m->connect($server->host, $server->port)
            or warn("Could not connect to {$server->host}:{$server->port}");
        $cache_servers[$name] = $m;
    }

    // may return null on connection failure
    return $cache_servers[$name];
}

function __cache_init_globals() {
    global $cache_servers;
    
    if(!isset($cache_servers)) $cache_servers = array();
}
?>
