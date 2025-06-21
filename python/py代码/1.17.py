def process_words(input_string):
    # 按空格分割单词，并使用set去重
    words = set(input_string.split())
    
    # 将单词按字母顺序排序
    sorted_words = sorted(words)
    
    # 返回排序后的单词
    return ' '.join(sorted_words)

# 接受用户输入
input_string = input("请输入一系列空格分隔的单词：")
result = process_words(input_string)

print("处理后的单词为：", result)
