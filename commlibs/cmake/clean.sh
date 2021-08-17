#!/bin/sh
#

find -type f | grep -v "sh$" | grep -v ".\/.git" | xargs chmod 664
find . -name "*~" -exec rm -f {} \;
find . -name "*.orig" -exec rm -f {} \;
