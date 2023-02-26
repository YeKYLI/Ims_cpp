
pkill a.out

g++ -ggdb -Wwrite-strings -I src/ src/main.cc -std=c++11 -pthread -o a.out

./a.out &

python test/test.py


