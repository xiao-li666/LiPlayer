#pragma once

#include <QSlider>
#include <QMouseEvent>

class PlayPosSlider  : public QSlider
{
	Q_OBJECT

public:
	PlayPosSlider(QWidget *parent = nullptr);
	~PlayPosSlider();

	void mousePressEvent(QMouseEvent* e);
};
