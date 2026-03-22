import numpy as np
import matplotlib.pyplot as plt
import platform

# ================= 🛠️ 字体设置 (保持不变) =================
def setup_chinese_font():
    system_name = platform.system()
    if system_name == 'Windows':
        plt.rcParams['font.sans-serif'] = ['Microsoft YaHei', 'SimHei']
    elif system_name == 'Darwin':
        plt.rcParams['font.sans-serif'] = ['PingFang SC', 'Heiti SC']
    else:
        plt.rcParams['font.sans-serif'] = ['WenQuanYi Micro Hei', 'SimHei']
    plt.rcParams['axes.unicode_minus'] = False 

setup_chinese_font()

# ================= 1. 数据准备 (保持不变) =================
np.random.seed(42)
X = 2 * np.random.rand(100, 1)
y = 1 + 2 * X + 0.1 * np.random.randn(100, 1)
X_b = np.c_[np.ones((100, 1)), X] 
theta_initial = np.random.randn(2, 1)

def compute_loss(X, y, theta):
    m = len(y)
    predictions = X.dot(theta)
    return (1 / (2 * m)) * np.sum((predictions - y) ** 2)

def gradient_descent(X, y, theta, learning_rate, n_iterations):
    m = len(y)
    loss_history = []
    for i in range(n_iterations):
        gradients = (1/m) * X.T.dot(X.dot(theta) - y)
        theta = theta - learning_rate * gradients
        loss = compute_loss(X, y, theta)
        loss_history.append(loss)
        if np.isnan(loss): break
    return theta, loss_history

# ================= 2. 绘图逻辑 (核心修改部分) =================
# 创建两个子图：ax1 看收敛细节，ax2 看发散
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True) # sharex=True 让横坐标对齐

learning_rates = [0.01, 0.1, 0.5, 1.2]
labels = ["太小 (Too Small)", "合适 (Just Right)", "太大 (Too Large)", "发散 (Divergent)"]
colors = ['gray', 'green', 'orange', 'red']
n_iterations = 50

# --- 绘制上图：收敛组 (排除发散的红线) ---
for i, lr in enumerate(learning_rates[:3]): # 只取前三个
    theta_start = theta_initial.copy()
    _, loss_hist = gradient_descent(X_b, y, theta_start, lr, n_iterations)
    
    ax1.plot(range(len(loss_hist)), loss_hist, label=f"{labels[i]} (α={lr})", color=colors[i], linewidth=2)

ax1.set_title('✅ 收敛情况对比 (放大细节)', fontsize=14, fontweight='bold')
ax1.set_ylabel('损失值 (Loss)', fontsize=12)
ax1.legend(loc='upper right')
ax1.grid(True, linestyle='--', alpha=0.6)
ax1.set_yscale('log') # 对数坐标看细节

# --- 绘制下图：发散组 (只看红线) ---
lr_divergent = learning_rates[3]
theta_start = theta_initial.copy()
_, loss_hist_div = gradient_descent(X_b, y, theta_start, lr_divergent, n_iterations)

ax2.plot(range(len(loss_hist_div)), loss_hist_div, label=f"{labels[3]} (α={lr_divergent})", color=colors[3], linewidth=2)

ax2.set_title('🚀 发散情况 (Loss 爆炸)', fontsize=14, fontweight='bold', color='red')
ax2.set_xlabel('迭代次数', fontsize=12)
ax2.set_ylabel('损失值 (Loss)', fontsize=12)
ax2.legend(loc='upper left')
ax2.grid(True, linestyle='--', alpha=0.6)
# 注意：发散图也可以不用 log 坐标，用线性坐标更能体现“爆炸”感，这里保持 log 方便对比数量级
ax2.set_yscale('log') 

plt.tight_layout()
plt.show()