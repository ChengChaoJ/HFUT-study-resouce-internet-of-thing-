import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import CubicSpline

# 1. 三对角方程的求解函数
def solve_tridiagonal(a, b, c, d):
    n = len(d)
    ac = np.zeros(n-1)
    bc = np.zeros(n)

    # 前向消去
    bc[0] = b[0]
    for i in range(1, n):
        ac[i-1] = a[i-1] / bc[i-1]
        bc[i] = b[i] - ac[i-1] * c[i-1]

    # 后向替代
    x = np.zeros(n)
    x[0] = d[0] / bc[0]
    for i in range(1, n):
        x[i] = (d[i] - a[i-1] * x[i-1]) / bc[i]

    return x

# 2. 主程序进行样条插值
def main():
    # 实验数据
    t = np.array([0, 0.2, 0.6, 1.0, 2.0, 5.0, 10.0])  # 时间点
    C = np.array([5.19, 3.77, 2.3, 1.57, 0.8, 0.25, 0.094])  # 浓度值

    # 计算一阶导数（简单估计）
    dC_dt = np.gradient(C, t)  # 计算离散导数
    dC_dt[0] = 0  # 假设起始点导数为0
    dC_dt[-1] = 0  # 假设结束点导数为0

    # 设置三角矩阵参数
    n = len(t)
    h = np.diff(t)
    alpha = np.zeros(n-1)
    beta = np.zeros(n)
    gamma = np.zeros(n-1)

    for i in range(1, n-1):
        alpha[i-1] = h[i-1]
        beta[i] = 2 * (h[i-1] + h[i])
        gamma[i-1] = h[i]

    # 右侧常数项
    rhs = np.zeros(n)
    rhs[1:n-1] = 3 * (dC_dt[2:n] - dC_dt[1:n-1])

    # 计算二阶导数
    d2C_dt2 = solve_tridiagonal(alpha[1:], beta[1:-1], gamma[:-1], rhs[1:-1])

    # 创建样条函数
    cs = CubicSpline(t, C, bc_type=((1, dC_dt[0]), (1, dC_dt[-1])))

    # 查询特定时间点的C值
    query_times = np.array([0.1, 0.4, 1.2, 5.8])
    C_values = cs(query_times)

    # 输出查询结果
    print("t = 0.1, 0.4, 1.2, 5.8 min 时的 C 值分别为：")
    print(", ".join(f"{value:.8f}" for value in C_values))

    # 生成插值点
    t_interp = np.linspace(t[0], t[-1], 100)  # 从0到10的时间点
    C_interp = cs(t_interp)

    # 绘制结果
    plt.figure(figsize=(10, 6))
    plt.scatter(t, C, color='red', label='实验数据', zorder=5)
    plt.plot(t_interp, C_interp, label='三次样条插值', color='blue')
    plt.title('反应物浓度C随时间t变化的插值')
    plt.xlabel('时间 t (min)')
    plt.ylabel('浓度 C (mol/L)')
    plt.grid()
    plt.legend()
    plt.show()

# 执行主程序
if __name__ == "__main__":
    main()
