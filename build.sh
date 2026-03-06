# С использованием CMake
if [ ! -d build ]; then
    $(mkdir build)
fi
cd build

cmake ..
make

cp ../test/inlet.in ../build/inlet.in
./list_serializer
