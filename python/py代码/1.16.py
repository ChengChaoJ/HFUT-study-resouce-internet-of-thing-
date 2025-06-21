import random
import string
from collections import Counter

# 生成包含1000个随机字符的字符串
random_string = ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(1000))

# 统计每个字符出现的次数
char_count = Counter(random_string)

# 打印结果
print("生成的随机字符串：", random_string)
print("每个字符出现的次数：", dict(char_count))
# 接受逗号分隔的单词序列
input_words = input("请输入逗号分隔的单词序列：")

# 使用列表推导式分割并排序
sorted_words = sorted(word.strip() for word in input_words.split(','))

# 按逗号分隔的序列打印单词
output = ','.join(sorted_words)
print("排序后的单词序列：", output)
