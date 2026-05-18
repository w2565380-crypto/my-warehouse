import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset

# --- 任务 1: 张量操作展示 ---
print("="*20, "任务 1: 张量操作", "="*20)
t1 = torch.randn(2, 3)
print(f"1. 随机张量:\n{t1}")
print(f"2. 形状: {t1.shape}")

t_view = t1.view(3, 2)
print(f"3. 变换形状 (3,2):\n{t_view}")

x = torch.tensor([2.0], requires_grad=True)
y = x**2 + 3
y.backward()
print(f"4. 自动求导 (y=x^2+3, x=2): grad={x.grad.item()}")

# --- 任务 2 & 3 数据准备 ---
# 生成 y = 4x1 + 3x2 + 1 的数据
X = torch.randn(100, 2)
true_w = torch.tensor([[4.0], [3.0]])
true_b = 1.0
y = torch.matmul(X, true_w) + true_b + torch.randn(100, 1) * 0.01

# --- 任务 3: 使用框架实现线性回归 ---
print("\n" + "="*20, "任务 3: 框架实现线性回归", "="*20)

# 数据封装 [cite: 3]
dataset = TensorDataset(X, y)
loader = DataLoader(dataset, batch_size=16, shuffle=True)

# 定义模型 [cite: 3]
model = nn.Linear(2, 1)
criterion = nn.MSELoss()
optimizer = optim.SGD(model.parameters(), lr=0.1)

# 训练循环 [cite: 3]
for epoch in range(20):
    for batch_X, batch_y in loader:
        optimizer.zero_grad()
        output = model(batch_X)
        loss = criterion(output, batch_y)
        loss.backward()
        optimizer.step()
    
    if (epoch + 1) % 5 == 0:
        print(f"Epoch [{epoch+1}/20], Loss: {loss.item():.6f}")

print("\n训练后的模型参数:")
print(f"W: {model.weight.data.numpy()}")
print(f"b: {model.bias.data.numpy()}")