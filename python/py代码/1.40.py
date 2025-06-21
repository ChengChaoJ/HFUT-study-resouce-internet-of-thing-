def is_divisible_by_5(binary_str):
    """检查二进制字符串转换为十进制后是否能被5整除"""
    decimal_value = int(binary_str, 2)  # 将二进制字符串转换为十进制整数
    return decimal_value % 5 == 0  # 检查是否能被5整除

def main():
    # 用户输入
    input_binaries = input("请输入一系列逗号分隔的4位二进制数：")
    binary_numbers = input_binaries.split(',')

    # 检查每个二进制数，并收集能被5整除的数
    divisible_by_5 = [binary for binary in binary_numbers if is_divisible_by_5(binary.strip())]

    # 输出能被5整除的二进制数
    if divisible_by_5:
        print("可以被5整除的二进制数是：", ', '.join(divisible_by_5))
    else:
        print("没有可以被5整除的二进制数。")

if __name__ == "__main__":
    main()
