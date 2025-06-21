import numpy as np

def clamped_cubic_spline_interpolation(x, y, y_prime_start, y_prime_end):
    n = len(x)
    h = np.diff(x)
    delta_y = np.diff(y)

    # 构造三对角矩阵的系数
    A = np.zeros((n, n))
    B = np.zeros(n)

    # 填充矩阵A和向量B
    for i in range(1, n-1):
        A[i, i-1] = h[i-1]
        A[i, i] = 2 * (h[i-1] + h[i])
        A[i, i+1] = h[i]
        B[i] = 3 * (delta_y[i]/h[i] - delta_y[i-1]/h[i-1])

    # 夹持边界条件
    A[0, 0] = 2 * h[0]
    A[0, 1] = h[0]
    B[0] = 3 * (delta_y[0]/h[0] - y_prime_start)

    A[-1, -2] = h[-1]
    A[-1, -1] = 2 * h[-1]
    B[-1] = 3 * (y_prime_end - delta_y[-1]/h[-1])

    # 求解二阶导数
    M = np.linalg.solve(A, B)

    # 构造插值函数
    def spline(t):
        for i in range(n-1):
            if x[i] <= t <= x[i+1]:
                a = (x[i+1] - t) / h[i]
                b = (t - x[i]) / h[i]
                c = (a**3 - a) * h[i]**2 / 6
                d = (b**3 - b) * h[i]**2 / 6
                return a * y[i] + b * y[i+1] + c * M[i] + d * M[i+1]
        return None

    return spline

# 示例
x = np.array([0, 1, 2, 3, 4])
y = np.array([0, 1, 0, 1, 0])
y_prime_start = 0  # 起始点的一阶导数
y_prime_end = 0    # 终点的一阶导数

spline = clamped_cubic_spline_interpolation(x, y, y_prime_start, y_prime_end)

# 测试插值
t_values = np.linspace(0, 4, 100)
interpolated_values = [spline(t) for t in t_values]

# 打印结果
import matplotlib.pyplot as plt
plt.plot(x, y, 'o', label='Data points')
plt.plot(t_values, interpolated_values, label='Clamped Cubic Spline')
plt.legend()
plt.show()
