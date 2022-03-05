#!/bin/bash

mkdir -p /usr/share/minilex/base

cp base/* /usr/share/minilex/base
cp minilex.py /usr/share/minilex

cat >/usr/share/minilex/config.py <<EOL
base_path = "/usr/share/minilex/base"
output_path = "./src"
EOL

chmod 777 /usr/share/minilex/minilex.py

ln -s /usr/share/minilex/minilex.py /usr/bin/minilex

echo "Done"

