import torch
import torch.nn as nn
import torch.optim as optim
from torchvision import datasets, transforms
from torch.utils.data import DataLoader

# ==========================================
# 1. 准备数据 (Data Preparation)
# 对应概念：深度学习需要大量数据作为“教材”
# ==========================================
print(">>> 步骤 1: 加载数据...")

# 定义预处理：将图片转换为张量(Tensor)，并归一化
transform = transforms.Compose([
    transforms.ToTensor(), 
    transforms.Normalize((0.1307,), (0.3081,))
])

# 下载并加载训练集 (6万张图片) 和 测试集 (1万张图片)
train_dataset = datasets.MNIST(root='./data', train=True, download=True, transform=transform)
test_dataset = datasets.MNIST(root='./data', train=False, download=True, transform=transform)

# 使用 DataLoader 分批加载数据 (Batch Size = 64)
# 深度学习通常不一次性把所有数据喂给模型，而是分批吃
train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=1000, shuffle=False)

# ==========================================
# 2. 定义模型 (Define the Model)
# 对应概念：神经网络结构 (输入层 -> 隐藏层 -> 输出层)
# ==========================================
print(">>> 步骤 2: 构建神经网络...")

class SimpleNet(nn.Module):
    def __init__(self):
        super(SimpleNet, self).__init__()
        # 输入层：28*28 = 784 个像素点
        # 隐藏层：128 个神经元 (使用 ReLU 激活函数，增加非线性)
        # 输出层：10 个神经元 (代表数字 0-9 的概率)
        self.fc1 = nn.Linear(784, 10) 
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(10, 10)

    def forward(self, x):
        # 前向传播：数据流动的方向
        x = x.view(-1, 784)  # 将 28x28 的图片拉平成 784 的向量
        x = self.fc1(x)
        x = self.relu(x)     # 激活函数
        x = self.fc2(x)
        return x

model = SimpleNet()
print(f"模型结构:\n{model}")

# ==========================================
# 3. 定义损失函数和优化器 (Loss & Optimizer)
# 对应概念：如何衡量错误？如何修正错误？
# ==========================================
# 损失函数：交叉熵损失 (常用于分类问题)，衡量预测值和真实值的差距
criterion = nn.CrossEntropyLoss()
# 优化器：Adam (一种高效的梯度下降变体)，负责更新参数
optimizer = optim.Adam(model.parameters(), lr=0.001)

# ==========================================
# 4. 训练模型 (Training Loop)
# 对应概念：迭代学习，反向传播，梯度下降
# ==========================================
print(">>> 步骤 3: 开始训练 (模拟 1 个 Epoch)...")

epochs = 1  # 为了演示快，只训练 1 轮。实际通常训练 5-10 轮

for epoch in range(epochs):
    model.train() # 设置为训练模式
    total_loss = 0
    
    for batch_idx, (data, target) in enumerate(train_loader):
        # A. 清零梯度 (每次更新前要把旧的梯度清空)
        optimizer.zero_grad()
        
        # B. 前向传播 (Forward Pass): 输入数据，得到预测结果
        output = model(data)
        
        # C. 计算损失 (Compute Loss): 预测结果和真实标签差多少？
        loss = criterion(output, target)
        
        # D. 反向传播 (Backward Pass): 计算梯度，找出哪个参数导致了误差
        loss.backward()
        
        # E. 更新参数 (Update Weights): 根据梯度调整参数，减小误差
        optimizer.step()
        
        total_loss += loss.item()
        
        if batch_idx % 100 == 0:
            print(f"Epoch {epoch}, Batch {batch_idx}, Loss: {loss.item():.4f}")

    print(f"Epoch {epoch} 完成，平均损失: {total_loss / len(train_loader):.4f}")

# ==========================================
# 5. 评估与预测 (Evaluation & Prediction)
# 对应概念：验证学习效果，实际应用
# ==========================================
print(">>> 步骤 4: 测试模型...")

model.eval() # 设置为评估模式 (关闭 Dropout 等训练特有机制)
correct = 0
total = 0

with torch.no_grad(): # 测试时不需要计算梯度，节省内存
    for data, target in test_loader:
        output = model(data)
        # 获取概率最大的类别作为预测结果
        _, predicted = torch.max(output.data, 1)
        total += target.size(0)
        correct += (predicted == target).sum().item()

accuracy = 100 * correct / total
print(f"测试集准确率: {accuracy:.2f}%")

print("\n>>> 恭喜！你已经跑通了第一个深度学习程序！")
print("这个过程就是：数据输入 -> 模型猜测 -> 计算误差 -> 反向修正 -> 再次猜测... 直到变聪明。")