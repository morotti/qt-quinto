#ifndef BoardModel_H
#define BoardModel_H

#include "precompiled.h"
#include "board.h"
#include "solver.h"

class BoardModel: public QAbstractTableModel
{
	Q_OBJECT

public:
    Board board;
    Solver solver;

private:
	struct {long x; long y; } lastPoint;

    QVariant blueBrush, whiteBrush, blackBrush;

public:
    BoardModel(QObject *parent);

    void click(long x, long y);
    void generate(long nSide, long nDensity);
    void retry(void);
    long solve(void);

    // inherited functions
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;

};

#endif // BoardModel_H
