import os
os.environ["KMP_DUPLICATE_LIB_OK"] = "TRUE"

import torch
from torch.utils.data import DataLoader, TensorDataset
import torch.nn as nn
import torch.optim as optim

# ================= 1. 数据生成 (重点：定义 X 和 y) =================
torch.manual_seed(42) # 固定随机种子，方便复现结果

# 构造输入特征 X: 100个样本，每个样本2个特征 (x1, x2)
X = torch.randn(100, 2) 

# 设定真实权重和偏置: y = 4x1 + 3x2 + 1
true_w = torch.tensor([[4.0], [3.0]])
true_b = 1.0

# 计算 y 并加入少许噪声 (模拟真实数据)
y = torch.matmul(X, true_w) + true_b + torch.randn(100, 1) * 0.01

# ================= 2. 使用框架实现 (任务3要求) =================

# 1. 加载数据
dataset = TensorDataset(X, y)
loader = DataLoader(dataset, batch_size=16, shuffle=True) # 使用 mini-batch 

# 2. 定义模型: 输入维度2，输出维度1
model = nn.Linear(2, 1) # 使用 nn.Module 

# 3. 损失函数与优化器
criterion = nn.MSELoss()
optimizer = optim.SGD(model.parameters(), lr=0.1) # 使用 optim 

# 4. 训练循环
print("开始训练...")
for epoch in range(50):
    epoch_loss = 0
    for batch_X, batch_y in loader:
        optimizer.zero_grad()       # 梯度清零
        output = model(batch_X)     # 前向传播
        loss = criterion(output, batch_y) # 计算损失
        loss.backward()             # 反向传播
        optimizer.step()            # 更新参数
        epoch_loss += loss.item()
    
    if (epoch + 1) % 10 == 0:
        print(f"Epoch [{epoch+1}/50], Avg Loss: {epoch_loss/len(loader):.6f}")

# ================= 3. 输出最终结果 =================
print("\n" + "="*30)
print("训练后的模型参数:")
print(f"学习到的权重 (w1, w2): {model.weight.data.numpy()}")
print(f"学习到的偏置 (b): {model.bias.data.numpy()}")
print("="*30)