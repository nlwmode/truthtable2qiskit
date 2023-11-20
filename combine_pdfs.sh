#!/bin/bash

# 检查参数
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 output_image.pdf input_folder"
    exit 1
fi

# 输出图像文件
output_image=$1
input_folder=$2

# 获取文件夹中所有的 PDF 文件
pdf_files=("$input_folder"/*.pdf)

for pdf_file in "${pdf_files[@]}"; do
    echo $pdf_file
done

# 将每个 PDF 转换为图像
images=()
for pdf_file in "${pdf_files[@]}"; do
    image_file=$(mktemp).png
    convert -density 300 "$pdf_file" -quality 100 "$image_file"
    images+=("$image_file")
done

# 使用 montage 整齐排列图像
montage -mode concatenate -tile 1x${#images[@]} "${images[@]}" -geometry +0+0 "$output_image"

# 清理临时图像文件
rm "${images[@]}"
