import cv2
import pyttsx3
from ultralytics import YOLO

# 初始化语音引擎
engine = pyttsx3.init()
model = YOLO(r"G:\ML\Garbage_YOLO\models\garbage_v2\weights\last.pt")
cap = cv2.VideoCapture(0)

# 为了防止语音一直重复吵死人，我们加个“冷却时间”
said_flag = False 

while cap.isOpened():
    ret, frame = cap.read()
    results = model(frame, conf=0.8)
    
    # 逻辑判断：如果结果里有东西
    if len(results[0].boxes) > 0:
        for box in results[0].boxes:
            cls_id = int(box.cls[0])
            label = model.names[cls_id]
            
            if label == "charging treasure" and not said_flag:
                print("警报：发现电池！")
                engine.say("Detected hazardous waste: Battery. Please dispose in the red bin.")
                engine.runAndWait()
                said_flag = True # 说过一次了，别再说了
    else:
        said_flag = False # 东西拿走了，重置状态

    cv2.imshow("Smart Logic", results[0].plot())
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()