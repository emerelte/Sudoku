#include "../headers/sudokuview.h"
#include "../headers/sudokumodel.h"
#include "../headers/difficultdialog.h"
#include <QApplication>
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[])
{
    srand(time(0));

    //SudokuModel sm(SudokuModel::easy,16);
    //SudokuModel sm1=SudokuModel(sm,0,0);
    //sm.cover();
    //cout << sm.hasOneSolution(9) << endl;
    //sm.show();
    //cout << sm.hasOneSolution(0) << endl;
    //cout << sm1.hasOneSolution() << endl;
    //cout << endl;
    //sm.show();

    QApplication a(argc, argv);
    SudokuView w;
    w.show();
    return a.exec();

}
