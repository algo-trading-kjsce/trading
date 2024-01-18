#!/bin/bash
if [ -d ".devcontainer" ] 
then
    rm -r .devcontainer/
fi

mkdir .devcontainer

python3 essentials/docker/ubuntu/create_docker_file.py "$1" "$2"

cp essentials/.clang-format algo_trading/.clang-format
