import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import classification_report
import os

# ==========================================
# 🚑 Windows 11 字体急救包 (保持原样，确保中文显示)
# ==========================================
def fix_chinese_font_win11():
    font_candidates = [
        r"C:\Windows\Fonts\msyh.ttc",
        r"C:\Windows\Fonts\simhei.ttf",
        r"C:\Windows\Fonts\simsun.ttc",
    ]
    font_path = None
    for path in font_candidates:
        if os.path.exists(path):
            font_path = path
            break
    
    if not font_path:
        plt.rcParams['font.sans-serif'] = ['Microsoft YaHei', 'SimHei', 'Arial Unicode MS']
    else:
        from matplotlib.font_manager import FontProperties
        global my_font
        my_font = FontProperties(fname=font_path)

    plt.rcParams['axes.unicode_minus'] = False
    plt.style.use('seaborn-v0_8-whitegrid')
    plt.rcParams['figure.figsize'] = (10, 6)
    plt.rcParams['axes.titlesize'] = 16
    plt.rcParams['axes.labelsize'] = 14
    plt.rcParams['xtick.labelsize'] = 12
    plt.rcParams['ytick.labelsize'] = 12
    plt.rcParams['legend.fontsize'] = 12

fix_chinese_font_win11()

# ==========================================
# 1. 数据准备 (固定不变，模拟真实世界)
# ==========================================
np.random.seed(42)
n_samples = 40
X_benign = np.random.normal(loc=2.5, scale=0.8, size=n_samples//2).reshape(-1, 1)
y_benign = np.zeros(n_samples//2)
X_malignant = np.random.normal(loc=5.5, scale=0.8, size=n_samples//2).reshape(-1, 1)
y_malignant = np.ones(n_samples//2)

X = np.vstack((X_benign, X_malignant))
y = np.hstack((y_benign, y_malignant))
valid_idx = X[:, 0] > 0
X = X[valid_idx]
y = y[valid_idx]

# ==========================================
# 2. 模型训练 (固定不变，学习数据规律)
# ==========================================
model = LogisticRegression()
model.fit(X, y)
w = model.coef_[0][0]
b = model.intercept_[0]

# ==========================================
# 🎯 【唯一需要修改的地方】在这里改变阈值！
# ==========================================
# 尝试修改下面的数字：
# 0.5 -> 默认平衡策略
# 0.2 -> 医疗筛查策略 (宁可错杀，提高召回率)
# 0.8 -> 严格确诊策略 (宁可漏网，提高精确率)
prob_threshold = 0.8  
# ==========================================

# 自动计算：根据新的概率阈值，反推对应的肿瘤大小临界点 (x)
# 公式: x = (ln(P/(1-P)) - b) / w
log_odds = np.log(prob_threshold / (1 - prob_threshold))
threshold_x = (log_odds - b) / w

# 打印控制台信息
print(f"\n✅ 模型已训练完成 (数据未变)")
print(f"🎯 当前设定概率阈值: {prob_threshold}")
print(f"📏 对应肿瘤大小临界点: {threshold_x:.4f} cm")
print(f"   (规则: 肿瘤大小 > {threshold_x:.4f} cm 判定为恶性)")

# ==========================================
# 3. 动态绘图 (自动响应阈值变化)
# ==========================================
X_plot = np.linspace(0, 8, 200).reshape(-1, 1)
y_prob = model.predict_proba(X_plot)[:, 1]

fig, ax = plt.subplots()

# --- 散点图 (数据层，不变) ---
ax.scatter(X[y==0], y[y==0], c='#2ca02c', s=120, label='良性样本', edgecolors='black', linewidth=1.2, alpha=0.8, zorder=5)
ax.scatter(X[y==1], y[y==1], c='#d62728', s=120, label='恶性样本', edgecolors='black', linewidth=1.2, alpha=0.8, zorder=5)

# --- 概率曲线 (模型层，不变) ---
ax.plot(X_plot, y_prob, 'b-', linewidth=3, label='模型预测概率曲线', zorder=4)

# --- 动态辅助线 (决策层，随阈值变化) ---
# 1. 水平线：当前的概率阈值
ax.axhline(prob_threshold, color='#ff7f0e', linestyle='--', linewidth=2.5, label=f'决策概率阈值 ({prob_threshold})', alpha=0.9, zorder=3)
# 2. 垂直线：计算出的新临界点
ax.axvline(threshold_x, color='#9467bd', linestyle='-', linewidth=3, label=f'新临界点 ({threshold_x:.2f} cm)', zorder=3)

# 3. 参考线：默认的 0.5 (变淡，仅作对比)
if prob_threshold != 0.5:
    default_x = -b/w
    ax.axhline(0.5, color='gray', linestyle=':', linewidth=1, label='默认参考线 (0.5)', alpha=0.5)
    ax.axvline(default_x, color='gray', linestyle=':', linewidth=1, alpha=0.5)

# --- 动态文本标注 ---
strategy_text = "高灵敏度 (筛查)" if prob_threshold < 0.5 else ("高特异度 (确诊)" if prob_threshold > 0.5 else "默认平衡")
ax.set_title(f'逻辑回归决策边界调整\n当前策略：{strategy_text} (阈值={prob_threshold})', 
             fontproperties=my_font, fontweight='bold', pad=15)
ax.set_xlabel('肿瘤大小 (cm)', fontproperties=my_font, fontweight='bold')
ax.set_ylabel('恶性概率 P(y=1)', fontproperties=my_font, fontweight='bold')

# 动态标注框
ax.text(threshold_x + 0.15, prob_threshold + 0.08, f'判定界限\n>{threshold_x:.2f}cm', 
        fontproperties=my_font, color='#9467bd', fontsize=14, fontweight='bold',
        bbox=dict(facecolor='white', edgecolor='#9467bd', alpha=0.9, boxstyle='round,pad=0.6'))

ax.legend(loc='upper left', frameon=True, fancybox=True, shadow=True, facecolor='white', prop=my_font)
ax.set_xlim(0, 8)
ax.set_ylim(-0.1, 1.1)
ax.grid(True, which='both', linestyle='--', alpha=0.4)

plt.tight_layout()
plt.show()

# ==========================================
# 4. 动态评估报告 (自动响应阈值变化)
# ==========================================
# 关键：不使用 model.predict()，而是根据自定义阈值手动生成预测结果
y_prob_all = model.predict_proba(X)[:, 1]
y_pred_custom = (y_prob_all >= prob_threshold).astype(int)

print(f"\n📊 模型评估报告 (基于阈值 {prob_threshold}):")
print("-" * 30)
print(classification_report(y, y_pred_custom, target_names=['良性', '恶性'], zero_division=0))