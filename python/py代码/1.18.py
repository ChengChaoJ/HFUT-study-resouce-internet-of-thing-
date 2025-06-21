import random
import string

def generate_nested_list(n, m):
    nested_list = []
    
    for _ in range(n):
        # 生成一个随机长度1到m的字符串
        length = random.randint(1, m)
        random_string = ''.join(random.choices(string.ascii_letters + string.digits, k=length))
        nested_list.append([random_string])  # 每个元素是一个列表，包含这个字符串

    # 按照字符串长度降序排序
    nested_list.sort(key=lambda x: len(x[0]), reverse=True)
    
    return nested_list

# 示例调用
n = int(input("请输入嵌套列表的元素个数 n："))
m = int(input("请输入字符串的最大长度 m："))

result = generate_nested_list(n, m)

print("生成的嵌套列表为：")
for item in result:
    print(item)
