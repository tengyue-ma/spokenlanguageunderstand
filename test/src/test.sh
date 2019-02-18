#!/bin/bash

mkdir -p ../test_result
cd ../../
source export_so.sh
cd work_flow

echo -e "\n\n>>>>>> 开始测试……\n"
./bin/slu_test -p 1 -u 1 -f ../test/test_set/music.txt -o ../test/test_result/music.out
./bin/slu_test -p 1 -u 1 -f ../test/test_set/nav.txt -o ../test/test_result/nav.out
./bin/slu_test -p 1 -u 1 -f ../test/test_set/phone.txt -o ../test/test_result/phone.out
./bin/slu_test -p 1 -u 1 -f ../test/test_set/poem.txt -o ../test/test_result/poem.out
./bin/slu_test -p 1 -u 1 -f ../test/test_set/story.txt -o ../test/test_result/story.out
./bin/slu_test -p 1 -u 1 -f ../test/test_set/weather.txt -o ../test/test_result/weather.out
./bin/slu_test -p 1 -u 1 -f ../test/test_set/other.txt -o ../test/test_result/other.out
./bin/slu_test -p 1 -u 1 -f ../test/test_set/regex.txt -o ../test/test_result/regex.out


cd ../test/src
python get_err.py ../test_set/music.lab ../test_result/music.out > ../test_result/music.err
python get_err.py ../test_set/nav.lab ../test_result/nav.out > ../test_result/nav.err
python get_err.py ../test_set/phone.lab ../test_result/phone.out > ../test_result/phone.err
python get_err.py ../test_set/poem.lab ../test_result/poem.out > ../test_result/poem.err
python get_err.py ../test_set/story.lab ../test_result/story.out > ../test_result/story.err
python get_err.py ../test_set/weather.lab ../test_result/weather.out > ../test_result/weather.err
python get_err.py ../test_set/other.lab ../test_result/other.out > ../test_result/other.err
python get_err.py ../test_set/regex.lab ../test_result/regex.out > ../test_result/regex.err

echo -e "\n\n>>>>>> 合并所有测试集错误的句子……\n"
cd ../test_result
cat music.err nav.err phone.err poem.err story.err weather.err other.err regex.err > all.err
