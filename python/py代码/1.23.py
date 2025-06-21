import random
import string
import re

def generate_random_string(n):
    # 生成包含字母和数字的随机字符串
    characters = string.ascii_letters + string.digits  # 包含大小写字母和数字
    return ''.join(random.choice(characters) for _ in range(n))

def find_longest_alpha_substring(s):
    # 使用正则表达式查找所有字母子串
    pattern = r'[a-zA-Z]+'  # 匹配字母组成的子串
    alpha_substrings = re.findall(pattern, s)

    # 找到最长的字母子串
    longest_substring = max(alpha_substrings, key=len) if alpha_substrings else ''
    return longest_substring

# 示例调用
n = int(input("请输入随机字符串的长度 n："))
random_string = generate_random_string(n)
print("生成的随机字符串：", random_string)

longest_alpha_substring = find_longest_alpha_substring(random_string)
print("最长的字母子串为：", longest_alpha_substring)
