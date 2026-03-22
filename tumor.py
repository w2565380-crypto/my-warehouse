import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import classification_report
import os
import platform

# ==========================================
# 🚑 Windows 11 字体急救包
# ==========================================
def fix_chinese_font_win11():
    # 1. 定义 Windows 11 常见的中文字体文件名
    # msyh.ttc (微软雅黑) 是 Win10/11 最稳的字体，SimHei.ttf (黑体) 是经典备用
    font_candidates = [
        r"C:\Windows\Fonts\msyh.ttc",       # 微软雅黑 (首选，字形现代)
        r"C:\Windows\Fonts\simhei.ttf",     # 黑体 (经典，粗壮)
        r"C:\Windows\Fonts\simsun.ttc",     # 宋体 (备选)
    ]
    
    font_path = None
    for path in font_candidates:
        if os.path.exists(path):
            font_path = path
            print(f"✅ 找到可用字体文件：{path}")
            break
    
    if not font_path:
        print("❌ 未找到常见中文字体文件，将尝试使用系统默认名称。")
        plt.rcParams['font.sans-serif'] = ['Microsoft YaHei', 'SimHei', 'Arial Unicode MS']
    else:
        # 2. 【关键步骤】直接通过文件路径加载字体属性
        # 这样绕过 Matplotlib 的字体名称查找机制，100% 生效
        from matplotlib.font_manager import FontProperties
        global my_font
        my_font = FontProperties(fname=font_path)
        print(f"🎨 已强制加载字体：{os.path.basename(font_path)}")

    # 3. 修复负号显示为方块的问题
    plt.rcParams['axes.unicode_minus'] = False
    
    # 4. 设置全局样式
    plt.style.use('seaborn-v0_8-whitegrid')
    plt.rcParams['figure.figsize'] = (10, 6)
    plt.rcParams['axes.titlesize'] = 16
    plt.rcParams['axes.labelsize'] = 14
    plt.rcParams['xtick.labelsize'] = 12
    plt.rcParams['ytick.labelsize'] = 12
    plt.rcParams['legend.fontsize'] = 12

# 执行修复
fix_chinese_font_win11()

# ==========================================
# 数据准备 (肿瘤案例)
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
# 模型训练
# ==========================================
model = LogisticRegression()
model.fit(X, y)
w = model.coef_[0][0]
b = model.intercept_[0]
threshold_x = -b/w

print(f"\n✅ 模型训练完成:")
print(f"   临界点：{threshold_x:.4f} cm")

# ==========================================
# 绘图 (使用局部 fontproperties 参数，确保万无一失)
# ==========================================
X_plot = np.linspace(0, 8, 200).reshape(-1, 1)
y_prob = model.predict_proba(X_plot)[:, 1]

fig, ax = plt.subplots()

# 散点图
ax.scatter(X[y==0], y[y==0], c='#2ca02c', s=120, label='良性 (Benign)', edgecolors='black', linewidth=1.2, alpha=0.8, zorder=5)
ax.scatter(X[y==1], y[y==1], c='#d62728', s=120, label='恶性 (Malignant)', edgecolors='black', linewidth=1.2, alpha=0.8, zorder=5)

# 曲线
ax.plot(X_plot, y_prob, 'b-', linewidth=3, label='恶性概率曲线 P(y=1)', zorder=4)

# 辅助线
ax.axhline(0.5, color='gray', linestyle='--', linewidth=2, label='决策阈值 (0.5)', alpha=0.7)
ax.axvline(threshold_x, color='#9467bd', linestyle=':', linewidth=3, label=f'临界点 ({threshold_x:.2f}cm)', zorder=3)

# --- 【核心修改】所有文本都显式传入 fontproperties=my_font ---
# 标题
ax.set_title('逻辑回归经典案例：肿瘤大小 vs 恶性概率', fontproperties=my_font, fontweight='bold', pad=15)
# 坐标轴标签
ax.set_xlabel('肿瘤大小 (cm)', fontproperties=my_font, fontweight='bold')
ax.set_ylabel('预测概率 (0=良性, 1=恶性)', fontproperties=my_font, fontweight='bold')
# 刻度标签 (如果需要确保刻度也是中文，虽然这里主要是数字)
# ax.set_xticklabels(ax.get_xticks(), fontproperties=my_font) 
# ax.set_yticklabels(ax.get_yticks(), fontproperties=my_font)

# 图例 (显式指定字体)
ax.legend(loc='upper left', frameon=True, fancybox=True, shadow=True, facecolor='white', prop=my_font)

# 文本标注 (带背景框)
ax.text(threshold_x + 0.15, 0.58, f'临界点\n{threshold_x:.2f}cm', 
        fontproperties=my_font, color='#9467bd', fontsize=13, fontweight='bold',
        bbox=dict(facecolor='white', edgecolor='#9467bd', alpha=0.8, boxstyle='round,pad=0.5'))

ax.set_xlim(0, 8)
ax.set_ylim(-0.1, 1.1)
ax.grid(True, which='both', linestyle='--', alpha=0.4)

plt.tight_layout()
plt.show()

# 评估报告
y_pred = model.predict(X)
print("\n📊 模型评估报告:")
print(classification_report(y, y_pred, target_names=['良性 Benign', '恶性 Malignant']))