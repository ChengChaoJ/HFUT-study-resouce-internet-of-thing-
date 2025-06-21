def write_strings_to_file(file_path, strings):
    """将字符串列表写入文件"""
    with open(file_path, 'w', encoding='utf-8') as file:
        for string in strings:
            file.write(string + '\n')  # 每个字符串换行写入

def read_strings_from_file(file_path):
    """从文件中读取字符串，并返回字符串个数"""
    with open(file_path, 'r', encoding='utf-8') as file:
        lines = file.readlines()  # 读取所有行
        return [line.strip() for line in lines]  # 去除每行末尾的换行符

def main():
    # 生成多个字符串
    strings = ["Hello, World!", "Python is great.", "File handling in Python.", "Let's count strings.", "Goodbye!"]

    # 文件路径
    file_path = 'strings.txt'

    # 写入字符串到文件
    write_strings_to_file(file_path, strings)
    print(f"{len(strings)} 个字符串已写入到 {file_path}。")

    # 从文件读取字符串并统计数量
    read_strings = read_strings_from_file(file_path)
    print(f"从文件中读取到 {len(read_strings)} 个字符串。")

if __name__ == "__main__":
    main()
