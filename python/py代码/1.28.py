def calculate_sum(a, n):
    total_sum = 0

    for i in range(1, n + 1):
        # 生成数字 aa..a (i个a)
        current_number = int(str(a) * i)  # 将a转换为字符串并重复i次，然后转换为整数
        total_sum += current_number

    return total_sum

# 用户输入
a = int(input("请输入一个整数（1-9之间）："))
n = int(input("请输入要相加的数字个数："))

if 1 <= a <= 9 and n > 0:
    result = calculate_sum(a, n)
    print(f"计算结果 s = {result}")
else:
    print("输入无效，请确保 a 是 1-9 之间的整数，n 是大于0的整数。")
