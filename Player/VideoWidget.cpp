#include "VideoWidget.h"
#include <QDebug>
#include <QTimer>
#include <iostream>

extern "C" {
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}

//�Զ���˫����
#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4

FILE* fp = NULL;

//����shader
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


//ƬԪshader
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


//׼������

VideoWidget::VideoWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    // ��ʼ��ʱ��������ڴ沢����ɫ������δ������Ƶʱ��ʾ�����ɫ��
    datas[0] = new unsigned char[width * height](); // () ��ʼ�������ֽ�Ϊ 0����ɫ�� Y ������
    datas[1] = new unsigned char[width * height / 4](); // U ����Ĭ�� 0����Ӧ YUV ��ɫ��
    datas[2] = new unsigned char[width * height / 4](); // V ����Ĭ�� 0����Ӧ YUV ��ɫ��
    // ���ÿؼ�����Ϊ��ɫ��˫�ر��ϣ�OpenGL δ��Ⱦʱ��ʾ�˱�����
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
    //��������ڴ�ռ�
    datas[0] = new unsigned char[width * height];		//Y
    datas[1] = new unsigned char[width * height / 4];	//U
    datas[2] = new unsigned char[width * height / 4];	//V

    // ����״̬������Ƶ��ʼ��ʱ������ʾ��ɫ
    hasVideoData = false;

    if (texs[0]) {
        glDeleteTextures(3, texs);
    }

    //��������
    glGenTextures(3, texs);

    //Y
    glBindTexture(GL_TEXTURE_2D, texs[0]);
    //�Ŵ���ˣ����Բ�ֵ   GL_NEAREST(Ч�ʸߣ�������������)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //���������Կ��ռ�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //U
    glBindTexture(GL_TEXTURE_2D, texs[1]);
    //�Ŵ���ˣ����Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //���������Կ��ռ�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //V
    glBindTexture(GL_TEXTURE_2D, texs[2]);
    //�Ŵ���ˣ����Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //���������Կ��ռ�
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

    // ���� U/V ���ݣ�UV ƽ���� Y ��һ�룩
    int uvHeight = height / 2;
    int uvWidth = width/2;
    for (int y = 0; y < uvHeight; y++) {
        memcpy(datas[1] + y * uvWidth, frame->data[1] + y * frame->linesize[1], uvWidth);
        memcpy(datas[2] + y * uvWidth, frame->data[2] + y * frame->linesize[2], uvWidth);
    }

    // �ؼ����״ν�����Ƶ֡ʱ�����Ϊ�����ݿ���Ⱦ
    hasVideoData = true;

    mux.unlock();
    av_frame_free(&frame);

    //дһ֡����ˢ��һ��
    update();

}

void VideoWidget::initializeGL()
{
    mux.lock();
    qDebug() << "initializeGL\n";

    //��ʼ��opengl ��QOpenGLFunctions�̳У�����
    initializeOpenGLFunctions();

    // �ؼ������� OpenGL �����ɫΪ��ɫ��RGBA��0,0,0,1��
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // ������������״γ�ʼ��ʱ��ʾ��ɫ��
    glClear(GL_COLOR_BUFFER_BIT);

    //program����shader�������ƬԪ���ű�
    //ƬԪ�����أ�
    qDebug() << program.addShaderFromSourceCode(QGLShader::Fragment, tString);
    //����shader
    qDebug() << program.addShaderFromSourceCode(QGLShader::Vertex, vString);

    //���ö�������ı���
    program.bindAttributeLocation("vertexIn", A_VER);

    //���ò�������
    program.bindAttributeLocation("textureIn", T_VER);

    //����shader
    qDebug() << "program.link() = " << program.link();

    qDebug() << "program.bind() = " << program.bind();

    //���ݶ���Ͳ�������
    //����
    static const GLfloat ver[] = {
        -1.0f,-1.0f,
        1.0f,-1.0f,
        -1.0f, 1.0f,
        1.0f,1.0f
    };

    //����
    static const GLfloat tex[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    //����
    glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
    glEnableVertexAttribArray(A_VER);

    //����
    glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
    glEnableVertexAttribArray(T_VER);


    //��shader��ȡ����
    unis[0] = program.uniformLocation("tex_y");
    unis[1] = program.uniformLocation("tex_u");
    unis[2] = program.uniformLocation("tex_v");

    mux.unlock();
    

    //������ʱ��
}

void VideoWidget::paintGL()
{

    mux.lock();

    // �ؼ���ÿ����Ⱦǰ�����������Ϊ��ɫ������������ʱ��ʾ��ɫ��
    glClear(GL_COLOR_BUFFER_BIT);

    // ��δ������Ƶ���ݣ�ֱ�ӷ��أ�����ʾ��ɫ������
    if (!hasVideoData) {
        mux.unlock();
        qDebug() << "paintGL: No video data, show black";
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs[0]); //0��󶨵�Y����
    //�޸Ĳ�������(�����ڴ�����)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
    //��shader uni��������
    glUniform1i(unis[0], 0);


    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, texs[1]); //1��󶨵�U����
    //�޸Ĳ�������(�����ڴ�����)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[1]);
    //��shader uni��������
    glUniform1i(unis[1], 1);


    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, texs[2]); //2��󶨵�V����
    //�޸Ĳ�������(�����ڴ�����)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[2]);
    //��shader uni��������
    glUniform1i(unis[2], 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    qDebug() << "paintGL";
    mux.unlock();

}

void VideoWidget::resizeGL(int w, int h)
{
    mux.lock();
    qDebug() << "resizeGL\n";
    //if (width == 0 || height == 0)
    //{
    //    mux.unlock();
    //    return; // ����������
    //}

    //// ������Ƶ��߱ȣ�ע�⸡����������
    //float videoAspect = static_cast<float>(width) / height;
    //std::cout << "��Ƶ��߱� �� " << videoAspect << endl;
    //// ���㴰�ڿ�߱�
    //float widgetAspect = static_cast<float>(w) / h;
    //std::cout << "���ڿ�߱�: " << widgetAspect << endl;

    //if (widgetAspect > videoAspect) {
    //    // ���ڱ���Ƶ�����ҼӺڱ� Pillarbox��
    //    int renderWidth = static_cast<int>(h * videoAspect);
    //    int xOffset = (w - renderWidth) / 2;
    //    glViewport(xOffset, 0, renderWidth, h);
    //}
    //else {
    //    // ���ڱ���Ƶ�ߣ����¼Ӻڱ� Letterbox��
    //    int renderHeight = static_cast<int>(w / videoAspect);
    //    int yOffset = (h - renderHeight) / 2;
    //    glViewport(0, yOffset, w, renderHeight);
    //}
    mux.unlock();
}

