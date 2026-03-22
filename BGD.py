import numpy as np
import matplotlib.pyplot as plt
import platform
import os

# ================= 🛠️ 配置中文显示 (关键步骤) =================
def setup_chinese_font():
    """
    根据操作系统自动设置支持中文的字体，解决绘图乱码问题。
    """
    system_name = platform.system()
    
    if system_name == 'Windows':
        # Windows 常用：微软雅黑 (Microsoft YaHei)
        plt.rcParams['font.sans-serif'] = ['Microsoft YaHei', 'SimHei', 'Arial Unicode MS']
    elif system_name == 'Darwin':  # macOS
        # macOS 常用：宋体 (Songti SC) 或 黑体 (Heiti SC)
        plt.rcParams['font.sans-serif'] = ['STSong', 'Heiti SC', 'Arial Unicode MS']
    else:  # Linux
        # Linux 通常需要安装字体，这里尝试常用开源字体
        plt.rcParams['font.sans-serif'] = ['WenQuanYi Micro Hei', 'DejaVu Sans', 'Arial Unicode MS']
    
    # 解决负号 '-' 显示为方块的问题
    plt.rcParams['axes.unicode_minus'] = False

# 执行配置
setup_chinese_font()

# ================= 1. 准备数据 =================
np.random.seed(42)

m = 100
X = 10 * np.random.rand(m, 1)       # 特征 (100行, 1列)
true_w = 2.0
true_b = 1.0
y = true_w * X + true_b + np.random.randn(m, 1) * 2  # 目标值

# ================= 2. 定义辅助函数 =================

def compute_cost(X, y, w, b):
    """计算均方误差成本"""
    m = len(y)
    y_hat = np.dot(X, w) + b
    errors = y_hat - y
    cost = (1 / (2 * m)) * np.sum(errors ** 2)
    return cost

def batch_gradient_descent(X, y, w_init, b_init, alpha, iterations):
    """批量梯度下降 (BGD)"""
    m = len(y)
    w = w_init.copy()
    b = b_init
    
    cost_history = []
    
    # 注意：这里打印汉字依赖于控制台字体，如果控制台乱码请检查终端设置
    print(f"开始训练... (初始成本: {compute_cost(X, y, w, b):.4f})")
    
    for i in range(iterations):
        # 1. 预测
        y_hat = np.dot(X, w) + b
        # 2. 误差
        errors = y_hat - y
        # 3. 计算梯度
        dw = (1 / m) * np.dot(X.T, errors)
        db = (1 / m) * np.sum(errors)
        # 4. 更新参数
        w = w - alpha * dw
        b = b - alpha * db
        
        # 记录成本
        current_cost = compute_cost(X, y, w, b)
        cost_history.append(current_cost)
        
        # 打印进度
        if i % 100 == 0:
            print(f"Iter {i:5d} | Cost: {current_cost:.6f} | w: {w[0][0]:.4f}, b: {b:.4f}")
            
    print(f"训练结束! (最终成本: {cost_history[-1]:.6f})")
    return w, b, cost_history

# ================= 3. 执行训练 =================
w_initial = np.zeros((1, 1))
b_initial = 0.0
learning_rate = 0.01
n_iterations = 1000

final_w, final_b, costs = batch_gradient_descent(
    X, y, w_initial, b_initial, learning_rate, n_iterations
)

# ================= 4. 结果可视化 (修改版) =================
fig, ax = plt.subplots(1, 2, figsize=(14, 5))

# --- 图 1: 成本下降曲线 (只看前 50 次) ---
# 我们只取前 50 个点来画，这样能看清细微的变化
limit_iter = 50 
ax[0].plot(costs[:limit_iter], color='#1f77b4', linewidth=2, marker='o', markersize=4)
ax[0].set_title(f'批量梯度下降：成本收敛曲线 (前 {limit_iter} 次)', fontsize=15, fontweight='bold')
ax[0].set_xlabel('迭代次数', fontsize=12)
ax[0].set_ylabel('成本 (Cost)', fontsize=12)
ax[0].grid(True, linestyle='--', alpha=0.6)

# 添加数值标注，让你看清每一步降了多少
for i in range(0, limit_iter, 5): # 每 5 步标一个数
    ax[0].text(i, costs[i], f'{costs[i]:.2f}', fontsize=8, ha='right', va='bottom')

# --- 图 2: 拟合效果 (保持不变) ---
X_line = np.linspace(0, 10, 100).reshape(-1, 1)
y_pred_line = np.dot(X_line, final_w) + final_b

ax[1].scatter(X, y, color='gray', alpha=0.6, label='原始数据 (带噪声)', edgecolors='k', s=50)
ax[1].plot(X_line, y_pred_line, color='#d62728', linewidth=3, label=f'模型拟合 (y={final_w[0][0]:.2f}x + {final_b:.2f})')
ax[1].plot(X_line, true_w * X_line + true_b, color='#2ca02c', linestyle='--', linewidth=2, label=f'真实关系 (y={true_w}x + {true_b})')

ax[1].set_title('回归拟合效果对比', fontsize=15, fontweight='bold')
ax[1].set_xlabel('特征 X', fontsize=12)
ax[1].set_ylabel('目标 y', fontsize=12)
ax[1].legend(fontsize=11, frameon=True, shadow=True)
ax[1].grid(True, linestyle='--', alpha=0.6)

plt.tight_layout()
plt.show()

# ================= 5. 最终结论 =================
print("\n" + "="*40)
print("最终模型参数结果:")
print(f"学习到的权重 w: {final_w[0][0]:.4f}  (真实值：{true_w})")
print(f"学习到的偏置 b: {final_b:.4f}        (真实值：{true_b})")
print("="*40)