import os
# 解决 OpenMP 重复初始化导致的程序崩溃问题
os.environ["KMP_DUPLICATE_LIB_OK"] = "TRUE"

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

# ==========================================
# 1. 环境配置与数据预处理
# ==========================================
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"当前运行设备: {device}")

transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.5,), (0.5,))
])

# 下载并加载 Fashion-MNIST 数据集
train_set = torchvision.datasets.FashionMNIST(root='./data', train=True, download=True, transform=transform)
test_set = torchvision.datasets.FashionMNIST(root='./data', train=False, download=True, transform=transform)

train_loader = DataLoader(train_set, batch_size=64, shuffle=True)
test_loader = DataLoader(test_set, batch_size=64, shuffle=False)

classes = ('T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat', 'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot')

print(f"训练集规模: {len(train_set)} 张图片")
print(f"测试集规模: {len(test_set)} 张图片")
print(f"单张图像尺寸: {train_set[0][0].shape}\n")


# ==========================================
# 2. 模型定义
# ==========================================
# 任务1：基础 CNN 模型
class BaseCNN(nn.Module):
    def __init__(self):
        super(BaseCNN, self).__init__()
        self.features = nn.Sequential(
            nn.Conv2d(1, 16, kernel_size=3, padding=1), 
            nn.ReLU(),
            nn.MaxPool2d(2, 2), 
            nn.Conv2d(16, 32, kernel_size=3, padding=1), 
            nn.ReLU(),
            nn.MaxPool2d(2, 2)  
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

# 任务2：改进版 CNN 模型（加入 BN, Dropout, 增大通道数）
class ImprovedCNN(nn.Module):
    def __init__(self):
        super(ImprovedCNN, self).__init__()
        self.features = nn.Sequential(
            nn.Conv2d(1, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.MaxPool2d(2, 2), 
            
            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.BatchNorm2d(64),
            nn.ReLU(),
            nn.MaxPool2d(2, 2)  
        )
        self.classifier = nn.Sequential(
            nn.Linear(64 * 7 * 7, 256),
            nn.BatchNorm1d(256),
            nn.ReLU(),
            nn.Dropout(0.4),
            nn.Linear(256, 10)
        )

    def forward(self, x):
        x = self.features(x)
        x = x.view(x.size(0), -1)
        x = self.classifier(x)
        return x


# ==========================================
# 3. 通用训练与评估函数
# ==========================================
def train_and_evaluate(model, train_loader, test_loader, criterion, optimizer, epochs=10):
    train_losses, test_losses, test_accs = [], [], []
    
    for epoch in range(epochs):
        model.train()
        running_loss = 0.0
        for images, labels in train_loader:
            images, labels = images.to(device), labels.to(device)
            
            optimizer.zero_grad()
            outputs = model(images)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()
            
            running_loss += loss.item() * images.size(0)
        
        epoch_train_loss = running_loss / len(train_loader.dataset)
        
        # 测试集评估
        model.eval()
        epoch_test_loss = 0.0
        correct = 0
        with torch.no_grad():
            for images, labels in test_loader:
                images, labels = images.to(device), labels.to(device)
                outputs = model(images)
                loss = criterion(outputs, labels)
                epoch_test_loss += loss.item() * images.size(0)
                _, predicted = torch.max(outputs, 1)
                correct += (predicted == labels).sum().item()
                
        epoch_test_loss = epoch_test_loss / len(test_loader.dataset)
        epoch_test_acc = correct / len(test_loader.dataset) * 100
        
        train_losses.append(epoch_train_loss)
        test_losses.append(epoch_test_loss)
        test_accs.append(epoch_test_acc)
        
        print(f"Epoch [{epoch+1}/{epochs}] - Train Loss: {epoch_train_loss:.4f} | Test Loss: {epoch_test_loss:.4f} | Test Acc: {epoch_test_acc:.2f}%")
        
    return train_losses, test_losses, test_accs


# ==========================================
# 4. 执行实验
# ==========================================
epochs = 10
criterion = nn.CrossEntropyLoss()

# --- 训练基础模型 ---
base_model = BaseCNN().to(device)
print("-" * 50)
print(f"基础模型总参数量: {sum(p.numel() for p in base_model.parameters() if p.requires_grad)}")
optimizer_base = optim.SGD(base_model.parameters(), lr=0.01, momentum=0.9)

print("开始训练基础模型...")
base_train_loss, base_test_loss, base_test_acc = train_and_evaluate(
    base_model, train_loader, test_loader, criterion, optimizer_base, epochs
)

# --- 训练改进模型 ---
improved_model = ImprovedCNN().to(device)
print("-" * 50)
print(f"改进模型总参数量: {sum(p.numel() for p in improved_model.parameters() if p.requires_grad)}")
optimizer_imp = optim.Adam(improved_model.parameters(), lr=0.001)

print("开始训练改进模型...")
start_time = time.time()
imp_train_loss, imp_test_loss, imp_test_acc = train_and_evaluate(
    improved_model, train_loader, test_loader, criterion, optimizer_imp, epochs
)
print(f"改进模型训练总耗时: {time.time() - start_time:.2f} 秒")


# ==========================================
# 5. 结果可视化与图表保存
# ==========================================
print("-" * 50)
print("正在生成对比图表...")

# 绘制 Loss 和 Accuracy 曲线
epochs_range = range(1, epochs + 1)
plt.figure(figsize=(14, 5))

# Loss 曲线对比
plt.subplot(1, 2, 1)
plt.plot(epochs_range, base_train_loss, 'r--', label='Base Train Loss')
plt.plot(epochs_range, base_test_loss, 'r-', label='Base Test Loss')
plt.plot(epochs_range, imp_train_loss, 'b--', label='Improved Train Loss')
plt.plot(epochs_range, imp_test_loss, 'b-', label='Improved Test Loss')
plt.title('Training and Testing Loss')
plt.xlabel('Epochs')
plt.ylabel('Loss')
plt.legend()
plt.grid(True)

# Accuracy 曲线对比
plt.subplot(1, 2, 2)
plt.plot(epochs_range, base_test_acc, 'r-', label='Base Test Acc')
plt.plot(epochs_range, imp_test_acc, 'b-', label='Improved Test Acc')
plt.title('Test Accuracy Comparison')
plt.xlabel('Epochs')
plt.ylabel('Accuracy (%)')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig('metrics_comparison.png') # 保存曲线图
print("已成功保存曲线对比图至本地: 'metrics_comparison.png'")

# 生成并保存改进模型的混淆矩阵
improved_model.eval()
all_preds = []
all_labels = []
with torch.no_grad():
    for images, labels in test_loader:
        images = images.to(device)
        outputs = improved_model(images)
        _, predicted = torch.max(outputs, 1)
        all_preds.extend(predicted.cpu().numpy())
        all_labels.extend(labels.numpy())

cm = confusion_matrix(all_labels, all_preds)
plt.figure(figsize=(10, 8))
sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', xticklabels=classes, yticklabels=classes)
plt.title('Confusion Matrix - Improved Model')
plt.ylabel('Actual Label')
plt.xlabel('Predicted Label')
plt.tight_layout()
plt.savefig('confusion_matrix.png') # 保存混淆矩阵
print("已成功保存混淆矩阵热力图至本地: 'confusion_matrix.png'")
print("所有任务执行完毕！")