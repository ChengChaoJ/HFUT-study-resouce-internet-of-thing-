def read_student_data(file_path):
    """读取学生数据，并返回学号、数学成绩和英语成绩的列表"""
    students = []
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            for line in file:
                # 以空格分割行数据
                parts = line.strip().split()
                if len(parts) == 3:
                    student_id = parts[0]
                    math_score = float(parts[1])
                    english_score = float(parts[2])
                    students.append((student_id, math_score, english_score))
    except FileNotFoundError:
        print(f"文件 {file_path} 不存在，请确认文件路径。")
    except Exception as e:
        print(f"发生错误: {e}")
    return students

def main():
    file_path = 'student1.txt'  # 文件路径

    # 读取学生数据
    students = read_student_data(file_path)

    if not students:
        print("没有读取到学生数据。")
        return

    # (1) 计算数学成绩和英语成绩的最高分
    math_scores = [score[1] for score in students]
    english_scores = [score[2] for score in students]

    max_math = max(math_scores)
    max_english = max(english_scores)

    print(f"数学成绩最高分: {max_math:.1f}")
    print(f"英语成绩最高分: {max_english:.1f}")

    # (2) 找到平均成绩≥90的学生
    print("\n平均成绩≥90的学生：")
    for student in students:
        student_id, math_score, english_score = student
        average_score = (math_score + english_score) / 2
        if average_score >= 90:
            print(f"学号: {student_id}, 数学成绩: {math_score:.1f}, 英语成绩: {english_score:.1f}, 平均成绩: {average_score:.1f}")

if __name__ == "__main__":
    main()
