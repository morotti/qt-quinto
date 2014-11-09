#include "precompiled.h"
#include "BoardModel.h"

BoardModel::BoardModel(QObject *parent) : QAbstractTableModel(parent)
{
    blueBrush = QVariant(QColor(Qt::darkCyan));
    whiteBrush = QVariant(QColor(Qt::white));
    blackBrush = QVariant(QColor(Qt::black));
}

int BoardModel::rowCount(const QModelIndex &) const
{
    return board.getSide();
}

int BoardModel::columnCount(const QModelIndex &) const
{
    return board.getSide();
}

void BoardModel::generate(long nSide, long nDensity)
{
    beginResetModel();

	lastPoint.x = lastPoint.y = -1;
    board.generate(nSide, nDensity);
    solver.reset();

    endResetModel();
}

long BoardModel::solve(void)
{
    beginResetModel();

    long nSolution = solver.solve(board.getBoard());

    endResetModel();

    return nSolution;
}

void BoardModel::retry(void)
{
    beginResetModel();

	lastPoint.x = lastPoint.y = -1;
    board.retry();
    solver.reset();

    endResetModel();
}

void BoardModel::click(long x, long y)
{
    board.click(x, y);
    if(solver.isSolved())
        solver.flip(x, y);

	QPoint p(lastPoint.x, lastPoint.y);
	lastPoint.x = x;
	lastPoint.y = y;
	
	dataChanged(createIndex(y, x), createIndex(y, x));

	if(y > 0)
		dataChanged(createIndex(y - 1, x), createIndex(y - 1, x));
	
    if(y < board.getSide() - 1)
		dataChanged(createIndex(y + 1, x), createIndex(y + 1, x));

	if(x > 0)
		dataChanged(createIndex(y, x - 1), createIndex(y, x - 1));

    if(x < board.getSide() - 1)
		dataChanged(createIndex(y, x + 1), createIndex(y, x + 1));

	dataChanged(createIndex(p.y(), p.x()), createIndex(p.y(), p.x()));
}

QVariant BoardModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
        if(solver.isSolved() == false)
            return QVariant();
        else
            return solver.get(index.column(), index.row()) ? QString("o") : QVariant();
	}
	else if (role == Qt::BackgroundRole)
	{
		//QBrush black(Qt::black);
		//QBrush white(Qt::white);

		if(index.column() == lastPoint.x && index.row() == lastPoint.y)
			return blueBrush;

        if(board.get(index.column(), index.row()))
			return blackBrush;
		else
			return whiteBrush;
	}
	else if (role == Qt::ForegroundRole)
	{
		return QColor(Qt::red);
	}
	else if (role == Qt::TextAlignmentRole)
	{
		return Qt::AlignCenter;
	}
	//else if (role == Qt::FontRole)
	//{
	//	return QFont("Verdana", 8, 600);
	//}

	return QVariant();
}
