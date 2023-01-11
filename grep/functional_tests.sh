#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"VAR test1.txt"
"VAR s21_grep.h"
"VAR patterns1.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep "end" $t > s21_grep_results.txt
    grep "end" $t > grep_results.txt
    DIFF_RES="$(diff -s s21_grep_results.txt grep_results.txt)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Файлы s21_grep_results.txt и grep_results.txt идентичны" ]
    then
      (( SUCCESS++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
}

#no params
for i in "${tests[@]}"
    do
        testing $i
    done

# 1 param
for var1 in c h i l n o s v
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 params
for var1 in c h i l n o s v
do
    for var2 in c h i l n o s v
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 params
for var1 in c h i l n o s v
do
    for var2 in c h i l n o s v
    do
        for var3 in c h i l n o s v
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done


echo -e "\033[31mFAIL: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"