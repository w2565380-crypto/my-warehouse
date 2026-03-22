from sklearn.linear_model import LinearRegression
import pandas as pd
import numpy as np
from sklearn.preprocessing import StandardScaler

# 1. 准备数据
data = {
    'rooms': [2, 3, 4, 5, 2, 6],
    'area': [80, 120, 150, 200, 60, 300],
    'price': [200, 350, 450, 700, 150, 1200] # 这是目标值 y
}
df = pd.DataFrame(data)

# 2. 特征工程 (构造新特征)
df['avg_room_size'] = df['area'] / df['rooms']
df['luxury_score'] = df['area'] * df['rooms']

# 3. 数据清洗 (归一化)
feature_cols = ['rooms', 'area', 'avg_room_size', 'luxury_score']
scaler = StandardScaler()
df[feature_cols] = scaler.fit_transform(df[feature_cols])

# --- 关键区别在这里 ---

# A. 这是你刚才看到的：处理后的【输入数据】 (X)
print("=== 处理后的输入数据 (特征值 x) ===")
print(df[feature_cols].head())
# 输出就是你看到的那些 -1.11, 0.07 之类的数字

# B. 现在我们要训练模型，让它去学习参数！
X = df[feature_cols]
y = df['price']

model = LinearRegression()
model.fit(X, y)  # <--- 这一步才是“学习”，模型在找最佳拟合线

# C. 这才是你要找的：【拟合参数】 (权重 w 和 截距 b)
print("\n=== 模型学到的拟合参数 (权重 w) ===")
for feature, coef in zip(feature_cols, model.coef_):
    print(f"特征 '{feature}' 的权重: {coef:.4f}")

print(f"\n截距 (bias b): {model.intercept_:.4f}")

# D. 最终的回归方程长这样：
# 价格 = (w1 * rooms) + (w2 * area) + (w3 * avg_room_size) + (w4 * luxury_score) + b