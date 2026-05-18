import os
# 1. 强行修复 OpenMP 冲突报错
os.environ["KMP_DUPLICATE_LIB_OK"] = "TRUE"

import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
import matplotlib.pyplot as plt

# 设置随机种子保证结果可复现
torch.manual_seed(42)
np.random.seed(42)

# 2. 数据生成：模仿异或（XOR）特点的线性不可分二维样本
def gen_xor_data(n_samples=400, noise=0.1):
    # 随机生成 [-2, 2] 之间的二维坐标点
    X = np.random.uniform(-2, 2, (n_samples, 2))
    # 象限异或规律：一、三象限（x*y > 0）为标签 0，二、四象限（x*y < 0）为标签 1
    y = (X[:, 0] * X[:, 1] < 0).astype(int)
    
    # 引入噪声：随机翻转部分标签
    flip_mask = np.random.rand(n_samples) < noise
    y[flip_mask] = 1 - y[flip_mask]
    
    return (torch.tensor(X, dtype=torch.float32), 
            torch.tensor(y, dtype=torch.float32).view(-1, 1))

X_cls, y_cls = gen_xor_data(n_samples=500, noise=0.1)

# 3. 定义多层感知机 (MLP 分类器)
class MLPClassifier(nn.Module):
    def __init__(self, use_act=True, hidden_dim=32):
        super().__init__()
        self.use_act = use_act
        # 纯 Sequential 结构，方便控制是否包含激活函数
        if use_act:
            self.net = nn.Sequential(
                nn.Linear(2, hidden_dim),
                nn.ReLU(),
                nn.Linear(hidden_dim, hidden_dim),
                nn.ReLU(),
                nn.Linear(hidden_dim, 1),
                nn.Sigmoid()  # 输出 0~1 之间的概率值
            )
        else:
            self.net = nn.Sequential(
                nn.Linear(2, hidden_dim),
                nn.Linear(hidden_dim, hidden_dim),
                nn.Linear(hidden_dim, 1),
                nn.Sigmoid()
            )

    def forward(self, x):
        return self.net(x)

# 4. 训练函数（带控制台打印与准确率计算）
def train_classifier(model, model_name, epochs=500, lr=0.02):
    print(f"\n开始训练模型: {model_name}...")
    criterion = nn.BCELoss()  # 二分类交叉熵损失
    optimizer = optim.Adam(model.parameters(), lr=lr)
    loss_history = []
    
    for epoch in range(1, epochs + 1):
        model.train()
        optimizer.zero_grad()
        
        pred = model(X_cls)
        loss = criterion(pred, y_cls)
        loss.backward()
        optimizer.step()
        loss_history.append(loss.item())
        
        # 计算当前准确率
        with torch.no_grad():
            preds_bin = (pred >= 0.5).float()
            accuracy = (preds_bin == y_cls).float().mean().item() * 100
            
        # 每 100 轮打印一次进度
        if epoch % 100 == 0 or epoch == 1:
            print(f"  Epoch {epoch:3d}/{epochs} - Loss: {loss.item():.4f} - Accuracy: {accuracy:.1f}%")
            
    print(f"模型 {model_name} 训练完成！最终最低 Loss: {loss_history[-1]:.4f}")
    return loss_history

# 5. 实例化并跑训练
model_cls_standard = MLPClassifier(use_act=True)
model_cls_no_act = MLPClassifier(use_act=False)

loss_standard = train_classifier(model_cls_standard, "标准MLP分类器 (带ReLU)")
loss_no_act = train_regression = train_classifier(model_cls_no_act, "无激活函数MLP分类器")

# 6. 绘制决策边界的辅助函数
def plot_decision_boundary(model, ax, title):
    model.eval()
    # 在 [-2.2, 2.2] 范围内生成密集的网格点
    x_min, x_max = -2.2, 2.2
    y_min, y_max = -2.2, 2.2
    xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.02),
                         np.arange(y_min, y_max, 0.02))
    
    # 将网格点平铺后转为 Tensor 喂给模型
    grid_points = np.c_[xx.ravel(), yy.ravel()]
    grid_tensor = torch.tensor(grid_points, dtype=torch.float32)
    
    with torch.no_grad():
        pred_probs = model(grid_tensor).reshape(xx.shape).numpy()
    
    # 绘制背景颜色深浅（代表模型预测的概率边界）
    ax.contourf(xx, yy, pred_probs, cmap=plt.cm.RdYlBu, alpha=0.6)
    
    # 绘制原始样本散点图
    X_np = X_cls.numpy()
    y_np = y_cls.numpy().ravel()
    ax.scatter(X_np[y_np == 0, 0], X_np[y_np == 0, 1], c='blue', edgecolors='k', label='Class 0', s=20)
    ax.scatter(X_np[y_np == 1, 0], X_np[y_np == 1, 1], c='red', edgecolors='k', label='Class 1', s=20)
    
    ax.set_title(title)
    ax.set_xlim(x_min, x_max)
    ax.set_ylim(y_min, y_max)
    ax.grid(True, alpha=0.3)

# 7. 可视化结果绘图
plt.figure(figsize=(14, 5))

# 子图1：Loss 曲线对比
plt.subplot(1, 3, 1)
plt.plot(loss_standard, label='Standard MLP (with ReLU)', color='blue')
plt.plot(loss_no_act, label='No Activation MLP', color='red', linestyle='--')
plt.title('Training Loss Curve')
plt.xlabel('Epochs')
plt.ylabel('BCELoss')
plt.legend()
plt.grid(True)

# 子图2：标准模型的决策边界
ax2 = plt.subplot(1, 3, 2)
plot_decision_boundary(model_cls_standard, ax2, "Standard MLP Decision Boundary")

# 子图3：无激活函数模型的决策边界
ax3 = plt.subplot(1, 3, 3)
plot_decision_boundary(model_cls_no_act, ax3, "No Activation Decision Boundary")

print("\n正在生成非线性分类可视化图表，请稍候...")
plt.show()