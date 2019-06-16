
#include "pch.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Engine
{
public:


	double l = 10, Hm = 0.01, Hv = 0001, C = 0.1, Ts = 0, Tmax = 110 + 273;
	vector <double> M { 20,75,100,105,75,0 };
	vector <double> V { 0,75,150,200,250,300 };

	double KFk(double x1, double x2, double y1, double y2)//Для работы с кусочно-линейной зависиомтью, мы представляем ее в виде
		//кусочно-заданной функии, состоящей из уравнений вида y = kx + b. Данная функция находит соответствующий коэффициент k.
	{
		double k;
		k = (y1 - y2) / (x1 - x2);


		return k;
	}
	double KFb(double x1, double x2, double y1, double y2)//Данная функция находит коэффициент b, для получения уравнений прямых. 
	{
		double k, b;
		k = (y1 - y2) / (x1 - x2);
		b = y2 - k * x2;

		return b;

	}
	double Mv(double Vt)//Значение крутящего момента от мгновенного значения углового ускорения в момент t.
	{
		double Mm;
		if (0 <= Vt && Vt < 75)
		{
			Mm = KFk(V[0], V[1], M[0], M[1])*Vt + KFb(V[0], V[1], M[0], M[1]);
		}
		else if (75 <= Vt && Vt < 150)
		{
			Mm = KFk(V[1], V[2], M[1], M[2])*Vt + KFb(V[1], V[2], M[1], M[2]);
		}
		else if (150 <= Vt && Vt < 200)
		{
			Mm = KFk(V[2], V[3], M[2], M[3])*Vt + KFb(V[2], V[3], M[2], M[3]);
		}
		else if (200 <= Vt && Vt < 250)
		{
			Mm = KFk(V[3], V[4], M[3], M[4])*Vt + KFb(V[3], V[4], M[3], M[4]);
		}
		else if (250 <= Vt && Vt <= 300)
		{
			Mm = KFk(V[4], V[5], M[4], M[5])*Vt + KFb(V[4], V[5], M[4], M[5]);
		}
		else
		{
			Mm = -1;
		}
		return Mm;
	}
	double Vt(int t)//Мгновенное значение углового ускорения в момент t.
	{
		double Vv;

		if (t == 0)
		{
			Vv = V[0];

		}
		else
		{
			Vv = Vt(t - 1) + At(t-1);
		}

		return Vv;
	} 
	double At(int t)//Мгновенное ускорение в момент t.
	{
		double A=0,L = 10;
		if(t==0)
		{
			A = Mv(V[0]) / L;
		}
		
		else
		{
			A = Mv(Vt(t)) / L;
		}
		
			
		
		return A;
	}
	double Td(int t,double Ts)//Мгновенное значение температуры двигателя, в момент t.
	{
		double T;
		if (t ==0)
		{ 
			T = Ts;
		}
		else
		{
			T = Td(t - 1,Ts) + Vtd(t-1,Ts);
		}
		return T;
	}
	double Vtd(int t, double Ts)//Изменение температуры двигателя за 1 секунду.
	{
		double dT;
		if (t == 0)
		{
			dT = (Mv(Vt(t))*Hm + Vt(t)*Vt(t)*Hv);

		}

		else
		{
			dT = (Mv(Vt(t))*Hm + Vt(t)*Vt(t)*Hv) + C*(Ts - Td(t, Ts));
		}

		return dT;
	}
	float temp()//Ввод температуры среды с клавиатуры.
	{
		float Ts = 0;
		int gj = 0;
		while (gj < 1)
		{
			cout << "Введите целое число в диапазоне от -273 до 110: " << endl;
			cin >> Ts;
			Ts += 273;
			if (Ts < 0 || Ts>120 + 273)
				cout << "Вы ввели неверное число, попробуйте ввести число заново." << endl;
			else
				gj++;

		}

		cout << "Температура среды: " << Ts << " по Кельвину." << endl;

		return Ts;
	}
};




int main()
{
	setlocale(LC_ALL, "Russian");
	Engine DVS;
	int t = 0;
	double Ts = DVS.temp(), Td = 0;

	while (DVS.Td(t,Ts) < DVS.Tmax)
	{

		cout << "Идет " << t << " секунда, температура двигателя равна: " << DVS.Td(t, Ts) << " по Кельвину." << endl;
		t++;
	}
	
	 
	if (t == 2 || t == 3 || t == 4)
		cout << "Двигатель перегреется через " << t << " секунды." << endl;
	else if (t == 0 || t>= 5)
		cout << "Двигатель перегреется через " << t << " секунд." << endl;
	else
		cout << "Двигатель перегреется через " << t << " секунду." << endl;

	system("Pause");
	return 0;
}

