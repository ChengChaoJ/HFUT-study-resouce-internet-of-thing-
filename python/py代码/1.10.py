def contains_apple(s):
    # 将字符串转换为小写，并检查是否包含 "apple"
    return "apple" in s.lower()

# 示例调用
input_string = input("请输入一个字符串：")
if contains_apple(input_string):
    print("yes")
else:
    print("no")
