#!/bin/bash

cd ../test_set/
cat regex.txt music.txt poem.txt story.txt weather.txt phone.txt nav.txt other.txt > all.txt
cat regex.lab music.lab poem.lab story.lab weather.lab phone.lab nav.lab other.lab > all.lab


mkdir -p ../test_result
cd ../../
source export_so.sh
cd work_flow


echo ">>>>>> 测试性能测试集"
./bin/slu_test -p 7 -u 7 -f ../test/test_set/all.txt -o ../test/test_result/all.out

echo ">>>>>> 测试回归测试集"
./bin/slu_test -p 1 -u 1 -f ../test/test_set/regression.txt -o ../test/test_result/regression.out

cd ../test/src
python3 evaluate_v3.py ../test_set/all.lab ../test_result/all.out > ../test_result/all.err
python3 evaluate_v3.py ../test_set/regression.lab ../test_result/regression.out > ../test_result/regression.err

