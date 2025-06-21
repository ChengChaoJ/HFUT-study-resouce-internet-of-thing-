
import random

def generate_tuple(n, m):
    # 使用生成器推导式生成不大于 m 的随机整数，并过滤掉偶数
    result_tuple = tuple(num for num in (random.randint(1, m) for _ in range(n)) if num % 2 != 0)
    return result_tuple

# 示例调用
n = int(input("请输入元组的长度 n："))
m = int(input("请输入每个元素的最大值 m："))

result = generate_tuple(n, m)
print("生成的元组为：", result)
