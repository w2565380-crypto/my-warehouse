import os
os.environ["KMP_DUPLICATE_LIB_OK"] = "TRUE"

import torch
import matplotlib.pyplot as plt

# 1. 构造数据 y = 4x1 + 3x2 + 1
X = torch.randn(100, 2)
true_w = torch.tensor([4.0, 3.0])
true_b = 1.0
y = torch.matmul(X, true_w) + true_b + torch.randn(100) * 0.01

# 2. 初始化参数
w = torch.randn(2, requires_grad=True)
b = torch.zeros(1, requires_grad=True)

lr = 0.1
losses = []

# 3. 训练循环
for epoch in range(100):
    pred = torch.matmul(X, w) + b
    loss = torch.mean((pred - y)**2)
    
    loss.backward()
    with torch.no_grad():
        w -= lr * w.grad
        b -= lr * b.grad
        w.grad.zero_()
        b.grad.zero_()
    
    losses.append(loss.item())

print(f"Result: w={w.data}, b={b.data}")