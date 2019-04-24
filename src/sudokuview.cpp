#include "../headers/sudokuview.h"
#include "../headers/ui_sudokuview.h"
#include "../headers/difficultdialog.h"
#include <vector>
#include <QTextEdit>
#include <QList>
#include <QDebug>
#include <QColor>
#include <QPushButton>
#include <QMessageBox>
#include <QRegExp>
#include <QComboBox>

using namespace std;

SudokuView::SudokuView(QWidget *parent) : QMainWindow(parent), ui(new Ui::SudokuView){
    sudoku = nullptr;
    SudokuModel::diffLevel dl = init();
    refresh(dl);
}
SudokuModel::diffLevel SudokuView::init(){
    DifficultDialog dd({"easy","medium","hard"});
    QString difficultyStr;
    if (dd.exec() == QDialog::Accepted)
    {
        difficultyStr = dd.comboBox()->currentText();
    }
    if (difficultyStr=="easy"){
        return SudokuModel::easy;
    }
    else if (difficultyStr=="medium"){
        return SudokuModel::medium;
    }
    else
        return SudokuModel::hard;
}
bool SudokuView::isWon(){
    for (int i=0;i<size*size;++i){
        vector<SudokuNumber *> answer = sudoku->getNumbers();
        QString str = "number" + QString::number(i);
        QList<QTextEdit *> number = ui->centralWidget->findChildren<QTextEdit *>(str);
        if (number[0]->toPlainText().toInt() != (int)(*answer[i])){
            return false;
        }
    }
    return true;
}
void SudokuView::refresh(SudokuModel::diffLevel dl){
    delete sudoku;
    sudoku = new SudokuModel(dl);
    QString dlString="";
    switch ((int)dl) {
        case SudokuModel::easy:
            dlString="Easy";
            break;
        case SudokuModel::medium:
            dlString="Medium";
            break;
        case SudokuModel::hard:
            dlString="Hard";
            break;
    }
    sudoku->cover();
    vector<SudokuNumber *> answer = sudoku->getNumbers();
    ui->setupUi(this);
    ui->levelLabel->setText(dlString);
    for (int i=0;i<size*size;++i){
        QString str = "number" + QString::number(i);
        QList<QTextEdit *> number = ui->centralWidget->findChildren<QTextEdit *>(str);
        if (answer[i]->isVisible()){
            number[0]->setText(QString::number((int)(*answer[i])));
            number[0]->setReadOnly(1);
            QPalette p = number[0]->palette();
            p.setColor(QPalette::Active, QPalette::Base, Qt::gray);
            number[0]->setPalette(p);
        }
    }
    refreshProgress();
    sudoku->show();
    QList<QTextEdit *> numbers = ui->centralWidget->findChildren<QTextEdit *>(QRegExp("number*"));
    for (int i=0;i<numbers.size();++i){
        connect(numbers[i],&QTextEdit::textChanged,
                [this]()->void{
                    this->refreshProgress();
                });
    }
    connect(ui->check,&QPushButton::clicked,
            [this]()->void{
                if(isWon()){
                    int answer = QMessageBox::question(this, tr("Wygrałeś!"),
                            tr("Chcesz zagrać jeszcze raz?"),QMessageBox::Yes | QMessageBox::No);
                    if (answer == QMessageBox::No)
                        close();
                    if (answer == QMessageBox::Yes){
                        SudokuModel::diffLevel dl = this->init();
                        this->refresh(dl);
                    }
                }
                else{
                    QMessageBox::question(this, tr("Niestety, źle!"),
                                                tr("Kontynuuj dalej!"),QMessageBox::Ok);
                }
            });
    connect(ui->solution,&QPushButton::clicked,
            [this]()->void{
                this->showSolution();
            });
}
SudokuView::~SudokuView()
{
    delete sudoku;
    delete ui;
}
void SudokuView::refreshProgress(){
    QList<QTextEdit *> numbers = ui->centralWidget->findChildren<QTextEdit *>(QRegExp("number*"));
    int l=0;int m=0;
    vector<SudokuNumber *> all = sudoku->getNumbers();
    for (auto it=all.begin();it!=all.end();++it){
        if ((*it)->isVisible()==0)
            m++;
    }
    for (int i=0;i<numbers.size();++i){
        if (numbers[i]->toPlainText()==""){
            l++;
        }
    }
    l=m-l;
    ui->progressBar->setValue(l*100/m);
}
void SudokuView::showSolution(){
    vector<SudokuNumber *> correctAnswer = sudoku->getNumbers();
    vector<int> currentAnswer;
    for (int i=0;i<size*size;++i){
        vector<SudokuNumber *> answer = sudoku->getNumbers();
        QString str = "number" + QString::number(i);
        QList<QTextEdit *> number = ui->centralWidget->findChildren<QTextEdit *>(str);
        currentAnswer.push_back(number[0]->toPlainText().toInt());
    }
    ui->setupUi(this);
    for (int i=0;i<size*size;++i){
        QString str = "number" + QString::number(i);
        QList<QTextEdit *> number = ui->centralWidget->findChildren<QTextEdit *>(str);
        if (correctAnswer[i]->isVisible()){
            number[0]->setText(QString::number((int)(*correctAnswer[i])));
            number[0]->setReadOnly(1);
            QPalette p = number[0]->palette();
            p.setColor(QPalette::Active, QPalette::Base, Qt::gray);
            number[0]->setPalette(p);
        }
        else{
            QPalette g = number[0]->palette();
            if (currentAnswer[i]==(int)(*correctAnswer[i])){
                g.setColor(QPalette::Active, QPalette::Base, Qt::green);
            }
            else if (currentAnswer[i]!=0){
                g.setColor(QPalette::Active, QPalette::Base, Qt::red);
            }
            number[0]->setText(QString::number((int)(*correctAnswer[i])));
            number[0]->setReadOnly(1);
            number[0]->setPalette(g);
        }
        ui->progressBar->hide();
        ui->progressLabel->hide();
    }
    ui->check->setText("Kolejne Sudoku");//nazwy trochę zmieniają znaczenie
    ui->solution->setText("Koniec");
    connect(ui->check,&QPushButton::clicked,
            [this]()->void{
                SudokuModel::diffLevel dl = this->init();
                this->refresh(dl);
            });
    connect(ui->solution,&QPushButton::clicked,
            [this]()->void{
                this->close();
            });
}
