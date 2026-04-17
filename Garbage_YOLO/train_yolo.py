import os
# 强制设置：防止 Windows 多线程导致的 OMP 错误
os.environ['KMP_DUPLICATE_LIB_OK']='True'

from ultralytics import YOLO

if __name__ == '__main__': # 必须加这一行，Windows 训练死理
    model = YOLO("yolo11n.pt") 
    
    results = model.train(
        data="G:/ML/Garbage_YOLO/data.yaml",
        epochs=50,
        imgsz=640,
        batch=4,       # 初始建议用 4，稳扎稳打
        workers=0,     # Windows 必须设为 0，否则容易报 CUDA 忙
        device=0,      # 指定显卡
        close_mosaic=10 # 最后10轮关闭马赛克增强，容易收敛
    )