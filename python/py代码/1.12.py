def calculate_effort_level(target_effect, work_days=260, rest_days=105, rest_penalty=0.01):
    # 计算1%努力的目标水平
    target_level = target_effect

    # 二分法查找努力程度
    low, high = 0, 1  # 假设努力程度在0到1之间

    while low < high:
        mid = (low + high) / 2
        work_level = mid ** work_days  # 工作日的努力效果
        rest_level = (1 - rest_penalty) ** rest_days  # 休息日的效果
        
        # 总效果
        total_effect = work_level * rest_level
        
        if total_effect < target_level:
            low = mid + 1e-6  # 调整低值
        else:
            high = mid

    return round(low, 4)

# 计算努力程度
target_effect = 1.01 ** 365  # 目标水平
effort_level = calculate_effort_level(target_effect)
print(f"工作日需要努力到的水平为：{effort_level}")
