def count_character_types(text):
    digit_count = 0  # 计算数字个数
    letter_count = 0  # 计算字母个数
    space_count = 0  # 计算空格个数
    other_count = 0  # 计算其他字符个数

    for char in text:
        if char.isdigit():
            digit_count += 1
        elif char.isalpha():
            letter_count += 1
        elif char.isspace():
            space_count += 1
        else:
            other_count += 1

    return {
        'digits': digit_count,
        'letters': letter_count,
        'spaces': space_count,
        'others': other_count
    }

# 示例调用
input_string = input("请输入一个字符串：")
result = count_character_types(input_string)
print("统计结果：", result)
