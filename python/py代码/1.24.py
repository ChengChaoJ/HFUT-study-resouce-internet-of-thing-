def process_string_to_dict(s):
    # 首先通过'*'分割字符串
    pairs = s.split('*')
    
    # 使用字典推导式构建字典，': '进行分割
    result_dict = {key_value.split(':')[0]: key_value.split(':')[1] for key_value in pairs}
    
    return result_dict

# 示例字符串
input_string = "a:1*a1:2*a2:3*a3:4"
result_dict = process_string_to_dict(input_string)

# 输出结果
print("生成的字典为：", result_dict)
