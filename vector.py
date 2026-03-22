import numpy as np
import time

# ================= 1. 准备超大规模数据 =================
print("正在生成大规模数据...")
np.random.seed(42)

m = 1_000_000   # 100 万条样本
n = 100         # 100 个特征

# 特征矩阵 X: (100万行, 100列)
X = np.random.rand(m, n)

# 权重向量 w: (100行, 1列)
w = np.random.rand(n, 1)

# 偏置 b
b = 0.5

print(f"数据规模：{m:,} 条样本 × {n} 个特征")
print("-" * 50)

# ================= 2. 方法 A：传统的 For 循环 (非向量化) =================
print("🐢 方法 A：正在运行 Python For 循环 (请勿关闭，可能需要几十秒)...")

start_time = time.time()

# 初始化结果数组
y_loop = np.zeros((m, 1))

# ⚠️ 警告：这里使用了双重循环，效率极低！
# 实际开发中绝对禁止这样写，这里仅为了演示慢速
for i in range(m):
    sum_val = 0
    for j in range(n):
        sum_val += X[i, j] * w[j, 0]
    y_loop[i, 0] = sum_val + b

end_time_loop = time.time()
time_loop = end_time_loop - start_time

print(f"✅ For 循环 完成！耗时：{time_loop:.4f} 秒")

# ================= 3. 方法 B：NumPy 向量化运算 =================
print("\n🚀 方法 B：正在运行 NumPy 向量化运算...")

start_time = time.time()

# ✨ 魔法时刻：一行代码完成所有计算
# 逻辑：矩阵乘法 (X @ w) + 广播机制 (+ b)
y_vec = np.dot(X, w) + b 
# 或者写成: y_vec = X @ w + b

end_time_vec = time.time()
time_vec = end_time_vec - start_time

print(f"✅ 向量化 完成！耗时：{time_vec:.6f} 秒")

# ================= 4. 结果验证与对比 =================
print("\n" + "="*50)
print("📊 性能对比报告")
print("="*50)

# 验证结果是否一致 (允许极小的浮点数误差)
if np.allclose(y_loop, y_vec):
    print("✔️ 结果验证：两种方法计算结果完全一致！")
else:
    print("❌ 结果验证：出错了，结果不一致！")

speed_up = time_loop / time_vec if time_vec > 0 else float('inf')

print(f"\n⏱️  For 循环耗时：{time_loop:.4f} 秒")
print(f"⚡  向量化耗时：{time_vec:.6f} 秒")
print(f"\n🔥  速度提升倍数：{speed_up:,.0f} 倍 !!!")

if speed_up > 1000:
    print("💡 评价：这就是为什么深度学习必须使用 GPU 和向量化库的原因！")
elif speed_up > 100:
    print("💡 评价：向量化让原本需要几分钟的计算在瞬间完成。")
else:
    print("💡 评价：即使在小数据上，向量化也有明显优势。")

print("="*50)