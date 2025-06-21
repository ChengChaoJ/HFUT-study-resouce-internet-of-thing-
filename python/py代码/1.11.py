import random

def generate_random_list_and_average(n):
    # 随机生成包含n个整数的列表，范围为1到100
    random_list = [random.randint(1, 100) for _ in range(n)]
    
    # 打印生成的随机列表
    print("生成的随机列表为：", random_list)

    # 计算平均值
    average = sum(random_list) / n

    # 过滤出所有大于平均值的整数
    above_average = [x for x in random_list if x > average]

    # 返回元组，包含平均值和大于平均值的整数
    return (average, *above_average)

# 示例调用
n = int(input("请输入随机产生的整数个数 n："))
result = generate_random_list_and_average(n)
print("生成的元组为：", result)
