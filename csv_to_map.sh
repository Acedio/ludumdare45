#!/bin/bash

echo "map->map = {"
sed -e "s/^/{/" -e "s/$/},/" $1
echo "};"
