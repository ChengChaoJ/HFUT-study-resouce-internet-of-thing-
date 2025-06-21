import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import CubicSpline
from scipy.linalg import solve_banded

# 三对角方程求解函数
def solve_tridiagonal(a, b, c, d):
    n = len(d)
    ab = np.zeros((3, n-1))
    ab[0, :] = a[1:-1]  # Sub-diagonal
    ab[1, :] = b[1:-1]  # Main diagonal
    ab[2, :] = c[1:-1]  # Super-diagonal
    rhs = d[1:-1]
    return solve_banded((1, 1), ab, rhs)

# 三次样条插值函数（使用自由边界条件）
def cubic_spline_interpolation(x, y):
    cs = CubicSpline(x, y, bc_type='natural')  # 'natural' 对应自由边界条件
    return cs

# 第一个测试用例
t_data = [0, 0.2, 0.6, 1, 2, 5, 10]
C_data = [5.19, 3.77, 2.3, 1.57, 0.8, 0.25, 0.094]
cs_C = cubic_spline_interpolation(t_data, C_data)

# 计算特定时间点的C值
t_query = [0.1, 0.4, 1.2, 5.8]
C_query = cs_C(t_query)
print(f"t={t_query}min时的C值分别为：{C_query}")

# 绘制C随t变化的曲线
t_interp = np.linspace(0, 10, 100)
C_interp = cs_C(t_interp)
plt.plot(t_interp, C_interp, label='C随t变化曲线')
plt.scatter(t_data, C_data, color='red', label='实验数据点')
plt.xlabel('时间 t (min)')
plt.ylabel('反应物浓度 C')
plt.legend()
plt.show()

# 第二个测试用例（这里只展示插值曲线的绘制，因为数据较多，插值部分类似）
# 第二个测试用例
# 合并两部分数据
x_data = [
    0.0, 0.6, 1.5, 1.7, 1.9, 2.1, 2.3, 2.6, 2.8, 3.0,
    3.6, 4.7, 5.2, 5.7, 5.8, 6.0, 6.4, 6.9, 7.6, 8.0
]
y_data = [
    -0.8, -0.34, 0.59, 0.59, 0.23, 0.1, 0.28, 1.03, 1.5, 1.44,
    0.74, -0.82, -1.27, -0.92, -0.92, -1.04, -0.79, -0.06, 1.0, 0.0
]
 
# 使用三次样条插值函数进行插值
cs_xy = cubic_spline_interpolation(x_data, y_data)
 
# 为了绘制插值曲线，我们生成一个密集的x值集合
x_interp = np.linspace(min(x_data), max(x_data), 1000)
y_interp = cs_xy(x_interp)
 
# 绘制插值曲线和原始数据点
plt.plot(x_interp, y_interp, label='插值曲线')
plt.scatter(x_data, y_data, color='red', label='原始数据点')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.title('自由边界三次样条插值曲线')
plt.grid(True)
plt.show()
