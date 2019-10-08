#!/bin/bash

echo "#ifndef LEVELDATA_H"
echo "#define LEVELDATA_H"
echo "#include <vector>"
echo "const std::vector<std::vector<std::vector<int>>> leveldata = {"
for f in $@
do
  echo "  {"
  sed -e "s/^/    {/" -e "s/$/},/" $f
  echo "  },"
done
echo "};"
echo "#endif"
