#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>
#include <VideoCall.h>
struct AVFrame;

class VideoWidget : public QOpenGLWidget, protected QOpenGLFunctions, public VideoCall
{
    Q_OBJECT
public:
    VideoWidget(QWidget* parent);
    ~VideoWidget();

    virtual void Init(int width, int height);

    virtual void Repaint(AVFrame* frame);

protected:
    //刷新显示
    void paintGL();
    //初始化gl
    void initializeGL();
    //窗口尺寸变化
    void resizeGL(int width, int height);

private:
    std::mutex mux;
    //shader程序
    QGLShaderProgram program;

    //shader中的yuv变量地址
    GLuint unis[3] = { 0 };
    //opengl的texture地址
    GLuint texs[3] = { 0 };

    int width = 240;
    int height = 128;
    //材质内存空间
    unsigned char* datas[3] = { 0 };

    bool hasVideoData = false; // 新增：标记是否有视频数据可渲染

};



