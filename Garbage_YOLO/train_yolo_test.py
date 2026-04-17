import cv2
from ultralytics import YOLO

# 1. 加载你训练好的模型
model = YOLO(r"C:\Users\seeyousx845\runs\detect\train24\weights\best.pt")

# 2. 打开摄像头
cap = cv2.VideoCapture(0)

# 检查摄像头是否正常打开
if not cap.isOpened():
    print("无法打开摄像头，请检查占用或驱动")
    exit()

print("按下 'q' 键退出程序")

while True:
    # 读取摄像头的一帧
    ret, frame = cap.read()
    if not ret:
        break

    # 3. 对这一帧进行推理 (不使用 show=True，我们自己画)
    # stream=True 模式更节省内存
    results = model(frame, conf=0.3)

    # 4. 获取画好框的图片
    # plot() 会返回一个带有检测框和标签的 numpy 数组（图片）
    annotated_frame = results[0].plot()

    # 5. 用 OpenCV 显示这个画面
    cv2.imshow("YOLO Garbage Detection", annotated_frame)

    # 按 'q' 键退出循环
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 释放资源
cap.release()
cv2.destroyAllWindows()