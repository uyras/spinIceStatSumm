#include <QCoreApplication>
#include <QDebug>
#include <QHash>
#include <QString>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include "config.h"
#include "PartArray.h"

using namespace std;

#define Kb 1.3806488e-16

void saveHash( QHash<double,int> *statSumm, double mult ){
    cout<<"saving hash"<<endl;
    QHashIterator<double,int> iter(*statSumm);
    ofstream f("energies.dat");
    while (iter.hasNext()){
        iter.next();
        f<<iter.value()<<"\t"<<iter.key()*mult<<endl;
    }
    f.close();
}

void saveMathHash( QHash<double,int> *statSumm, double mult ){
    cout<<"saving hash"<<endl;
    QHashIterator<double,int> iter(*statSumm);
    ofstream f("energiesMath.dat");

    f<<"s=";
    QString n;
    while (iter.hasNext()){
        iter.next();
        n = QString::number(iter.key()*mult/Kb);
        n.replace('e',"*10^");
        f<<iter.value()<<"*Exp["<<n.toStdString()<<"*t]+";
    }
    f<<"0;";
    f.close();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    config::Instance()->set2D();
    config::Instance()->randmode_standart();
    config::Instance()->srand(time(NULL));

    config::Instance()->ergGauss = 3e7 * 927.400915e-23; //3*10^-7 - намагниченность одной частицы (магн. Бора), 9274e-24 - эрг/Гс в обном Боре
    config::Instance()->santiMeter = 1E-7; //перевод нанометров в сантиметры
    int l = 800+81;
    //PartArray *sys= new PartArray(l,l,1);
    PartArray *sys= new PartArray(l,l,1);
    sys->dropSpinIce(80,220,400);
    sys->save("system.dat");
    cout<<"n="<<sys->count()<<endl;
    double eInit = sys->calcEnergy1FastIncrementalFirst();
    double e;
    QHash<double,int> statSumm;
    int i=0;
    double multiplier = pow(config::Instance()->ergGauss,2.)/pow(config::Instance()->santiMeter,3.); //множитель для вычисления энергии в ЭРГах
    do {
        e = sys->calcEnergy1FastIncremental(eInit);
        if (statSumm.contains(e))
            statSumm[e] += 1;
        else
            statSumm[e] = 1;

        i++;
        if (i>10000){
            saveMathHash(&statSumm,multiplier);
            i=0;
        }
    } while (sys->state->next());
    saveMathHash(&statSumm,multiplier);

    cout<<"finish"<<endl;
    return a.exec();
}
