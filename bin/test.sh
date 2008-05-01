#!/usr/bin/env bash

bin/dbinit.sh
t/TEST -configure -documentroot `pwd`/htdocs > /dev/null 2> /dev/null
t/TEST
t/TEST -clean > /dev/null 2> /dev/null
