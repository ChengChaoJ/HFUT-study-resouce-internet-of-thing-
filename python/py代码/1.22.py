import re

def find_three_letter_words(text):
    # 使用正则表达式查找所有长度为3的单词
    pattern = r'\b[a-zA-Z]{3}\b'  # 匹配长度为3的单词（字母组成）
    three_letter_words = re.findall(pattern, text)

    return three_letter_words

# 用户输入
input_text = input("请输入一段英文：")
result = find_three_letter_words(input_text)

# 输出结果
print("长度为3的单词有：", result)
