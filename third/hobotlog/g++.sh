set -ex
g++ test.cpp -c -I./include/ -fPIC
file=`ls lib/*.o`
#g++ -shared -fPIC test.o $file -o libtest.so  -std=c++11 -lpthread
g++ -shared -fPIC test.o -o libtest.so -llogmodule -llog4cpp -std=c++11 -L./lib -lpthread
