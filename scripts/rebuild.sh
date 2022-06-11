printf "* Cleaning build files...\n\n\n"

if [ -d "build" ] 
then
    rm -r build/
fi

printf "* Finished cleaning build files.\n\n\n"

./scripts/build.sh ${1-Debug}