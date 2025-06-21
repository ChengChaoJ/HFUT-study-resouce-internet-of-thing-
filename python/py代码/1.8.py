def sieve_of_eratosthenes(n):
    # 初始化一个布尔数组，初始假设所有数都是素数
    is_prime = [True] * n
    p = 2  # 从2开始，因为1不是素数

    # 0和1不是素数
    is_prime[0] = is_prime[1] = False

    while p * p < n:
        # 如果是素数，则标记其倍数为非素数
        if is_prime[p]:
            for i in range(p * p, n, p):
                is_prime[i] = False
        p += 1

    # 收集所有素数
    prime_numbers = [i for i in range(n) if is_prime[i]]
    return prime_numbers

# 获取用户输入
try:
    num = int(input("请输入一个大于2的自然数："))
    if num > 2:
        primes = sieve_of_eratosthenes(num)
        print(f"小于 {num} 的所有素数有：{primes}")
    else:
        print("请输入一个大于2的自然数。")
except ValueError:
    print("请输入一个有效的自然数。")
