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

# 2. 数据生成 (区间内 [-2π, 2π])
def gen_regression_data(n_samples=300, noise=0.1):
    X = np.linspace(-2 * np.pi, 2 * np.pi, n_samples)
    y = np.sin(X) + np.cos(2 * X)
    y_noise = y + np.random.normal(0, noise, n_samples)
    return (torch.tensor(X, dtype=torch.float32).view(-1, 1), 
            torch.tensor(y_noise, dtype=torch.float32).view(-1, 1),
            torch.tensor(y, dtype=torch.float32).view(-1, 1))

X_train, y_train, y_true = gen_regression_data(n_samples=300, noise=0.1)

# 测试集：用于评估区间外扩展能力 [-3π, 3π]
X_test = torch.linspace(-3 * np.pi, 3 * np.pi, 500).view(-1, 1)
y_test_true = torch.sin(X_test) + torch.cos(2 * X_test)

# 3. 定义多层感知机 (MLP)
class MLPRegression(nn.Module):
    def __init__(self, use_act=True, hidden_dim=64):
        super().__init__()
        self.use_act = use_act
        self.fc1 = nn.Linear(1, hidden_dim)
        self.fc2 = nn.Linear(hidden_dim, hidden_dim)
        self.fc3 = nn.Linear(hidden_dim, 1)
        self.act = nn.ReLU()

    def forward(self, x):
        if self.use_act:
            x = self.act(self.fc1(x))
            x = self.act(self.fc2(x))
        else:
            x = self.fc1(x)
            x = self.fc2(x)
        return self.fc3(x)

# 4. 训练函数（带控制台打印）
def train_regression(model, model_name, epochs=600, lr=0.01):
    print(f"\n开始训练模型: {model_name}...")
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters(), lr=lr)
    loss_history = []
    
    for epoch in range(1, epochs + 1):
        model.train()
        optimizer.zero_grad()
        pred = model(X_train)
        loss = criterion(pred, y_train)
        loss.backward()
        optimizer.step()
        loss_history.append(loss.item())
        
        # 每 100 轮打印一次进度
        if epoch % 100 == 0 or epoch == 1:
            print(f"  Epoch {epoch:3d}/{epochs} - Loss: {loss.item():.4f}")
            
    print(f"模型 {model_name} 训练完成！最终最低 Loss: {loss_history[-1]:.4f}")
    return loss_history

# 5. 实例化并跑训练
model_standard = MLPRegression(use_act=True)
model_no_act = MLPRegression(use_act=False)

loss_standard = train_regression(model_standard, "标准MLP模型 (带ReLU)")
loss_no_act = train_regression(model_no_act, "无激活函数MLP模型")

# 6. 获取模型预测值用于绘图
model_standard.eval()
model_no_act.eval()
with torch.no_grad():
    pred_standard = model_standard(X_test).numpy()
    pred_no_act = model_no_act(X_test).numpy()

# 7. 绘图可视化结果
plt.figure(figsize=(12, 5))

# 子图1：Loss 曲线对比
plt.subplot(1, 2, 1)
plt.plot(loss_standard, label='Standard MLP (with ReLU)', color='blue')
plt.plot(loss_no_act, label='No Activation MLP', color='red', linestyle='--')
plt.title('Training Loss Curve')
plt.xlabel('Epochs')
plt.ylabel('MSE Loss')
plt.legend()
plt.grid(True)

# 子图2：预测曲线与真实曲线对比
plt.subplot(1, 2, 2)
plt.scatter(X_train.numpy(), y_train.numpy(), color='gray', alpha=0.4, s=10, label='Train Data (with noise)')
plt.plot(X_test.numpy(), y_test_true.numpy(), color='black', linewidth=2, label='True Function')
plt.plot(X_test.numpy(), pred_standard, color='blue', linewidth=2, label='Standard MLP Predict')
plt.plot(X_test.numpy(), pred_no_act, color='red', linewidth=2, linestyle='--', label='No Activation Predict')

# 画出训练集的边界线（展示区间外泛化效果）
plt.axvline(x=-2*np.pi, color='green', linestyle=':', label='Train Boundary')
plt.axvline(x=2*np.pi, color='green', linestyle=':')

plt.title('Regression Curve Comparison')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)

print("\n正在生成可视化图表，请稍候...")
plt.show()