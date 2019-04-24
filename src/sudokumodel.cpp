#include "../headers/sudokumodel.h"
#include <cmath>

SudokuNumber::SudokuNumber(int m_size, Part * m_row, Part * m_column, Part * m_square) : visible(true){
    size=m_size;
    value=0;
    row = m_row;
    row->addNumber(this);
    column = m_column;
    column->addNumber(this);
    square = m_square;
    square->addNumber(this);
}
SudokuNumber::SudokuNumber(int m_value,int m_size, Part * m_row, Part * m_column, Part * m_square):SudokuNumber(m_size,m_row,m_column,m_square){
        setValue(m_value);
}
vector<int> SudokuNumber::getPossible()const{
        vector<int> all;
        for (int m_value=1;m_value<=size;m_value++){
            if (((find(row->begin(),row->end(),m_value))!=row->end()) &&
                ((find(column->begin(),column->end(),m_value))!=column->end()) &&
                ((find(square->begin(),square->end(),m_value))!=square->end()))
                all.push_back(m_value);
        }
        return all;
}
bool SudokuNumber::isPossible(int m_value)const{
    if (((find(row->begin(),row->end(),m_value))==row->end()) ||
        ((find(column->begin(),column->end(),m_value))==column->end()) ||
        ((find(square->begin(),square->end(),m_value))==square->end()))
            return false;
    return true;
}
bool SudokuNumber::setValue(int m_value){
        if (m_value==0)
            value=m_value;
        if (!isPossible(m_value))
            return false;
        value = m_value;
        row->erase(find(row->begin(),row->end(),value));
        column->erase(find(column->begin(),column->end(),value));
        square->erase(find(square->begin(),square->end(),value));
        return true;
}

SudokuModel::SudokuModel(diffLevel dl,int m_size){
    size=m_size;
    setDifficulty(dl);
    for (int i=0;i<size;++i){
        rows.push_back(new Part(size));
        columns.push_back(new Part(size));
        squares.push_back(new Part(size));
    }
    for (int i=0;i<size*size;++i){
        numbers.push_back(new SudokuNumber(size,rows[i/size],columns[i%size],squares[int(i%size)/sqrt(size)+(int(i/(size*sqrt(size)))*sqrt(size))]));
    }
    //ustawia środkowe kwadraty i zapisuje wylosowane liczby w wektorze middle
    vector<int> middle;
    for (int i=0;i<sqrt(size);++i){
        for (int j=0;j<sqrt(size);++j){
            for (int k=0;k<sqrt(size);++k){
                while(!numbers[k+size*j+(size*sqrt(size)+sqrt(size))*i]->setValue(rand()%size+1)){
                    continue;
                }
                middle.push_back((int)(*numbers[k+size*j+(size*sqrt(size)+sqrt(size))*i]));
             }
        }
    }
    //dopasowuje resztę sukoku do wylosowanych liczb
    while(1){
         int i;
         for (i=0;i<size*size;++i){
              if (int(*numbers[i])==0){
                        break;
                    }
         }
         if (i==size*size)
              break;//gdy wszystkie dopasuje, wychodzi z nieskończonej pętli
         //gdy nie dopasował wszystki, czyści wszystkie SudokuNumbers oprócz diagonalnych kwadratów
         for (int i=0;i<size*size;++i){
              numbers[i]->setValue(0);
         }
         for (int i=0;i<size;++i){
              rows[i]->refresh();
              columns[i]->refresh();
              squares[i]->refresh();
         }
         int m=0;
         for (int i=0;i<sqrt(size);++i){
              for (int j=0;j<sqrt(size);++j){
                    for (int k=0;k<sqrt(size);++k){
                        numbers[k+size*j+(size*sqrt(size)+sqrt(size))*i]->setValue(middle[m]);
                        m++;
                    }
              }
         }
         //próbuje dopasować resztę
         for (int i=0;i<size*size;++i){
              int br=0;
              if ((int)(*numbers[i])==0){
                   while(!numbers[i]->setValue(rand()%size+1)){
                       br++;
                       if (br==200){
                            br=0;
                            break;
                       }
                   }
              }
         }
    }
}
SudokuModel::SudokuModel(const SudokuModel & sm, int pos, int val){
        size=sm.size;
        for (int i=0;i<size;++i){
            rows.push_back(new Part(size));
            columns.push_back(new Part(size));
            squares.push_back(new Part(size));
        }
        for (int i=0;i<size*size;++i){
            if (i==pos)
                numbers.push_back(new SudokuNumber(val,size,rows[i/size],columns[i%size],squares[int(i%size)/sqrt(size)+(int(i/(size*sqrt(size)))*sqrt(size))]));
            else
                numbers.push_back(new SudokuNumber(((int)*(sm.numbers[i])),size,rows[i/size],columns[i%size],squares[int(i%size)/sqrt(size)+(int(i/(size*sqrt(size)))*sqrt(size))]));
        }
}
SudokuModel::SudokuModel(vector<int> m_numbers){
            size=sqrt(m_numbers.size());
            for (int i=0;i<size;++i){
                rows.push_back(new Part(size));
                columns.push_back(new Part(size));
                squares.push_back(new Part(size));
            }
            for (int i=0;i<size*size;++i){
                numbers.push_back(new SudokuNumber(m_numbers[i],size,rows[i/size],columns[i%size],squares[int(i%size)/sqrt(size)+(int(i/(size*sqrt(size)))*sqrt(size))]));
            }
}
SudokuModel::~SudokuModel(){
    for (auto it=numbers.begin();it!=numbers.end();++it)
        delete *it;
    for (auto it=rows.begin();it!=rows.end();++it)
        delete *it;
    for (auto it=columns.begin();it!=columns.end();++it)
        delete *it;
    for (auto it=squares.begin();it!=squares.end();++it)
        delete *it;
}
bool SudokuModel::hasOneSolution(){
        static int count=0;
        if (count > 1)
            return false;
        bool sol=true;
        for (int i=0;i<size*size;++i){
            if (int(*numbers[i])==0){
                const vector<int> temVec = numbers[i]->getPossible();
                for (int j=0;j<temVec.size();++j){
                    SudokuModel temp=SudokuModel(*this,i,temVec[j]);
                    temp.hasOneSolution();
                }
                sol=false;
            }
        }
        if (sol==true)
            count++;
        if (count==1)
            return true;
        return false;
    }
bool SudokuModel::hasOneSolution(int pos){
    vector<int> visible;
    for (int i=0;i<size*size;++i){
        if (i==pos)
            visible.push_back(0);
        else{
            if (numbers[i]->isVisible())
                visible.push_back((int)(*numbers[i]));
            else
                visible.push_back(0);
        }
    }
    SudokuModel temp = SudokuModel(visible);
    return temp.hasOneSolution();
}
void SudokuModel::cover(){
    for (int i=0;i<difficulty;++i){
        int rnd = rand()%(size*size);
        int br=0;
        while (!hasOneSolution(rnd)){
            rnd=rand()%(size*size);
            br++;
            if (br==5)
                break;
        }
        numbers[rnd]->cover();
    }
}
void SudokuModel::show(){
    int i=0;
    for (auto it=numbers.begin();it!=numbers.end();++it){
        if (i==size){
            cout << endl;
            i=0;
        }
        ++i;
        cout << *(*it) << " ";
    }
    cout << endl;
}
