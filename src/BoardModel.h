#ifndef BoardModel_H
#define BoardModel_H

#include "precompiled.h"
#include "board.h"

class BoardModel : public QAbstractTableModel
{
	Q_OBJECT

private:
    Board board;
	struct {long x; long y; } lastPoint;
	QVariant blueBrush, whiteBrush, blackBrush;
	QProgressBar *bar;

signals:
	void UpdateCounters(void);

public:
	BoardModel(QObject *parent);
	QProgressBar *GetProgressBar(void) { return bar; };

	virtual void click(long x, long y);
	virtual void generate(long nSide, long nDensity);
    virtual void retry(void);
	virtual long solve(void);
	void Ai(void);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	
	void run(void);
};

#endif // BoardModel_H
