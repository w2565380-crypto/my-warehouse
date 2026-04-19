from ultralytics import YOLO
import os

# 解决 Windows 多线程问题
os.environ['KMP_DUPLICATE_LIB_OK']='True'

if __name__ == '__main__':
    # 加载预训练模型
    model = YOLO("yolo11n.pt") 
    
    # 开始训练
    model.train(
        data="G:/ML/Garbage_YOLO/data.yaml",
        epochs=100,         # 增加到 100 轮
        imgsz=640,          # 输入图片尺寸
        batch=8,            # 4060 显卡可以尝试 8 或 16
        workers=0,          # Windows 设为 0
        device=0,           # 使用 GPU
        name="garbage_v2"   # 给这次训练取个名字
    )