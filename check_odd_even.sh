#!/bin/bash

read -p "请输入一个整数: " num

if [ -z "$num" ]; then
    echo "错误：你没有输入任何内容。脚本退出。"
    exit 1
fi

remainder=$(($num % 2))

if [ $remainder -eq 0 ]; then
    echo "数字 $num 是一个 偶数 (Even)。"
else
    echo "数字 $num 是一个 奇数 (Odd)。"
fi