#include "PlayPosSlider.h"

PlayPosSlider::PlayPosSlider(QWidget *parent)
	: QSlider(parent)
{}

PlayPosSlider::~PlayPosSlider()
{}

void PlayPosSlider::mousePressEvent(QMouseEvent * e)
{
	double pos = (double)e->pos().x() / this->width();
	this->setValue(pos * this->maximum());
	QSlider::mousePressEvent(e);
}
