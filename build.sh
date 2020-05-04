rm -rf build
mkdir build
cd build
cmake ..
make
cd ..

[ $# -eq 1 ] && [ $1 = 'doc' ] && ./documentation.sh

[ $# -eq 1 ] && [ $1 = 'check' ] && python3 test_suite.py -t 2 build/42sh