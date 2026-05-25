# 1. 改进版 CNN 模型定义
class ImprovedCNN(nn.Module):
    def __init__(self):
        super(ImprovedCNN, self).__init__()
        self.features = nn.Sequential(
            nn.Conv2d(1, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.MaxPool2d(2, 2), # 14x14
            
            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.BatchNorm2d(64),
            nn.ReLU(),
            nn.MaxPool2d(2, 2)  # 7x7
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

improved_model = ImprovedCNN().to(device)
print(improved_model)
imp_total_params = sum(p.numel() for p in improved_model.parameters() if p.requires_grad)
print(f"改进模型总可训练参数量: {imp_total_params}\n")

# 使用 Adam 优化器
optimizer_imp = optim.Adam(improved_model.parameters(), lr=0.001)
imp_train_losses, imp_test_losses, imp_test_accs = [], [], []

print("开始训练改进模型...")
start_time = time.time()
for epoch in range(epochs):
    improved_model.train()
    running_loss = 0.0
    for images, labels in train_loader:
        images, labels = images.to(device), labels.to(device)
        
        optimizer_imp.zero_grad()
        outputs = improved_model(images)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer_imp.step()
        
        running_loss += loss.item() * images.size(0)
        
    epoch_train_loss = running_loss / len(train_loader.dataset)
    
    # 评估
    improved_model.eval()
    epoch_test_loss = 0.0
    correct = 0
    with torch.no_grad():
        for images, labels in test_loader:
            images, labels = images.to(device), labels.to(device)
            outputs = improved_model(images)
            loss = criterion(outputs, labels)
            epoch_test_loss += loss.item() * images.size(0)
            _, predicted = torch.max(outputs, 1)
            correct += (predicted == labels).sum().item()
            
    epoch_test_loss = epoch_test_loss / len(test_loader.dataset)
    epoch_test_acc = correct / len(test_loader.dataset) * 100
    
    imp_train_losses.append(epoch_train_loss)
    imp_test_losses.append(epoch_test_loss)
    imp_test_accs.append(epoch_test_acc)
    
    print(f"Epoch [{epoch+1}/{epochs}] - Train Loss: {epoch_train_loss:.4f} | Test Loss: {epoch_test_loss:.4f} | Test Acc: {epoch_test_acc:.2f}%")

print(f"改进模型训练完成，总耗时: {time.time() - start_time:.2f}s")