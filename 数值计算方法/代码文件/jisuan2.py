import numpy as np

# 复化梯形公式求积算法
def trapezoidal_rule(f, a, b, n):
    h = (b - a) / n
    result = (f(a) + f(b)) / 2.0
    for i in range(1, n):
        result += f(a + i * h)
    result *= h
    return result

# 复化辛浦生求积算法
def simpson_rule(f, a, b, n):
    if n % 2 == 1:  # 确保 n 是偶数
        n += 1
    h = (b - a) / n
    result = f(a) + f(b)
    for i in range(1, n, 2):
        result += 4 * f(a + i * h)
    for i in range(2, n - 1, 2):
        result += 2 * f(a + i * h)
    result *= h / 3
    return result

# 龙贝格算法修改后的输出
def romberg_integration(f, a, b, tol):
    max_iter = 20
    R = np.zeros((max_iter, max_iter))

    print("k\tT\t\tS\t\tC\t\tR")  # 输出表头
   
    for k in range(max_iter):
        R[k][0] = trapezoidal_rule(f, a, b, 2 ** k)
        
        for j in range(1, k + 1):
            R[k][j] = (4 ** j * R[k][j - 1] - R[k - 1][j - 1]) / (4 ** j - 1)

        # 输出当前行结果
        print(f"{k}\t{R[k][0]:.6f}", end=' ')
        for j in range(1, k + 1):
            print(f"{R[k][j]:.6f}", end=' ')
        print()
        
        # 检查误差
        if k > 0 and abs(R[k][k] - R[k - 1][k - 1]) < tol:
            break
    
    return R[k][k]


# 需要积分的函数 f(x)
def f(x):
    return np.sin(x)  # 示例函数

# 积分区间和容忍误差
a = 0
b = np.pi / 2
exact_solution = 0.9460831  # 精确解
tol = 1e-6

# 测试复化梯形公式
print("复化梯形公式:")
for n in [2, 4, 8, 16]:
    integral_trap = trapezoidal_rule(f, a, b, n)
    print(f"n={n}, 结果={integral_trap:.6f}, 误差={abs(integral_trap - exact_solution):.6f}")

# 测试复化辛浦生公式
print("\n复化辛浦生公式:")
for n in [2, 4, 8, 16]:
    integral_simp = simpson_rule(f, a, b, n)
    print(f"n={n}, 结果={integral_simp:.6f}, 误差={abs(integral_simp - exact_solution):.6f}")

# 运行龙贝格算法
print("\n龙贝格算法:")
final_result = romberg_integration(f, a, b, tol)
print(f"the result is: {final_result:.6f}")
