# ♻️ 基于 YOLO11 的智能垃圾分类系统

本项目使用 YOLO11n 模型，针对“充电宝”等特定垃圾进行了 50 张样本的微调训练。

## 🚀 功能
- 实时摄像头检测
- Gradio Web 交互界面
- 支持 GPU (RTX 4060) 加速推理

## 🛠️ 如何运行
1. 安装依赖：`pip install -r requirements.txt`
2. 启动 Web 界面：`python app.py`
3. 启动实时检测：`python inference.py`

## 相关环境
- ultralytics
- gradio
- opencv-python