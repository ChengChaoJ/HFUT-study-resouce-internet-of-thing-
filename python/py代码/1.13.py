def max_overlap_length(s1, s2):
    # 计算s1的后缀和s2的前缀的最大重叠部分
    return max(len(s1) - s1.rfind(s2[:i]) for i in range(1, len(s2) + 1) if s1.endswith(s2[:i]))

def combine_strings(s1, s2):
    # 使用lambda找出最大重叠长度
    overlap = max_overlap_length(s1, s2)
    # 将两个字符串连接，移除重叠部分
    return s1 + s2[overlap:]

# 示例调用
str1 = input("请输入第一个字符串：")
str2 = input("请输入第二个字符串：")
result = combine_strings(str1, str2)
print("连接后的字符串为：", result)
