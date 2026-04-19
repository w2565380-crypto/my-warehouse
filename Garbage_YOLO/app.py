import gradio as gr
import cv2
from ultralytics import YOLO
import numpy as np

# 1. 加载你刚搬运过来的 v2 模型
model = YOLO(r"G:\ML\Garbage_YOLO\models\garbage_v2\weights\last.pt")

def predict_image(img):
    if img is None:
        return None
    
    # 2. 运行推理
    results = model(img, conf=0.25)
    
    # 3. 绘制检测框
    # results[0].plot() 返回的是 BGR 格式，Gradio 需要 RGB
    annotated_img = results[0].plot()
    return cv2.cvtColor(annotated_img, cv2.COLOR_BGR2RGB)

# 4. 构建 Gradio 界面
with gr.Blocks(title="AI 垃圾分类助手") as demo:
    gr.Markdown("# ♻️ 智能垃圾分类检测系统")
    gr.Markdown("上传一张照片，或者开启摄像头，AI 将自动识别其中的垃圾类别。")
    
    with gr.Tab("图片检测"):
        with gr.Row():
            image_input = gr.Image(type="numpy", label="输入图片")
            image_output = gr.Image(type="numpy", label="识别结果")
        image_button = gr.Button("开始识别")
    
    with gr.Tab("摄像头实时检测"):
        cam_input = gr.Image(sources=["webcam"], streaming=False, label="点击拍照")
        cam_output = gr.Image(type="numpy", label="结果")
        cam_button = gr.Button("拍照并识别")

    # 绑定逻辑
    image_button.click(predict_image, inputs=image_input, outputs=image_output)
    cam_button.click(predict_image, inputs=cam_input, outputs=cam_output)

# 5. 启动应用
if __name__ == "__main__":
    # share=True 会生成一个公网访问链接，有效期 72 小时
    demo.launch(share=False)
