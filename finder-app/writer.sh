#!/bin/sh

writefile=$1
writestr=$2

if [ $# -lt 2 ];then
    echo "Invalid number of arguments, should be 2.\n1) Path to store file\n2)Word to write to file\n"
    exit 1
fi

new_dir="$(dirname "${writefile}")"

mkdir -p "${new_dir}/"

echo $writestr > "${writefile}"