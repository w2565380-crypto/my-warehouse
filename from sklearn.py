from sklearn.model_selection import train_test_split
from sklearn import datasets
import numpy as np

iris = datasets.load_iris()
X = iris.data
y = iris.target

print("--- 实验 1：不设置 random_state (每次结果不同) ---")
# 运行两次，你会发现切分出来的数据索引不一样
X_train1, X_test1, _, _ = train_test_split(X, y, test_size=0.2)
print("第一次运行的测试集第一行:", X_test1[0])

X_train2, X_test2, _, _ = train_test_split(X, y, test_size=0.2)
print("第二次运行的测试集第一行:", X_test2[0]) 
# 注意：这两行大概率是不一样的！

print("\n--- 实验 2：设置 random_state=42 (每次结果相同) ---")
# 无论运行多少次，结果都锁定
X_train_a, X_test_a, _, _ = train_test_split(X, y, test_size=0.2, random_state=42)
print("第一次运行的测试集第一行:", X_test_a[0])

X_train_b, X_test_b, _, _ = train_test_split(X, y, test_size=0.2, random_state=42)
print("第二次运行的测试集第一行:", X_test_b[0]) 
# 注意：这两行绝对是一模一样的！