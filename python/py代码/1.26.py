def find_duplicates(numbers):
    count_dict = {}  # 用于存储数字及其出现的次数

    # 统计每个数字的出现次数
    for number in numbers:
        if number in count_dict:
            count_dict[number] += 1
        else:
            count_dict[number] = 1

    # 筛选出重复的数字
    duplicates = {num: count for num, count in count_dict.items() if count > 1}
    
    return duplicates

# 示例调用
input_numbers = [1, 2, 3, 4, 2, 3, 5, 1, 6, 1, 3]
result = find_duplicates(input_numbers)

# 输出结果
if result:
    print("重复的数字及其出现次数：")
    for num, count in result.items():
        print(f"数字 {num}: {count} 次")
else:
    print("没有重复的数字。")
