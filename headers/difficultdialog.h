#ifndef DIFFICULTDIALOG_H
#define DIFFICULTDIALOG_H

#include "sudokumodel.h"
#include <QDialog>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>

class DifficultDialog : public QDialog
{
public:
    DifficultDialog(const QStringList& items)
    {
        setLayout(new QHBoxLayout());

        box = new QComboBox;
        box->addItems(items);
        layout()->addWidget(box);
/*
        size = new QComboBox;
        size->addItems({"2","4","9"});
        layout()->addWidget(size);
*/
        QPushButton* ok = new QPushButton("ok");
        layout()->addWidget(ok);
        connect(ok, &QPushButton::clicked, this, [this]()
        {
           accept();
        });
    }

    QComboBox* comboBox() { return box; }

private:
    QComboBox* box;
    QComboBox* size;
};

#endif // DIFFICULTDIALOG_H
