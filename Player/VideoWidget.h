#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>
#include <VideoCall.h>
#include <QImage>
#include <QMouseEvent>
struct AVFrame;

class VideoWidget : public QOpenGLWidget, protected QOpenGLFunctions, public VideoCall
{
    Q_OBJECT
public:
    VideoWidget(QWidget* parent);
    ~VideoWidget();

    virtual void Init(int width, int height);

    virtual void Repaint(AVFrame* frame);
    virtual QImage saveScreenshot();
    virtual QImage yuvToQImage(unsigned char* yData, unsigned char* uData, unsigned char* vData, int width, int height);

protected:
    //ˢ����ʾ
    void paintGL();
    //��ʼ��gl
    void initializeGL();
    //���ڳߴ�仯
    void resizeGL(int width, int height);

    void mouseDoubleClickEvent(QMouseEvent* event) override;


private:
    std::mutex mux;
    //shader����
    QGLShaderProgram program;

    //shader�е�yuv������ַ
    GLuint unis[3] = { 0 };
    //opengl��texture��ַ
    GLuint texs[3] = { 0 };

    int width = 240;
    int height = 128;
    //�����ڴ�ռ�
    unsigned char* datas[3] = { 0 };

    bool hasVideoData = false; // ����������Ƿ�����Ƶ���ݿ���Ⱦ

    // ��Ƶ���ζ������꣨�洰�ڱ�����̬������
    GLfloat ver[8] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

};



