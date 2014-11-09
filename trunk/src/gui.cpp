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

    QProgressBar *bar = model->GetProgressBar();
	statusBar()->addPermanentWidget(bar);
	bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //ui->tableView->verticalHeader()->setResizeMode(QHeaderView::Stretch);
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
}

void Gui::UpdateLabels(void)
{
    ui->actionsLabel->setText(QString("%1").arg(model->getClickCount()));
    ui->whiteLabel->setText(QString("%1").arg(model->getWhiteCount()));
    ui->blackLabel->setText(QString("%1").arg(model->getBlackCount()));
}

void Gui::OnNewGame(void)
{
    model->generate(side, density);
}

void Gui::OnRetry(void)
{
    model->retry();
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

    model->Ai();

    ui->buttonBox->setEnabled(true);
}
