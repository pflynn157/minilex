#!/bin/bash

if [[ -f /usr/bin/minilex ]] ; then
    rm /usr/bin/minilex
fi

if [[ -d /usr/share/minilex ]] ; then
    rm -rf /usr/share/minilex
fi

mkdir -p /usr/share/minilex/base

cp base/* /usr/share/minilex/base
cp minilex.py /usr/share/minilex

chmod 777 /usr/share/minilex/minilex.py

ln -s /usr/share/minilex/minilex.py /usr/bin/minilex

echo "Done"

