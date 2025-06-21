def process_number(num):
    # 确保输入是一个字符串，以便处理
    num_str = str(num)
    
    # 获取位数
    digit_count = len(num_str)

    # 逆序输出各位数字
    reversed_digits = num_str[::-1]
    
    return digit_count, reversed_digits

# 用户输入
input_number = int(input("请输入一个不多于5位的正整数："))

if 1 <= input_number <= 99999:  # 确保输入在有效范围内
    digit_count, reversed_digits = process_number(input_number)
    print(f"数字的位数：{digit_count}")
    print(f"逆序输出各位数字：{reversed_digits}")
else:
    print("输入无效，请确保输入的是一个不多于5位的正整数。")
