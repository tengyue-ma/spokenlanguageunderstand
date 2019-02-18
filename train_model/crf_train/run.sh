#!/bin/sh
./bin/crf_feat_test -f music_data.txt -o train.data
./bin/crf_learn -c 10.0 template train.data model
#../../crf_test  -m model test.data

#../../crf_learn -a MIRA template train.data model
#../../crf_test  -m model test.data
