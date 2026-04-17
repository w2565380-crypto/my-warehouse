from ultralytics import YOLO
import cv2

# 1. 加载官方预训练模型 (它已经认识 80 种生活用品了，比如杯子、手机、人)
model = YOLO("yolo11n.pt") 

# 2. 开启摄像头实时检测
# 如果你没有摄像头，可以把 0 换成一张图片的路径
results = model.predict(source="0", show=True)