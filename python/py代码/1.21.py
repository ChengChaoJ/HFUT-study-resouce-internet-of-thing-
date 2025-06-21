def word_count(text):
    # 将输入文本转换为小写并分割成单词
    words = text.split()
    
    # 创建一个空字典用于存储单词及其计数
    count_dict = {}

    # 遍历每个单词并统计出现次数
    for word in words:
        # 对单词进行处理，去掉标点符号（如果有的话）
        word = word.strip().lower()  # 转换为小写以避免大小写影响计数
        if word in count_dict:
            count_dict[word] += 1  # 如果单词已存在，则计数加1
        else:
            count_dict[word] = 1  # 新单词，初始化为1

    return count_dict

# 示例调用
input_text = input("请输入一段英文：")
result = word_count(input_text)

# 输出结果
for word, count in result.items():
    print(f"{word}: {count}")
