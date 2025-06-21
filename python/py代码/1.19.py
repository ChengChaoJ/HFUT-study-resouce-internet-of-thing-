import random

def generate_integer_list(length):
    # 生成一个包含随机整数的列表
    return [random.randint(1, 100) for _ in range(length)]

def process_list(lst):
    # 输出原列表
    print("原列表：", lst)
    
    # 输出逆序列表
    reversed_list = lst[::-1]
    print("逆序列表：", reversed_list)
    
    # 输出偶数位置的元素列表
    even_position_elements = lst[::2]
    print("偶数位置的元素列表：", even_position_elements)

# 示例调用
length = int(input("请输入列表的长度："))
integer_list = generate_integer_list(length)
process_list(integer_list)
