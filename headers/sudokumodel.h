#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;

class SudokuNumber;
//Part - rząd, kolumna albo kwadrat
//przechowuje niewykorzystane liczby
//numbers - wykorzystane numery
class Part : public vector<int>{
private:
    int size;
    vector<SudokuNumber *> numbers;
public:
    Part(int m_size){size=m_size;refresh();}
    void refresh(){clear(); for (int i=1;i<size+1;++i) push_back(i);}
    void addNumber(SudokuNumber * nr){numbers.push_back(nr);}
    vector<int> getRemained()const{return *this;}
    vector<SudokuNumber *> getNumbers()const{return numbers;}
};

class SudokuNumber{
private:
    int size;//maksymalna wartość
    int value;
    Part * row;
    Part * column;
    Part * square;
    bool visible;
public:
    SudokuNumber(int m_size, Part * m_row, Part * m_column, Part * m_square);
    SudokuNumber(int m_value,int m_size, Part * m_row, Part * m_column, Part * m_square);
    vector<int> getPossible() const;//zwraca wszystkie aktualnie możliwe liczby
    bool isPossible(int m_value) const;
    bool setValue(int m_value);
    bool isVisible() const{return visible;}
    void cover(){visible=false;}
    explicit operator int(){return value;}
    friend ostream & operator<<(ostream & os, SudokuNumber sn){
        os << sn.value;
        return os;
    }
};

class SudokuModel{
public:
    enum diffLevel{easy,medium,hard};
private:
    int size=9;
    int difficulty;
    vector<Part *> rows;
    vector<Part *> columns;
    vector<Part *> squares;
    vector<SudokuNumber *> numbers;
    bool hasOneSolution();//pomocnicza do hasOneSolution(int)
    void setDifficulty(diffLevel dl){
        switch (dl) {
            case easy:
                difficulty=size*size/4;
                break;
            case medium:
                difficulty=size*size/2;
                break;
            case hard:
                difficulty=size*size;
        }
    }
public:
    SudokuModel(diffLevel m_dl=easy,int m_size=9);//tworzy całe sudoku
    SudokuModel(const SudokuModel & sm, int pos, int val);//kopiuje sudoku, zmieniając 1 numer
    SudokuModel(vector<int> m_numbers);//tworzy sudoku z wektora
    ~SudokuModel();
    vector<SudokuNumber *> getNumbers(){
        return numbers;
    }
    bool hasOneSolution(int pos);//czy gdy numer pod pos stanie się niewidoczny, sudoku będzie miało tylko 1 rozwiązanie
    void cover();//zasłania losowe numerki (ile zasłoni, zależy od difficulty)
    void show();
};

#endif // SUDOKUMODEL_H
