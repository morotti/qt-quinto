#include "precompiled.h"
#include "gui.h"
#include "ui_gui.h"

Gui::Gui(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Gui)
{
	density = 0;
	side = 2;

    model = new BoardModel(parent);
    model->generate(side, density);

    ui->setupUi(this);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);

    connect(model, SIGNAL(UpdateCounters()), this, SLOT(UpdateLabels()));
}

Gui::~Gui()
{
    delete ui;
    delete model;
}

void Gui::OnClick(QModelIndex index)
{
    model->click(index.column(), index.row());
    UpdateLabels();

    if(model->board.isWon())
        QMessageBox::information(this, "Victory", "You win !");
}

void Gui::UpdateLabels(void)
{
    ui->actionsLabel->setText(QString("%1").arg(model->board.getClickCount()));
    ui->whiteLabel->setText(QString("%1").arg(model->board.getWhiteCount()));
    ui->blackLabel->setText(QString("%1").arg(model->board.getBlackCount()));
}

void Gui::OnNewGame(void)
{
    model->generate(side, density);
    UpdateLabels();
}

void Gui::OnRetry(void)
{
    model->retry();
    UpdateLabels();
}

void Gui::OnSolve(void)
{
    ui->buttonBox->setEnabled(false);
	
	QElapsedTimer timer;
	timer.start();

	long nSolution;
    nSolution = model->solve();
	
	double time = static_cast<long>(timer.elapsed()) / 1000.0;

	if(nSolution == -1)
		statusBar()->showMessage(QString("No solution found in %1 ms").arg(time));
	else if(nSolution < 17)
		statusBar()->showMessage(QString("%1 solutions found in %2 ms").arg(pow(2.0f, nSolution)).arg(time));
	else
		statusBar()->showMessage(QString("2^%1 solutions found in %2 ms").arg(nSolution).arg(time));
	
    ui->buttonBox->setEnabled(true);
}

void Gui::OnAi(void)
{
    ui->buttonBox->setEnabled(false);

    if(model->solver.isSolved() == false)
        this->OnSolve();

    if(model->solver.isSolved() == false)
        return;

    for( long y = 0 ; y < model->rowCount() ; y++)
    {
        for( long x = 0 ; x < model->columnCount() ; x++)
        {
            if(model->solver.get(x, y))
            {
                OnClick(model->index(y, x));
                qApp->processEvents();
                //yieldCurrentThread();
                //msleep(10);
            }
        }
    }

    ui->buttonBox->setEnabled(true);
}
