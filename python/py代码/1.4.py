def is_perfect_number(n):
    # 计算n的因子之和
    if n < 2:
        return False

    factor_sum = 1  # 从1开始，因为1是任何数的因子
    
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            factor_sum += i
            if i != n // i:  # 处理平方数的情况
                factor_sum += n // i
    
    return factor_sum == n

def find_perfect_numbers(limit):
    perfect_numbers = []
    
    for i in range(2, limit):
        if is_perfect_number(i):
            perfect_numbers.append(i)
    
    return perfect_numbers

# 查找1000以内的完数
perfect_numbers = find_perfect_numbers(1000)
print("1000以内的完数有：", perfect_numbers)
