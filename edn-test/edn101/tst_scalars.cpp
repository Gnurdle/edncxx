#include <QtTest>

// add necessary includes here

class scalars : public QObject
{
    Q_OBJECT

public:
    scalars();
    ~scalars();

private slots:
    void test_case1();

};

scalars::scalars()
{

}

scalars::~scalars()
{

}

void scalars::test_case1()
{

}

QTEST_APPLESS_MAIN(scalars)

#include "tst_scalars.moc"
