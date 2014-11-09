#ifndef GUI_H
#define GUI_H

#include <QtWidgets/QMainWindow>

#include "board.h"
#include "boardModel.h"

namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
	Q_OBJECT

public:
    explicit Gui(QWidget *parent = 0);
	~Gui();


public slots:
	void SetDensity(int d) { density = d; }
	void SetSide(int s) { side = s; }
	void OnClick(QModelIndex index);
	void OnNewGame(void);
	void OnRetry(void);
	void OnSolve(void);
	void OnAi(void);
	void UpdateLabels(void);

private:
    Ui::Gui *ui;
	long density;
	long side;
    BoardModel *model;
};

#endif // GUI_H
