import numpy as np
import matplotlib.pyplot as plt

# ==========================================
# 1. 准备数据
# ==========================================
X = np.array([[1], [2], [3], [4], [5], [6], [7], [8], [9], [10]]) 
y = np.array([0, 0, 0, 0, 0, 1, 1, 1, 1, 1]) 
X_plot = np.linspace(0, 11, 100).reshape(-1, 1)

print("📦 数据已准备好：10个样本，1个特征（学习时长）")

# ==========================================
# 2. 核心函数定义
# ==========================================
def sigmoid(z):
    return 1 / (1 + np.exp(-z))

def compute_loss(X, y, w, b):
    m = len(y)
    # 确保 w 是标量参与运算，如果 w 是数组则自动广播，这里逻辑没问题
    z = np.dot(X, w) + b       
    y_pred = sigmoid(z)
    # 加上 1e-15 防止 log(0)
    loss = -np.mean(y * np.log(y_pred + 1e-15) + (1 - y) * np.log(1 - y_pred + 1e-15))
    return loss

# ==========================================
# 3. 训练过程 (梯度下降)
# ==========================================
# 【修复点 1】初始化为纯浮点数 (float)，而不是 numpy 数组
w = 0.0   
b = 0.0   
learning_rate = 0.1
iterations = 1000

print("\n🏃‍♂️ 开始训练 (梯度下降)...")
loss_history = []

for i in range(iterations):
    # 1. 前向传播
    # np.dot(X, w) 当 w 是标量时，结果是一维数组 (10,)
    # 但为了数学严谨，如果 w 是标量，dot 结果形状取决于 X
    # 这里 X 是 (10, 1), w 是标量 -> 结果 (10, 1) 或 (10,) 取决于版本，通常没问题
    z = np.dot(X, w) + b
    y_pred = sigmoid(z)
    
    # 2. 计算梯度
    # dw 的计算结果可能是一个包含一个元素的数组 [0.005]，我们需要把它取出来变成 0.005
    dw = (1/len(y)) * np.dot(X.T, (y_pred - y))
    db = (1/len(y)) * np.sum(y_pred - y)
    
    # 【修复点 2】如果 dw 是数组，提取其中的数字 .item() 或 [0]
    # 因为我们的特征只有1个，dw 形状是 (1,)，我们需要标量
    if isinstance(dw, np.ndarray):
        dw = dw[0]
    if isinstance(db, np.ndarray):
        db = db.item() # 或者 db[0]

    # 3. 更新参数
    w = w - learning_rate * dw
    b = b - learning_rate * db
    
    # 记录损失
    if i % 100 == 0:
        # 【修复点 3】打印前确保是数字，虽然上面更新了，但保险起见用 float() 包裹
        current_loss = compute_loss(X, y, w, b)
        print(f"迭代 {i}: 损失 = {current_loss:.4f}, w = {float(w):.4f}, b = {float(b):.4f}")

print("\n✅ 训练完成！")
print(f"最终参数: 权重 w = {float(w):.4f}, 偏置 b = {float(b):.4f}")

# ==========================================
# 4. 验证与可视化
# ==========================================
def predict(X_input, w_val, b_val):
    prob = sigmoid(np.dot(X_input, w_val) + b_val)
    return (prob >= 0.5).astype(int), prob

y_pred_custom, probs_custom = predict(X_plot, w, b)

# --- 对比 sklearn ---
try:
    from sklearn.linear_model import LogisticRegression
    model_sk = LogisticRegression()
    model_sk.fit(X, y)
    w_sk = model_sk.coef_[0][0]
    b_sk = model_sk.intercept_[0]
    y_pred_sk, probs_sk = predict(X_plot, w_sk, b_sk)

    print("\n🔍 结果对比:")
    print(f"手写模型: w={float(w):.4f}, b={float(b):.4f}")
    print(f"Sklearn库:  w={w_sk:.4f}, b={b_sk:.4f}")
except ImportError:
    print("\n⚠️ 未安装 scikit-learn，跳过库对比环节。")

# 画图
plt.figure(figsize=(10, 6))
plt.scatter(X, y, color='black', label='真实数据', zorder=5)
plt.plot(X_plot, probs_custom, 'r-', linewidth=2, label='手写模型概率曲线')
plt.axhline(0.5, color='gray', linestyle='--', alpha=0.5, label='决策阈值 (0.5)')
plt.title('逻辑回归：学习时长 vs 考试通过概率', fontsize=14)
plt.xlabel('学习时长 (小时)')
plt.ylabel('通过概率 (0 ~ 1)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.ylim(-0.1, 1.1)
plt.show()

# 画损失图
plt.figure(figsize=(8, 4))
plt.plot(range(0, iterations, 100), loss_history, 'b-o')
plt.title('训练过程中的损失变化')
plt.xlabel('迭代次数')
plt.ylabel('损失值')
plt.grid(True, alpha=0.3)
plt.show()