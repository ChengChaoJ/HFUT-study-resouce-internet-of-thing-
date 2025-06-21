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
    # 整合数据
    x = np.array([0.0, 0.6, 1.5, 1.7, 1.9, 2.1, 2.3, 2.6, 2.8, 3.0, 
                  3.6, 4.7, 5.2, 5.7, 5.8, 6.0, 6.4, 6.9, 7.6, 8.0])
    y = np.array([-0.8, -0.34, 0.59, 0.59, 0.23, 0.1, 0.28, 1.03, 1.5, 1.44, 
                  0.74, -0.82, -1.27, -0.92, -1.04, -0.79, -0.06, 1.0, 0.0, 0.0])

    # 创建三次样条对象
    cs = CubicSpline(x, y, bc_type='natural')  # 自然边界条件

    # 生成插值点
    x_interp = np.linspace(min(x), max(x), 100)  # 全部数据的插值点
    y_interp = cs(x_interp)

    # 绘制结果
    plt.figure(figsize=(10, 6))
    
    plt.scatter(x, y, color='red', label='已知数据点', zorder=5)
    plt.plot(x_interp, y_interp, label='三次样条插值', color='blue')
    plt.title('整合数据的样条插值')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid()
    plt.legend()
    plt.show()

# 执行主程序
if __name__ == "__main__":
    main()
