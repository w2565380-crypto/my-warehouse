import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

# 设置中文字体，防止乱码
plt.rcParams['font.sans-serif'] = ['SimHei'] 
plt.rcParams['axes.unicode_minus'] = False

# 构造数据：[年龄, 年收入(万元)]
# 故意构造三团明显的数据
X = np.array([
    # 少年组 (年龄小，无收入或很低)
    [12, 0.5], 
    [13, 0.8], 
    [14, 0.6], 
    [15, 1.0],
    
    # 青年组 (年龄20-35，收入较高，波动大)
    [22, 8.0], 
    [25, 12.5], 
    [28, 15.0], 
    [30, 11.0],
    [35, 18.0],
    
    # 老年组 (年龄60+，收入稳定或退休金)
    [64, 9.0], 
    [68, 8.5], 
    [72, 10.0], 
    [88, 7.0]
])

# 创建模型：分成 3 类 (少年、青年、老年)
kmeans = KMeans(n_clusters=3, random_state=42)
kmeans.fit(X)

labels = kmeans.labels_
centers = kmeans.cluster_centers_

# 打印结果看看机器怎么分的
print("原始数据及分类标签:")
for i, age_income in enumerate(X):
    print(f"年龄:{age_income[0]:.0f}岁, 收入:{age_income[1]:.1f}万 -> 类别:{labels[i]}")

print("\n聚类中心点 (机器找到的典型代表):")
print(centers)

# 画图
plt.figure(figsize=(8, 6))

# 画散点：c=labels 会根据类别自动上色
# cmap='viridis' 是配色方案，s=100 是点的大小，edgecolors='black' 是边框
plt.scatter(X[:, 0], X[:, 1], c=labels, cmap='viridis', s=100, edgecolors='black', alpha=0.7)

# 画出聚类中心 (用红色的大叉号表示)
plt.scatter(centers[:, 0], centers[:, 1], c='red', s=200, marker='X', label='聚类中心')

# 添加标签和标题
plt.xlabel('年龄 (岁)')
plt.ylabel('年收入 (万元)')
plt.title('K-Means 人群分类：少年、青年、老年')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.5)

plt.show()