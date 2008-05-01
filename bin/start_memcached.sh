#!/usr/bin/env bash

hostname=127.0.0.1
memperdaemon=128
daemonport1=11211
daemonport2=11212

memcached -d -l $hostname -m $memperdaemon -p $daemonport1
memcached -d -l $hostname -m $memperdaemon -p $daemonport2
