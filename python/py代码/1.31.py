class Vehicle:
    def __init__(self, max_speed, weight):
        self.__max_speed = max_speed  # 最大速度（私有属性）
        self.__weight = weight          # 重量（私有属性）

    @property
    def max_speed(self):
        return self.__max_speed

    @property
    def weight(self):
        return self.__weight


class Bicycle(Vehicle):
    def __init__(self, max_speed, weight, height):
        super().__init__(max_speed, weight)  # 调用父类构造方法
        self.__height = height  # 高度（私有属性）

    @property
    def height(self):
        return self.__height

    @height.setter
    def height(self, height):
        self.__height = height

    def set_max_speed(self, max_speed):
        self._Vehicle__max_speed = max_speed  # 直接访问私有属性


def main():
    # 创建自行车对象
    my_bicycle = Bicycle(max_speed=20, weight=15.5, height=1.1)

    # 设置新的最大速度
    my_bicycle.set_max_speed(25)

    # 输出属性值
    print(f"最大速度: {my_bicycle.max_speed} km/h")
    print(f"重量: {my_bicycle.weight} kg")
    print(f"高度: {my_bicycle.height} m")

    # 修改高度
    my_bicycle.height = 1.2
    print(f"修改后的高度: {my_bicycle.height} m")


if __name__ == "__main__":
    main()
