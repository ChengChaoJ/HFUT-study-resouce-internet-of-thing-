def last_person_standing(n):
    # 创建一个列表，表示所有人的编号，从1到n
    people = list(range(1, n + 1))
    index = 0  # 从第一个人开始

    while len(people) > 1:
        # 每数到3就删除该位置的人
        index = (index + 2) % len(people)  # 这将对报数进行计算
        print(f"退出的人是: {people[index]}")
        people.pop(index)  # 移除报到3的人

    return people[0]

def main():
    # 用户输入
    n = int(input("请输入人数 n："))
    
    if n <= 0:
        print("人数必须为正整数。")
        return

    last_person = last_person_standing(n)
    print(f"最后剩下的是原来第 {last_person} 号的人。")

if __name__ == "__main__":
    main()
