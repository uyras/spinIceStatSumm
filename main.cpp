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

void saveMathHash( QHash<double,int> *statSumm, double mult, QString fname ){
    cout<<"saving hash"<<endl;
    QHashIterator<double,int> iter(*statSumm);
    ofstream f(fname.toStdString().c_str());

    f<<"t=.;"<<endl;
    f<<"s=";
    QString n;
    while (iter.hasNext()){
        iter.next();
        n = QString::number(iter.key()*mult/Kb);
        n.replace('e',"*10^");
        f<<iter.value()<<"*Exp["<<n.toStdString()<<"*t]+";
    }
    f<<"0;"<<endl;
    f<<"ds = -D[Log[s]/12, t];"<<endl;
    f<<"t := 1/T;"<<endl;
    f<<"dss = D[ds, T];"<<endl;
    f<<"Plot[dss, {T, 0.001, 10000}, AxesLabel -> {\"T (k)\", \"C\"}, AspectRatio -> 1/2]"<<endl;
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
    double multiplier = pow(config::Instance()->ergGauss,2.)/pow(config::Instance()->santiMeter,3.); //множитель для вычисления энергии в ЭРГах
    int w=80,h=220;//высота и ширина частицы
    for (int d=300;d<10000;d+=100){ //параметр решетки
        int l = (2*d)+w+1; //высота и ширина решетки (4 ячейки всего)
        PartArray *sys= new PartArray(l,l,1);
        sys->dropSpinIce(w,h,d);
        //sys->save("system.dat");
        cout<<"n="<<sys->count()<<"; d="<<d<<endl;
        double eInit = sys->calcEnergy1FastIncrementalFirst();
        double e;
        QHash<double,int> statSumm;
        int i=0;
        do {
            e = sys->calcEnergy1FastIncremental(eInit);
            if (statSumm.contains(e))
                statSumm[e] += 1;
            else
                statSumm[e] = 1;

            i++;
            if (i>10000){
                //saveMathHash(&statSumm,multiplier,QString("energiesMath_")+QString::number(sys->count())+QString("_")+QString::number(d)+QString(".dat"));
                i=0;
            }
        } while (sys->state->next());
        saveMathHash(&statSumm,multiplier,QString("energiesMath_")+QString::number(sys->count())+QString("_")+QString::number(d)+QString(".dat"));
        statSumm.clear();
        delete sys;
    }

    cout<<"finish"<<endl;
    return a.exec();
}
