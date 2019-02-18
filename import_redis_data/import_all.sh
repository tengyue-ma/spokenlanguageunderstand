#!/bin/bash

for file in `ls dict/music`
do
    ./import_file_data.sh dict/music/$file "music_"$file
done

for file in `ls dict/poem`
do
    ./import_file_data.sh dict/poem/$file "poem_"$file
done

for file in `ls dict/story`
do
    ./import_file_data.sh dict/story/$file "story_"$file
done

for file in `ls dict/sound`
do
    ./import_file_data.sh dict/sound/$file "sound_"$file
done

for file in `ls dict/weather`
do
    ./import_file_data.sh dict/weather/$file $file
done


# 单独导入
#./import_file_data.sh dict/music/language language
