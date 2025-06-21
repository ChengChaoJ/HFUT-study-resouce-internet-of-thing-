import re

def is_valid_password(password):
    if not (6 <= len(password) <= 12):  # 检查长度
        return False
    if not re.search(r'[a-z]', password):  # 检查至少一个小写字母
        return False
    if not re.search(r'[A-Z]', password):  # 检查至少一个大写字母
        return False
    if not re.search(r'[0-9]', password):  # 检查至少一个数字
        return False
    if not re.search(r'[$#@]', password):  # 检查至少一个特殊字符
        return False
    return True  # 如果通过所有检查，则返回 True

def main():
    # 用户输入
    input_passwords = input("请输入多个逗号分隔的密码：")
    passwords = input_passwords.split(',')

    # 检查每个密码的有效性并收集符合条件的密码
    valid_passwords = [password.strip() for password in passwords if is_valid_password(password.strip())]

    # 输出符合条件的密码
    print("符合条件的密码有：", ', '.join(valid_passwords))

if __name__ == "__main__":
    main()
