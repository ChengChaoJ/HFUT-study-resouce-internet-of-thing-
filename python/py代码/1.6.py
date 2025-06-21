import random

# 生成一个包含20个随机整数的列表，范围是1到100
random_numbers = [random.randint(1, 100) for _ in range(20)]

print("生成的随机整数列表：")
print(random_numbers)

# 提取偶数下标的元素
even_indexed_elements = [random_numbers[i] for i in range(0, len(random_numbers), 2)]

# 对偶数下标的元素进行降序排序
even_indexed_elements.sort(reverse=True)

# 将排序后的偶数下标元素放回到原列表中
for i, value in enumerate(even_indexed_elements):
    random_numbers[i * 2] = value

print("偶数下标元素降序排列后的列表：")
print(random_numbers)
