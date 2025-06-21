import numpy as np
import matplotlib.pyplot as plt

# 定义 x 的范围
x = np.linspace(0, 2 * np.pi, 1000)  # 生成 1000 个点

# 计算 y = sin(x) 和 y = cos(x)
y_sin = np.sin(x)
y_cos = np.cos(x)

# 创建图形
plt.figure(figsize=(10, 6))

# 绘制 y = sin(x)
plt.plot(x, y_sin, label='y = sin(x)', color='blue')

# 绘制 y = cos(x)
plt.plot(x, y_cos, label='y = cos(x)', color='orange')

# 添加图例
plt.legend()

# 添加标题和轴标签
plt.title('Plot of y = sin(x) and y = cos(x)')
plt.xlabel('x (radians)')
plt.ylabel('y')

# 设置 x 轴范围
plt.xticks(np.arange(0, 2 * np.pi + 0.1, np.pi / 2), 
           [r'0', r'$\frac{\pi}{2}$', r'$\pi$', r'$\frac{3\pi}{2}$', r'$2\pi$'])

# 添加网格
plt.grid()

# 显示图形
plt.show()
