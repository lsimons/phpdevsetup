<?php

/**
 * Connect to all mysql servers (masters and slaves) for a specific data source.
 */
function db_connect_all($datasource) {
    global $db_servers, $settings;
    __db_init_globals($datasource);
    
    $ds = $settings->mysql_servers->$datasource;

    foreach($ds->servers as $name => $server) {
        if(array_key_exists($name, $db_servers[$datasource])) continue;
        
        $db = __db_do_connect($datasource, $name, $server->host, $server->port, 
                $ds->username, $ds->password);
        if($db != NULL) {
            $db_servers[$datasource][$name] = $db;
        }
    }
    
    // may not have all slave servers
    return $db_servers[$datasource];
}

/**
 * Connect to one named mysql server for a specific data source.
 */
function db_connect($datasource, $name) {
    global $db_servers, $settings;
    __db_init_globals($datasource);

    if(!array_key_exists($name, $db_servers[$datasource])) {
        $ds = $settings->mysql_servers->$datasource;
        $server = $ds->servers->$name;
        $db = __db_do_connect($datasource, $name, $server->host, $server->port, 
                $ds->username,
                $ds->password);
        if($db != NULL) {
            $db_servers[$datasource][$name] = $db;
        }
    }

    // may return null on slave connection failure
    return $db_servers[$datasource][$name];
}

/**
 * Get a connection to a database master.
 */
function db_connect_master($datasource) {
    return db_connect($datasource, "master");
}


/**
 * Get a connection to a database slave.
 */
function db_connect_slave($datasource) {
    global $db_servers, $settings;
    __db_init_globals($datasource);

    // about once every 5 times, we give ourselves an oppurtunity to 'discover' a new slave
    if(rand(0,4) != 0) {
        // can we find one or more connected slaves? If so pick one at random
        $connected_slaves = array();
        foreach($db_servers[$datasource] as $name => $db) {
            if($name == "master") { // prefer a slave
                continue;
            }
            $connected_slaves[] = $db;
        }
        if(count($connected_slaves) > 0) {
            return array_rand($connected_slaves);
        }
    }
    
    // apparently no connected slaves yet...

    // look through the config for a slave server to connect to
    $ds = $settings->mysql_servers->$datasource;
    $slaves = array();
    foreach($ds->servers as $name => $server) {
        if($name == "master") { // we really prefer a slave...
            continue;
        }
        $slaves[] = $name;
    }
    shuffle($slaves);
    
    $slaves[] = "master"; // ...but we'll add the master as last resort
    foreach($slaves as $name) {
        $db = db_connect($datasource, $name);
        if($db != NULL) {
            return $db;
        }
    }

    // won't happen
    fatal("We should've died already since asking db_connect for a master should" .
          " not return NULL!");
}

function __db_init_globals($datasource) {
    global $db_servers;
    
    if(!isset($db_servers)) $db_servers = array();
    if(!isset($db_servers[$datasource])) $db_servers[$datasource] = array();
}

function __db_do_connect($datasource, $name, $host, $port, $username, $password) {
    try {
        return new PDO("mysql:host={$host};port={$port};dbname={$datasource}",
                $username, $password, array(
                    PDO::ATTR_PERSISTENT => true,
                    PDO::ERRMODE_EXCEPTION => true
                )
            );
    } catch(PDOException $e) {
        if($name == "master") {
            fatal("Cannot connect to datasource $datasource master: {$e->getMessage()}");
        } else {
            warn("Cannot connect to datasource $datasource slave $name: {$e->getMessage()}");
        }
        return NULL;
    }
}

?>
