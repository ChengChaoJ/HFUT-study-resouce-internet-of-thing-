class Employee:
    def __init__(self, name, employee_id, monthly_salary):
        self.name = name
        self.employee_id = employee_id
        self.monthly_salary = monthly_salary
    
    def pay(self):
        # 计算薪水
        return self.monthly_salary

    def show(self):
        # 显示员工信息
        return f"姓名: {self.name}, 编号: {self.employee_id}, 月薪: {self.pay()}"

class Manager(Employee):
    def __init__(self, name, employee_id, monthly_salary, bonus):
        super().__init__(name, employee_id, monthly_salary)
        self.bonus = bonus  # 经理的额外奖金

    def pay(self):
        # 经理薪水 = 月薪 + 奖金
        return self.monthly_salary + self.bonus

    def show(self):
        return f"经理 - {super().show()}, 奖金: {self.bonus}"

class Salesman(Employee):
    def __init__(self, name, employee_id, monthly_salary, commission):
        super().__init__(name, employee_id, monthly_salary)
        self.commission = commission  # 销售员的佣金

    def pay(self):
        # 销售员薪水 = 月薪 + 佣金
        return self.monthly_salary + self.commission

    def show(self):
        return f"销售员 - {super().show()}, 佣金: {self.commission}"

def main():
    # 创建经理对象
    manager = Manager("张经理", "001", 10000, 3000)
    print(manager.show())  # 显示经理信息
    print(f"经理薪水: {manager.pay()}")  # 显示经理薪水

    print()  # 换行

    # 创建销售员对象
    salesman = Salesman("李销售", "002", 8000, 1500)
    print(salesman.show())  # 显示销售员信息
    print(f"销售员薪水: {salesman.pay()}")  # 显示销售员薪水

if __name__ == "__main__":
    main()
