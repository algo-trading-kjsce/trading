printf "* Running cppcheck...\n\n\n"

cppcheck --platform=unix64 --quiet -icpp/gtest cpp

printf "\n* Finished with cppcheck.\n\n\n"

if [ ! -d "build" ] 
then
    mkdir build
fi

cd build

printf "* Creating CMake files...\n\n\n"

CC=gcc-10 CXX=g++-10 cmake -DCMAKE_BUILD_TYPE=${1-Debug} ../cpp ../build

printf "\n\n* Finished creating CMake files.\n\n\n"

printf "* Starting build...\n\n\n"
make -j12 all
printf "\n\n* Finished build.\n"