from sklearn import datasets
import numpy as np

iris = datasets.load_iris()
X = iris.data
y = iris.target

print("--- 特征 X 的形状 ---")
print(f"形状: {X.shape}")
print("前 3 朵花的数据 (每行 4 个特征):")
print(X[:3]) 
# 输出示例: [[5.1 3.5 1.4 0.2], [4.9 3.0 1.4 0.2], [4.7 3.2 1.3 0.2]]

print("\n--- 标签 y 的形状 ---")
print(f"形状: {y.shape}")
print("前 3 朵花的标签 (对应的答案):")
print(y[:3]) 
# 输出示例: [0 0 0]

print("\n--- 验证对应关系 ---")
print(f"第 1 朵花的特征是: {X[0]}")
print(f"第 1 朵花的标签是: {y[0]}")
print("它们是一一对应的！")