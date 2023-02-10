#!/bin/bash
if [ -d ".devcontainer" ] 
then
    rm -r .devcontainer/
fi

mkdir .devcontainer

cp -aru essentials/docker/ubuntu/. .devcontainer/
python3 .devcontainer/create_docker_file.py "$1" "$2"

cp -u essentials/.clang-format algo_trading/.clang-format
