import numpy as np

def compute_cost(X, y, w, b):
    """
    X: 输入特征 (比如面积)
    y: 真实标签 (比如房价)
    w: 权重 (斜率)
    b: 偏置 (截距)
    """
    m = len(y)  # 样本数量
    
    # 1. 计算预测值: y_hat = w * x + b
    y_hat = w * X + b
    
    # 2. 计算误差: error = y_hat - y
    errors = y_hat - y
    
    # 3. 计算平方误差并求和
    squared_errors = errors ** 2
    total_squared_error = np.sum(squared_errors)
    
    # 4. 计算平均成本 (除以 2m)
    cost = total_squared_error / (2 * m)
    
    return cost

# 测试一下
X_test = np.array([1, 2, 3])
y_test = np.array([2, 4, 6]) # 完美线性关系 y = 2x
w_test = 1.2
b_test = 1.5

print(f"当前成本: {compute_cost(X_test, y_test, w_test, b_test)}") 
# 结果应该是 0.0，因为预测完全准确！