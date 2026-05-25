import torch
import torch.nn as nn
import torch.optim as optim
import torchvision
import torchvision.transforms as transforms
from torch.utils.data import DataLoader
import matplotlib.pyplot as plt
import numpy as np
from sklearn.metrics import confusion_matrix
import seaborn as sns
import time

# 1. 数据集加载与预处理
transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.5,), (0.5,))
])

train_set = torchvision.datasets.FashionMNIST(root='./data', train=True, download=True, transform=transform)
test_set = torchvision.datasets.FashionMNIST(root='./data', train=False, download=True, transform=transform)

train_loader = DataLoader(train_set, batch_size=64, shuffle=True)
test_loader = DataLoader(test_set, batch_size=64, shuffle=False)

classes = ('T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat', 'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot')

print(f"训练集规模: {len(train_set)} 张图片")
print(f"测试集规模: {len(test_set)} 张图片")
print(f"单张图像尺寸: {train_set[0][0].shape}\n")

# 2. 基础 CNN 模型定义
class BaseCNN(nn.Module):
    def __init__(self):
        super(BaseCNN, self).__init__()
        self.features = nn.Sequential(
            nn.Conv2d(1, 16, kernel_size=3, padding=1), # Layer 1
            nn.ReLU(),
            nn.MaxPool2d(2, 2), # 14x14
            nn.Conv2d(16, 32, kernel_size=3, padding=1), # Layer 2
            nn.ReLU(),
            nn.MaxPool2d(2, 2)  # 7x7
        )
        self.classifier = nn.Sequential(
            nn.Linear(32 * 7 * 7, 128),
            nn.ReLU(),
            nn.Linear(128, 10)
        )

    def forward(self, x):
        x = self.features(x)
        x = x.view(x.size(0), -1)
        x = self.classifier(x)
        return x

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
base_model = BaseCNN().to(device)

# 打印模型结构与可训练参数量
print(base_model)
total_params = sum(p.numel() for p in base_model.parameters() if p.requires_grad)
print(f"基础模型总可训练参数量: {total_params}\n")

# 3. 训练与测试逻辑
criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(base_model.parameters(), lr=0.01, momentum=0.9)

epochs = 10
base_train_losses, base_test_losses, base_test_accs = [], [], []

print("开始训练基础模型...")
for epoch in range(epochs):
    base_model.train()
    running_loss = 0.0
    for images, labels in train_loader:
        images, labels = images.to(device), labels.to(device)
        
        optimizer.zero_grad()
        outputs = base_model(images)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()
        
        running_loss += loss.item() * images.size(0)
    
    epoch_train_loss = running_loss / len(train_loader.dataset)
    
    # 测试集评估
    base_model.eval()
    epoch_test_loss = 0.0
    correct = 0
    with torch.no_grad():
        for images, labels in test_loader:
            images, labels = images.to(device), labels.to(device)
            outputs = base_model(images)
            loss = criterion(outputs, labels)
            epoch_test_loss += loss.item() * images.size(0)
            _, predicted = torch.max(outputs, 1)
            correct += (predicted == labels).sum().item()
            
    epoch_test_loss = epoch_test_loss / len(test_loader.dataset)
    epoch_test_acc = correct / len(test_loader.dataset) * 100
    
    base_train_losses.append(epoch_train_loss)
    base_test_losses.append(epoch_test_loss)
    base_test_accs.append(epoch_test_acc)
    
    print(f"Epoch [{epoch+1}/{epochs}] - Train Loss: {epoch_train_loss:.4f} | Test Loss: {epoch_test_loss:.4f} | Test Acc: {epoch_test_acc:.2f}%")