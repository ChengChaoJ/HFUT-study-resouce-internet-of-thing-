import random

# 生成一个包含50个随机整数的列表，范围是1到100
random_numbers = [random.randint(1, 100) for _ in range(50)]

print("生成的随机整数列表：")
print(random_numbers)

# 删除所有奇数，从后向前遍历
for i in range(len(random_numbers) - 1, -1, -1):
    if random_numbers[i] % 2 != 0:  # 判断是否为奇数
        del random_numbers[i]

print("删除奇数后的列表：")
print(random_numbers)
