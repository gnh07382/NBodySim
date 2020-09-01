#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <array>
#include <vector>
#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>

#define K pow(2.0, 1.0 / 3.0)

//한 번 실행

template <typename Data>//Data는 고정 길이 배열임
class Euler 
{
public:
	Euler()
	{
		
	}

	template <typename Dynamics>
	Data do_step(Dynamics& system, Data& x, double dt, double t, Data& dxdt)
	{
		for (auto& const [xi, dxdti] : boost::combine(x, dxdt))
			xi += system(xi, dxdti) * dt;

		return x;
	}
};

template <typename Data>
class RK4
{
public:
	RK4()
	{
	
	}

	template <typename Dynamics>
	void do_step(Dynamics& system, Data& x, double dt, double t, Data& dxdt)
	{
		for (auto& const [xi, dxdti] : boost::combine(x, dxdt))
		{
			double k1, k2, k3, k4;

			k1 = dt * system(xi, dxdti);
			k2 = dt * system(xi + k1 / 2.0, dxdti);
			k3 = dt * system(xi + k2 / 2.0, dxdti);
			k4 = dt * system(xi + k3, dxdti);

			xi += k1 + 2.0 * k2 + 2.0 * k3 + k4 / 6.0;
		}
	}
};

template <typename Data>
class SymplecticForestRuth
{
private:
	const std::array<double, 4> c = 
	{
		1.0 / (2.0 * K),
		(1 - K) / (2 * (2 - K)),
		(1 - K) / (2 * (2 - K)),
		1.0 / (2.0 * K)
	};

	const std::array<double, 4> d =
	{
		1.0 / (2 - K),
		-K / (2 - K),
		1.0 / (2 - K),
		0
	};

public:
	SymplecticForestRuth();

	template <typename Dynamics>
	void do_step(Dynamics& system, Data& q, Data& p, Data& d2xdt2, double dt)
	{
		for (auto& const i : boost::combine(q, p, d2xdt2))//Boost를 쓰던가 해야될듯
		{

			double qi;
			double pi;
			double d2xdt2i;
			boost::tie(qi, pi, d2xdt2i)=i;
			//q:위치, p: 속도
			double q1, q2, q3, q4;
			double p1, p2, p3, p4;

			q1 = qi + c[0] * p * dt;
			p1 = pi + d[0] * d2xdt2 * dt;
			q2 = q1 + c[1] * p1 * dt;
			p2 = p1 + d[1] * d2xdt2 * dt;
			q3 = q2 + c[2] * p2 * dt;
			p3 = p2 + d[2] * d2xdt2 * dt;
			q4 = q3 + c[3] * p3 * dt;
			p4 = q3 + d[3] * d2xdt2 * dt;

			qi = q4;
			pi = p4;
		}
	}
};

#endif // !INTEGRATOR_H
//심플렉틱, RK4, 오일러