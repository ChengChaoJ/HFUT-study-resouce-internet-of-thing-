class MyQueue:
    def __init__(self, size):
        self.size = size         # 队列的最大长度
        self.data = []          # 存储队列元素
        self.current = 0        # 当前元素个数

    def is_empty(self):
        """判断队列是否为空"""
        return self.current == 0

    def is_full(self):
        """判断队列是否满"""
        return self.current == self.size

    def front(self):
        """取出队头元素"""
        if self.is_empty():
            return "队列为空，无法取出队头元素"
        return self.data[0]

    def enqueue(self, element):
        """入队，将元素放到队列的尾部"""
        if self.is_full():
            return "队列已满，无法入队"
        self.data.append(element)
        self.current += 1

    def dequeue(self):
        """出队，删除当前队头元素"""
        if self.is_empty():
            return "队列为空，无法出队"
        element = self.data.pop(0)  # 删除队头元素
        self.current -= 1
        return element

def main():
    N = int(input("请输入队列的最大长度 N："))
    queue = MyQueue(N)

    # 测试队列的功能
    print("初始化队列。")
    
    # 检查是否为空
    print("队列为空吗？", queue.is_empty())

    # 入队一些元素
    for i in range(1, N + 1):
        print(f"入队元素: {i}")
        print(queue.enqueue(i))

    # 再次检查是否满
    print("队列满吗？", queue.is_full())

    # 取出队头元素
    print("队头元素为:", queue.front())

    # 出队一些元素
    for _ in range(2):
        print("出队元素:", queue.dequeue())  # 出队两个元素

    # 重新检查队头元素
    print("新的队头元素为:", queue.front())

if __name__ == "__main__":
    main()
