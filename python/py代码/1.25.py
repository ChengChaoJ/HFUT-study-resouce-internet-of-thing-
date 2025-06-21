def binary_search(arr, target):
    left, right = 0, len(arr) - 1
    
    while left <= right:
        mid = (left + right) // 2  # 计算中间索引

        if arr[mid] == target:  # 找到目标元素
            return mid
        elif arr[mid] < target:  # 目标在右侧
            left = mid + 1
        else:  # 目标在左侧
            right = mid - 1
    
    return -1  # 未找到目标元素

# 示例调用
sorted_array = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]
target_value = int(input("请输入要查找的数字："))
result = binary_search(sorted_array, target_value)

if result != -1:
    print(f"数字 {target_value} 的索引是：{result}")
else:
    print("数字未找到。")
