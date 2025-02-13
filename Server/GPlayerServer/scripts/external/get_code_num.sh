#! /bin/bash
set -e 

now_dir="."
if [ $# -eq 0 ];then
    now_dir="."
elif [ $# -eq 1 ];then
    now_dir=$1
elif [ $# -gt 1 ];then
    echo "useage: ./getCodeNum.sh [input_path]"
    echo "example: ./getCodeNum.sh obs"
    exit
fi
cd $now_dir

# 获取当前目录名称
filename=$(basename $(pwd))

# 定义一个函数来统计文件个数和行数  
function count_files_and_lines {  
    local ext=$1  
    local file_count=$(find . -name "$ext" | wc -l)  
    local line_count=$(find . -name "$ext" -exec cat {} + | grep -v ^$ | wc -l)  
    echo $file_count $line_count  
}  

# 定义文件扩展名数组  
extensions=("*.c" "*.cc" "*.cxx" "*.cpp" "*.h" "*.h++" "*.hpp" "*.md")  

# 初始化统计变量  
total_files=0  
total_lines=0  
declare -a counts  
declare -a lines  

# 统计每种类型的文件和代码行数  
for ext in "${extensions[@]}"; do  
    read count line < <(count_files_and_lines "$ext")  
    counts+=($count)  
    lines+=($line)  
    total_files=$((total_files + count))  
    total_lines=$((total_lines + line))  
done  

# 连接统计数组元素并添加分隔符 |  
exten_output=$(IFS='|'; echo "| ${extensions[*]} |")  
counts_output=$(IFS='|'; echo "| ${counts[*]} |")  
lines_output=$(IFS='|'; echo "| ${lines[*]} |")  

length=${#extensions[@]}  
length_output="|---|---|"
for ((i=0; i<length; i++)); do
    length_output+="---|"
done

# 使用 Markdown 表格呈现结果  
echo "**${filename}中代码统计**"  
echo "| ${filename} ${exten_output}  **总和** |"  
echo $length_output
echo "${counts_output} **${total_files}** |"  
echo "${lines_output} **${total_lines}** |"  