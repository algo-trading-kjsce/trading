if [ -d "build" ] 
then
    rm -r build/
fi


if [ ! -d "build" ] 
then
    mkdir build
fi

cd build

CC=gcc-10 CXX=g++-10 cmake -DCMAKE_BUILD_TYPE=${1-Debug} ../cpp ../build

make -j12 all
