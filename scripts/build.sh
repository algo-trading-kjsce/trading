if [ ! -d "build" ] 
then
    mkdir build
fi

cd build

CC=gcc-10 CXX=g++-10 cmake -DCMAKE_BUILD_TYPE=${1-Debug} -DCMAKE_CXX_FLAGS=${2-"-Wall"} ..

make all
