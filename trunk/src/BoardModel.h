#ifndef BoardModel_H
#define BoardModel_H

#include "precompiled.h"
#include "board.h"
#include "solver.h"

class BoardModel: public QAbstractTableModel
{
	Q_OBJECT

private:
    Board board;
    Solver solver;

	struct {long x; long y; } lastPoint;

	QVariant blueBrush, whiteBrush, blackBrush;
	QProgressBar *bar;

signals:
	void UpdateCounters(void);

public:
    BoardModel(QObject *parent);
	QProgressBar *GetProgressBar(void) { return bar; };

    void click(long x, long y);
    void generate(long nSide, long nDensity);
    void retry(void);
    long solve(void);
	void Ai(void);

    // inherited functions
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;

};

#endif // BoardModel_H
