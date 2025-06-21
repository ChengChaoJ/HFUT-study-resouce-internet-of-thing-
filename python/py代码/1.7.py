def fibonacci_less_than(n):
    # 定义斐波那契数列的初始化值
    fibonacci_sequence = []
    a, b = 0, 1

    # 生成斐波那契数列并添加到列表中，直到大于或等于 n
    while a < n:
        fibonacci_sequence.append(a)
        a, b = b, a + b  # 更新斐波那契数列的下一个值

    return fibonacci_sequence

# 示例调用
n = 100
result = fibonacci_less_than(n)
print(f"小于 {n} 的斐波那契数列值有：{result}")
