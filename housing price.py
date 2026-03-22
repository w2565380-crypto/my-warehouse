import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from mpl_toolkits.mplot3d import Axes3D


# ================= 新增配置开始 =================
# 设置字体为 SimHei (黑体)，解决中文显示问题
plt.rcParams['font.sans-serif'] = ['SimHei'] 
# 用来正常显示负号 (否则负号会显示成方块)
plt.rcParams['axes.unicode_minus'] = False 
# ================= 新增配置结束 =================

#房屋面积和房龄
X=np.array([
    [50,4],
    [60,3],
    [80,6],
    [100,2],
    [120,1]
    ])

#房价（单位每平米万元）
y=np.array([150,200,240,380,460])

#创建并训练模型
model=LinearRegression()
model.fit(X,y)

#查看学习到的参数
print(f"截距(b):{model.intercept_:2f}")
print(f"系数 (w): {model.coef_}")
print(f"解释: 房价 = {model.intercept_:.2f} + {model.coef_[0]:.2f}*面积 + {model.coef_[1]:.2f}*房龄")

#预测
pred=model.predict([[90,4]])
print("90平米,4年房龄的房价预测：",pred)

# 5. 可视化 (因为有两个特征，我们需要画3D图才能看清)
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')

# 画散点 (真实数据)
ax.scatter(X[:, 0], X[:, 1], y, color='red', label='真实数据')

# 画预测平面
# 生成网格数据用于画平面
x1_range = np.linspace(40, 130, 10) # 面积范围
x2_range = np.linspace(0, 12, 10)   # 房龄范围
x1_grid, x2_grid = np.meshgrid(x1_range, x2_range)
X_grid = np.c_[x1_grid.ravel(), x2_grid.ravel()] # 合并成模型需要的格式
y_grid = model.predict(X_grid).reshape(x1_grid.shape)

# 画平面
ax.plot_surface(x1_grid, x2_grid, y_grid, alpha=0.5, color='blue', label='预测平面')

ax.set_xlabel('面积 (平米)')
ax.set_ylabel('房龄 (年)')
ax.set_zlabel('房价 (万元)')
plt.title('多变量线性回归：面积 + 房龄 -> 房价')
plt.show()