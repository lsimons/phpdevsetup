#!/usr/bin/env bash

mysql $MYSQL_ARGS < sql/DB.sql
mysql $MYSQL_ARGS < sql/GRANTS.sql
mysql $MYSQL_ARGS test < sql/test_ddl.sql
