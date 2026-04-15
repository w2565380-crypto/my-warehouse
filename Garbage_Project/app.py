import gradio as gr
import torch
from torchvision import models, transforms
from PIL import Image
import os
import re

# --- 1. 定义映射字典 (这是你的“翻译官”) ---
label_map_chinese = {
    "0": "其他垃圾/一次性快餐盒", "1": "其他垃圾/污损塑料", "2": "其他垃圾/烟蒂", "3": "其他垃圾/牙签",
    "4": "其他垃圾/破碎花盆及碟碗", "5": "其他垃圾/竹筷", "6": "厨余垃圾/剩饭剩菜", "7": "厨余垃圾/大骨头",
    "8": "厨余垃圾/水果果皮", "9": "厨余垃圾/水果果肉", "10": "厨余垃圾/茶叶渣", "11": "厨余垃圾/菜叶菜根",
    "12": "厨余垃圾/蛋壳", "13": "厨余垃圾/鱼骨", "14": "可回收物/充电宝", "15": "可回收物/包",
    "16": "可回收物/化妆品瓶", "17": "可回收物/塑料玩具", "18": "可回收物/塑料碗盆", "19": "可回收物/塑料衣架",
    "20": "可回收物/快递纸袋", "21": "可回收物/插头电线", "22": "可回收物/旧衣服", "23": "可回收物/易拉罐",
    "24": "可回收物/枕头", "25": "可回收物/毛绒玩具", "26": "可回收物/洗发水瓶", "27": "可回收物/玻璃杯",
    "28": "可回收物/皮鞋", "29": "可回收物/砧板", "30": "可回收物/纸板箱", "31": "可回收物/调料瓶",
    "32": "可回收物/酒瓶", "33": "可回收物/金属食品罐", "34": "可回收物/锅", "35": "可回收物/食用油桶",
    "36": "可回收物/饮料瓶", "37": "有害垃圾/干电池", "38": "有害垃圾/软膏", "39": "有害垃圾/过期药物"
}

# --- 2. 标签索引对齐 (模拟 PyTorch 训练时的字典序) ---
# 确保这个路径指向你电脑上真实的训练集文件夹
data_path = 'G:/ML/Garbage_Project/data/processed/train'
if os.path.exists(data_path):
    classes = sorted(os.listdir(data_path)) 
else:
    classes = sorted([str(i) for i in range(40)])

# --- 3. 加载模型 ---
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model = models.resnet18()
model.fc = torch.nn.Linear(model.fc.in_features, 40)

# 加载你训练好的权重
model_path = r'G:\ML\Garbage_Project\garbage_resnet18_augmented_v2.pth'
if os.path.exists(model_path):
    model.load_state_dict(torch.load(model_path, map_location=device))
    print(f"成功加载模型权重: {model_path}")
else:
    print(f"找不到模型文件: {model_path}，请确保文件在当前目录下")

model.to(device)
model.eval()

# --- 4. 预测逻辑 ---
def predict(img):
    if img is None: return None
    
    transform = transforms.Compose([
        transforms.Resize((224, 224)),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ])
    
    img = Image.fromarray(img.astype('uint8'), 'RGB')
    img_tensor = transform(img).unsqueeze(0).to(device)
    
    with torch.no_grad():
        outputs = model(img_tensor)
        probs = torch.nn.functional.softmax(outputs[0], dim=0)
    
    top3_prob, top3_idx = torch.topk(probs, 3)
    
    results = {}
    for prob, idx in zip(top3_prob, top3_idx):
        folder_name = classes[idx.item()]
        display_name = label_map_chinese.get(folder_name, f"未知({folder_name})")
        results[display_name] = float(prob)
    return results

# --- 5. 启动界面 ---
demo = gr.Interface(
    fn=predict, 
    inputs=gr.Image(), 
    outputs=gr.Label(num_top_classes=3), 
    title="🗑️ 智能垃圾分类系统 v4.0",
    description="上传垃圾照片，AI 将实时识别所属类别（ResNet18 驱动）。"
)

if __name__ == "__main__":
    # share=True 会生成一个公网链接
    demo.launch(share=True)