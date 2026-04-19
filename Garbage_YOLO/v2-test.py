import cv2
from ultralytics import YOLO

# 1. 指向你刚出炉的 v2 版权重文件
model_path = r"C:\Users\seeyousx845\runs\detect\garbage_v2\weights\best.pt"
model = YOLO(model_path)

# 2. 开启摄像头
cap = cv2.VideoCapture(0)

print("正在启动摄像头检测... 按下 'q' 键退出")

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    # 3. 推理（conf=0.5 过滤掉不确定的框，让画面更干净）
    results = model(frame, conf=0.7)

    # 4. 绘制结果并显示
    annotated_frame = results[0].plot()
    cv2.imshow("Garbage YOLO v2 - Live", annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()