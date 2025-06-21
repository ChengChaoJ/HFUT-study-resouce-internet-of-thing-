import itertools

# 所有可用的数字
digits = [1, 2, 3, 4]

# 生成所有互不相同且无重复数字的三位数
three_digit_numbers = list(itertools.permutations(digits, 3))

# 显示所有三位数
for number in three_digit_numbers:
    print(''.join(map(str, number)))

# 统计总数
total_count = len(three_digit_numbers)
print(f"总共有 {total_count} 个互不相同的三位数。")
