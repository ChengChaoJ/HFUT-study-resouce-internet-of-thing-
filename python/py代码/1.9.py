def is_palindrome(s):
    # 移除字符串中的空格并转换为小写
    s = s.replace(" ", "").lower()
    # 判断字符串是否与其反转相同
    return s == s[::-1]

# 示例调用
test_string = input("请输入一个字符串：")
if is_palindrome(test_string):
    print(f"'{test_string}' 是回文字符串。")
else:
    print(f"'{test_string}' 不是回文字符串。")
