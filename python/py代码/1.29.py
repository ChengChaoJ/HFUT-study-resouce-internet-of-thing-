import operator

class Student:
    def __init__(self, student_id, name, gender, class_name, grades):
        self.student_id = student_id
        self.name = name
        self.gender = gender
        self.class_name = class_name
        self.grades = grades  # 字典，存储课程成绩

    def average_grade(self):
        return sum(self.grades.values()) / len(self.grades) if self.grades else 0

    def update_grade(self, course, score):
        self.grades[course] = score

    def delete_grade(self, course):
        if course in self.grades:
            del self.grades[course]

    def __str__(self):
        avg = self.average_grade()
        grades_str = ', '.join(f'{course}: {score}' for course, score in self.grades.items())
        return f'{self.student_id} {self.name} {self.gender} {self.class_name} {grades_str} 平均成绩: {avg:.2f}'

def read_students_from_file(file_path):
    students = []
    with open(file_path, 'r', encoding='utf-8') as file:
        for line in file:
            parts = line.strip().split()
            student_id = parts[0]
            name = parts[1]
            class_name = parts[2]
            gender = parts[3]
            grades = {}
            for grade_part in parts[4:]:
                course, score = grade_part.split(':')
                grades[course] = int(score)
            student = Student(student_id, name, gender, class_name, grades)
            students.append(student)
    return students

def find_student(students, identifier):
    for student in students:
        if student.student_id == identifier or student.name == identifier:
            return student
    return None

def display_students(students):
    for student in students:
        print(student)

def display_ranking(students):
    students_sorted = sorted(students, key=lambda x: x.average_grade())
    print("\n按平均成绩排名：")
    for rank, student in enumerate(students_sorted, start=1):
        print(f'排名: {rank}, {student}')

def add_student(students):
    student_id = input("请输入学号：")
    name = input("请输入姓名：")
    gender = input("请输入性别：")
    class_name = input("请输入班级：")
    
    grades = {}
    while True:
        course = input("请输入课程名称（输入'结束'停止输入）：")
        if course == '结束':
            break
        score = int(input(f"请输入 {course} 的成绩："))
        grades[course] = score
    
    student = Student(student_id, name, gender, class_name, grades)
    students.append(student)
    print("学生信息添加成功！")

def main():
    file_path = 'students.txt'  # 请将此路径修改为你的文件路径
    students = read_students_from_file(file_path)

    while True:
        print("\n菜单选项:")
        print("1. 显示所有学生信息")
        print("2. 按平均成绩排名")
        print("3. 查找并修改学生成绩")
        print("4. 添加学生信息")
        print("5. 删除学生成绩")
        print("0. 退出")

        choice = input("请选择一个操作（0-5）：")
        
        if choice == '1':
            display_students(students)
        elif choice == '2':
            display_ranking(students)
        elif choice == '3':
            identifier = input("请输入要查找的学号或姓名：")
            student = find_student(students, identifier)
            if student:
                print("找到学生信息：", student)
                course = input("请输入修改的课程名称：")
                new_score = int(input("请输入新的成绩："))
                student.update_grade(course, new_score)
                print("修改后的学生信息：", student)
            else:
                print("未找到该学生。")
        elif choice == '4':
            add_student(students)  # 调用添加学生信息的函数
        elif choice == '5':
            identifier = input("请输入要删除成绩的学生学号或姓名：")
            student = find_student(students, identifier)
            if student:
                course = input("请输入要删除的课程名称：")
                student.delete_grade(course)
                print("删除成绩后的学生信息：", student)
            else:
                print("未找到该学生。")
        elif choice == '0':
            print("退出程序。")
            break
        else:
            print("无效选择，请重新输入。")

if __name__ == '__main__':
    main()
