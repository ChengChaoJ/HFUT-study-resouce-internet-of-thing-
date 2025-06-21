
import random

def guess_the_number():
    # 设置随机数生成的范围
    lower_bound = 1
    upper_bound = 100
    # 生成一个随机正整数
    secret_number = random.randint(lower_bound, upper_bound)
    
    # 设置最大猜测次数
    max_attempts = 7
    attempts = 0

    print(f"欢迎来到猜数字游戏！")
    print(f"我已经选择了一个在 {lower_bound} 到 {upper_bound} 之间的随机正整数。")
    print(f"你有 {max_attempts} 次猜测机会。")

    while attempts < max_attempts:
        # 获取用户输入
        guess = input(f"请输入你的猜测（第 {attempts + 1} 次猜测）：")
        
        # 检查用户输入是否为数字
        if not guess.isdigit():
            print("请输入一个有效的正整数！")
            continue
        
        guess = int(guess)
        attempts += 1
        
        # 判断用户猜测的结果
        if guess == secret_number:
            print("恭喜你！猜对了！")
            break
        elif guess < secret_number:
            print("太小了，请再试一次。")
        else:
            print("太大了，请再试一次。")
    
    # 如果超过最大尝试次数仍未猜对
    if attempts == max_attempts:
        print(f"游戏结束！正确答案是 {secret_number}。")

# 运行游戏
guess_the_number()
