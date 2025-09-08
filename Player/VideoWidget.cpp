#include "VideoWidget.h"
#include <QDebug>
#include <QTimer>
#include <iostream>
#include<qdir.h>
#include <QDateTime>
#include <QMessageBox>

extern "C" {
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}

//自动加双引号
#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4

FILE* fp = NULL;

//顶点shader
const char* vString = GET_STR(
    attribute vec4 vertexIn;
    attribute vec2 textureIn;
    varying vec2 textureOut;
    void main(void)
    {
        gl_Position = vertexIn;
        textureOut = textureIn;
    }
);


//片元shader
const char* tString = GET_STR(
    varying vec2 textureOut;
    uniform sampler2D tex_y;
    uniform sampler2D tex_u;
    uniform sampler2D tex_v;
    void main(void)
    {
        vec3 yuv;
        vec3 rgb;
        yuv.x = texture2D(tex_y, textureOut).r;
        yuv.y = texture2D(tex_u, textureOut).r - 0.5;
        yuv.z = texture2D(tex_v, textureOut).r - 0.5;
        rgb = mat3(1.0, 1.0, 1.0,
            0.0, -0.39465, 2.03211,
            1.13983, -0.58060, 0.0) * yuv;
        gl_FragColor = vec4(rgb, 1.0);
    }

);


//准备数据

VideoWidget::VideoWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    // 初始化时分配材质内存并填充黑色（避免未加载视频时显示随机颜色）
    datas[0] = new unsigned char[width * height](); // () 初始化所有字节为 0（黑色的 Y 分量）
    datas[1] = new unsigned char[width * height / 4](); // U 分量默认 0（对应 YUV 黑色）
    datas[2] = new unsigned char[width * height / 4](); // V 分量默认 0（对应 YUV 黑色）
    // 设置控件背景为黑色（双重保障：OpenGL 未渲染时显示此背景）
    this->setStyleSheet("background-color: black;");
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::Init(int width, int height)
{
    mux.lock();
    this->width = width;
    this->height = height;
    std::cout << "Init start" << width << " " << height << std::endl;
    //this->resize(QSize(960, 544));
    delete datas[0];
    delete datas[1];
    delete datas[2];
    //分配材质内存空间
    datas[0] = new unsigned char[width * height];		//Y
    datas[1] = new unsigned char[width * height / 4];	//U
    datas[2] = new unsigned char[width * height / 4];	//V

    // 重置状态：新视频初始化时，先显示黑色
    hasVideoData = false;

    if (texs[0]) {
        glDeleteTextures(3, texs);
    }

    //创建材质
    glGenTextures(3, texs);

    //Y
    glBindTexture(GL_TEXTURE_2D, texs[0]);
    //放大过滤，线性插值   GL_NEAREST(效率高，但马赛克严重)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //创建材质显卡空间
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //U
    glBindTexture(GL_TEXTURE_2D, texs[1]);
    //放大过滤，线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //创建材质显卡空间
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //V
    glBindTexture(GL_TEXTURE_2D, texs[2]);
    //放大过滤，线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //创建材质显卡空间
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    mux.unlock();
}

void VideoWidget::Repaint(AVFrame* frame)
{
    if (!frame) {
        return;
    }
    if (!datas[0] || width * height == 0 || frame->width != width || frame->height != height) {
        av_frame_free(&frame);
        return;
    }
    mux.lock();
    /*memcpy(datas[0], frame->data[0], width * height);
    memcpy(datas[1], frame->data[1], width * height / 4);
    memcpy(datas[2], frame->data[2], width * height / 4);*/
    for (int y = 0; y < height; y++) {
        memcpy(datas[0] + y * width, frame->data[0] + y * frame->linesize[0], width);
    }

    // 拷贝 U/V 数据（UV 平面是 Y 的一半）
    int uvHeight = height / 2;
    int uvWidth = width/2;
    for (int y = 0; y < uvHeight; y++) {
        memcpy(datas[1] + y * uvWidth, frame->data[1] + y * frame->linesize[1], uvWidth);
        memcpy(datas[2] + y * uvWidth, frame->data[2] + y * frame->linesize[2], uvWidth);
    }

    // 关键：首次接收视频帧时，标记为有数据可渲染
    hasVideoData = true;

    mux.unlock();
    av_frame_free(&frame);

    //写一帧数据刷新一次
    update();

}

QImage VideoWidget::saveScreenshot()
{
    mux.lock();
    // 检查是否有有效视频数据
    if (!hasVideoData || !datas[0] || width <= 0 || height <= 0) {
        mux.unlock();
        return QImage(); // 返回空图片表示失败
    }

    // 从YUV缓冲区转换为QImage（确保与渲染画面一致）
    QImage image = yuvToQImage(datas[0], datas[1], datas[2], width, height);

    mux.unlock();
    return image;
}

QImage VideoWidget::yuvToQImage(unsigned char* yData, unsigned char* uData, unsigned char* vData, int width, int height)
{
    QImage image(width, height, QImage::Format_RGB32);
    int uvWidth = width / 2;
    int uvHeight = height / 2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // 1. 读取YUV分量（注意：原始数据通常是0-255的字节，需转为-128~127的偏移值）
            int Y = yData[y * width + x];
            int U = (int)uData[(y / 2) * uvWidth + (x / 2)] - 128;  // 减去128得到偏移量
            int V = (int)vData[(y / 2) * uvWidth + (x / 2)] - 128;

            // 2. 使用与shader完全一致的转换公式（关键！）
            int r = qBound(0, Y + (int)(1.13983 * V), 255);
            int g = qBound(0, Y + (int)(-0.39465 * U) + (int)(-0.58060 * V), 255);
            int b = qBound(0, Y + (int)(2.03211 * U), 255);

            image.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return image;
}

void VideoWidget::initializeGL()
{
    mux.lock();
    qDebug() << "initializeGL\n";

    //初始化opengl （QOpenGLFunctions继承）函数
    initializeOpenGLFunctions();

    // 关键：设置 OpenGL 清除颜色为黑色（RGBA：0,0,0,1）
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // 清除缓冲区（首次初始化时显示黑色）
    glClear(GL_COLOR_BUFFER_BIT);

    //program加载shader（顶点和片元）脚本
    //片元（像素）
    qDebug() << program.addShaderFromSourceCode(QGLShader::Fragment, tString);
    //顶点shader
    qDebug() << program.addShaderFromSourceCode(QGLShader::Vertex, vString);

    //设置顶点坐标的变量
    program.bindAttributeLocation("vertexIn", A_VER);

    //设置材质坐标
    program.bindAttributeLocation("textureIn", T_VER);

    //编译shader
    qDebug() << "program.link() = " << program.link();

    qDebug() << "program.bind() = " << program.bind();

    //传递顶点和材质坐标
    //顶点
    /*static const GLfloat ver[] = {
        -1.0f,-1.0f,
        1.0f,-1.0f,
        -1.0f, 1.0f,
        1.0f,1.0f
    };*/

    //材质
    static const GLfloat tex[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    //顶点
    glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
    glEnableVertexAttribArray(A_VER);

    //材质
    glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
    glEnableVertexAttribArray(T_VER);


    //从shader获取材质
    unis[0] = program.uniformLocation("tex_y");
    unis[1] = program.uniformLocation("tex_u");
    unis[2] = program.uniformLocation("tex_v");

    mux.unlock();
    

    //启动定时器
}

void VideoWidget::paintGL()
{

    mux.lock();

    // 关键：每次渲染前先清除缓冲区为黑色（保障无数据时显示黑色）
    glClear(GL_COLOR_BUFFER_BIT);

    // 若未加载视频数据，直接返回（仅显示黑色背景）
    if (!hasVideoData) {
        mux.unlock();
        qDebug() << "paintGL: No video data, show black";
        return;
    }

    glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
    glEnableVertexAttribArray(A_VER);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs[0]); //0层绑定到Y材质
    //修改材质内容(复制内存内容)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
    //与shader uni遍历关联
    glUniform1i(unis[0], 0);


    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, texs[1]); //1层绑定到U材质
    //修改材质内容(复制内存内容)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[1]);
    //与shader uni遍历关联
    glUniform1i(unis[1], 1);


    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, texs[2]); //2层绑定到V材质
    //修改材质内容(复制内存内容)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[2]);
    //与shader uni遍历关联
    glUniform1i(unis[2], 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    qDebug() << "paintGL";
    mux.unlock();

}

void VideoWidget::resizeGL(int w, int h)
{
    mux.lock();
    if (width == 0 || height == 0) {
        mux.unlock();
        return;
    }

    float video_aspect = (float)width / height;
    float window_aspect = (float)w / h;

    float scale_x = 1.0f;
    float scale_y = 1.0f;

    if (window_aspect > video_aspect) {
        scale_x = video_aspect / window_aspect;
    }
    else {
        scale_y = window_aspect / video_aspect;
    }

    ver[0] = -scale_x; ver[1] = -scale_y;
    ver[2] = scale_x; ver[3] = -scale_y;
    ver[4] = -scale_x; ver[5] = scale_y;
    ver[6] = scale_x; ver[7] = scale_y;

    glViewport(0, 0, w, h);

    mux.unlock();
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QImage frame = saveScreenshot();
        if (frame.isNull()) {
            qDebug() << "No video frame to capture";
            return;
        }

        // 创建保存目录
        QString dirPath = QDir::currentPath() + "/screenshots";
        QDir dir(dirPath);
        if (!dir.exists()) {
            dir.mkpath("."); // 递归创建目录
        }

        // 生成唯一文件名（时间戳+分辨率）
        QString fileName = QString("%1/screenshot_%2_%3x%4.png")
            .arg(dirPath)
            .arg(QDateTime::currentMSecsSinceEpoch())
            .arg(width)
            .arg(height);

        // 保存图片
        if (frame.save(fileName)) {
            qDebug() << "Video frame saved to:" << fileName;
            // 可选：显示提示框
           // QMessageBox::information(this, "截图成功", QString("已保存至：%1").arg(fileName));
        }
        else {
            qDebug() << "Failed to save screenshot";
        }
    }
    QOpenGLWidget::mouseDoubleClickEvent(event);

}