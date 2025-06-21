def remove_duplicates(lst):
    seen = set()  # 用于记录已经遇到的元素
    unique_list = []  # 存储去重后的元素

    for item in lst:
        if item not in seen:
            seen.add(item)  # 记录该元素
            unique_list.append(item)  # 保留该元素

    return unique_list

# 示例调用，允许用户输入元素
input_str = input("请输入列表元素（用逗号或空格分隔）：")
# 将输入的字符串分割成列表并转换为整数
input_list = [int(x.strip()) for x in input_str.replace(',', ' ').split()]

result = remove_duplicates(input_list)
print("去重后的列表为：", result)
