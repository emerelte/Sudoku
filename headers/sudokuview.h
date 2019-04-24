#ifndef SUDOKUVIEW_H
#define SUDOKUVIEW_H

#include "sudokumodel.h"
#include <QMainWindow>

namespace Ui {
class SudokuView;
}

class SudokuView : public QMainWindow
{
    Q_OBJECT
public:
    explicit SudokuView(QWidget *parent = nullptr);
    ~SudokuView();
    SudokuModel::diffLevel init();
    bool isWon();
    void refresh(SudokuModel::diffLevel dl);
    void showSolution();
    void refreshProgress();
private:
    int size=9;
    SudokuModel * sudoku;
    Ui::SudokuView *ui;
};

#endif // SUDOKUVIEW_H
