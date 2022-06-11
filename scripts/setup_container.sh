if [ -d ".devcontainer" ] 
then
    rm -r .devcontainer/
fi

mkdir .devcontainer

cp -a essentials/docker/. .devcontainer/

cp essentials/.clang-format cpp/.clang-format
