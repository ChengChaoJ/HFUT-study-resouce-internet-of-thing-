def convert_case(input_file, output_file):
    try:
        # 读取文本文件内容
        with open(input_file, 'r', encoding='utf-8') as file:
            content = file.read()

        # 反转大小写
        converted_content = content.swapcase()

        # 将转换后的内容写入输出文件
        with open(output_file, 'w', encoding='utf-8') as file:
            file.write(converted_content)
        
        print(f"成功反转了文件 {input_file} 中的字母，并将结果写入到 {output_file}。")
    
    except FileNotFoundError:
        print(f"文件 {input_file} 不存在，请检查文件路径。")
    except Exception as e:
        print(f"发生错误: {e}")

def main():
    input_file = 'input.txt'      # 输入文件名
    output_file = 'output.txt'    # 输出文件名

    convert_case(input_file, output_file)

if __name__ == "__main__":
    main()
