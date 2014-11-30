#include "FlowerAlgorithm.h"
#include <stdlib.h>
#include <vector>
#include "AlgorithmController.h"
#include <algorithm>
#include "TwoOpt.h"

//#define M 5

bool m_Candidate_compareByAngle(const m_Candidate &a, const m_Candidate &b)
{
	return a.angle < b.angle;
}

std::vector<m_Candidate> m_getCandidates(CitiesData cd)
{
	std::vector<m_Candidate> r;

	for(int i = 0; i < cd.count; i++){
		m_Candidate c;
		c.c = cd.cities[i];
		c.distance = distanceBetween(c.c,cd.warehouse);

		c.cos_th = cos((c.c.location.x - cd.warehouse.location.x)/c.distance);
		c.sin_th = sin((c.c.location.y - cd.warehouse.location.y)/c.distance);

		double th = atan2((c.c.location.y - cd.warehouse.location.y),(c.c.location.x - cd.warehouse.location.x)); // ????

		c.angle = th;

		r.push_back(c);
	}

	//sort by angle
	std::sort(r.begin(), r.end(),m_Candidate_compareByAngle);

	return r;
}

Solution Flower_ConstructSolution(int angleDisp, std::vector<m_Candidate> &candidates, int &i, CitiesData &cd)
{
	Solution s;
	s.num_routes = 0;
	s.routes = nullptr;

	std::rotate(candidates.begin(),candidates.begin()+angleDisp,candidates.end());

	while(candidates.size() > 0){
		i = s.num_routes -1;

		m_Candidate chosen = take(candidates,candidates.size()-1);

		if(i >= 0 && s.routes[i].num_cities < cd.k + 2){
			// dodajemy do ostatniej, niepe³nej trasy
			s.routes[i].num_cities++;
			s.routes[i].cities = (City*)realloc((void*)s.routes[i].cities,s.routes[i].num_cities*sizeof(City));
			s.routes[i].cities[s.routes[i].num_cities-2] = chosen.c;
			s.routes[i].cities[s.routes[i].num_cities-1] = cd.warehouse;
		}else{
			// ostatnia trasa jest pe³na, wiêc tworzymy now¹ z jednym elementem
			i++;
			s.num_routes++;
			if(i > 0){
				s.routes = (Route*)realloc(s.routes,s.num_routes*sizeof(Route));
			}else{
				s.routes = (Route*)malloc(s.num_routes*sizeof(Route));
			}

			s.routes[i].num_cities = 3;
			s.routes[i].cities = (City*)malloc(3*sizeof(City));
			s.routes[i].cities[0] = cd.warehouse;
			s.routes[i].cities[1] = chosen.c;
			s.routes[i].cities[2] = cd.warehouse;		
		}
	}

	s = twoOpt(s);

	return s;
}

int Flower_Optimize(Solution &s, CitiesData &cd, int i)
{
	double totalGain;
	do{
		totalGain = 0.0;
		double bef = getRoutesLength(s);

		for (int szpara = 1; szpara <= cd.k; szpara++)
		{
			float gain;
			do{
				if(s.num_routes < (szpara * 2 + 1)) break;
				gain = 0.0;
				double before = getRoutesLength(s);

				for (i = szpara; i < s.num_routes - szpara; i++)
				{
					Route left = s.routes[i-szpara];
					Route r = s.routes[i];
					Route right = s.routes[i+szpara];

					twoOpt(r,left,right);
				}

				s = twoOpt(s);

				double after = getRoutesLength(s);

				gain = before - after;
			}while(gain > 0);
		}

		double mid = getRoutesLength(s);

		for (int szpara = cd.k; szpara > 0; szpara--)
		{
			float gain;
			do{
				if(s.num_routes < (szpara * 2 + 1)) break;
				gain = 0.0;
				double before = getRoutesLength(s);

				for (i = szpara; i < s.num_routes - szpara; i++)
				{
					Route left = s.routes[i-szpara];
					Route r = s.routes[i];
					Route right = s.routes[i+szpara];

					twoOpt(r,left,right);
				}

				s = twoOpt(s);

				double after = getRoutesLength(s);

				gain = before - after;
			}while(gain > 0);
		}

		double aft = getRoutesLength(s);
		totalGain = bef - aft;
	}while(totalGain > 0);	return i;
}

Solution Flower_findPath(CitiesData cd){
	Solution s;
	int i;
	
	std::vector<m_Candidate> candidates = m_getCandidates(cd);
	
	Solution best = Flower_ConstructSolution(0,candidates,i,cd);
	for (int angleDisp = 1; angleDisp < cd.count; angleDisp++)
	{
		std::vector<m_Candidate> candidates = m_getCandidates(cd);
		s = Flower_ConstructSolution(angleDisp, candidates, i, cd);
		Flower_Optimize(s, cd, i);
		if(getRoutesLength(s) < getRoutesLength(best)){
			printf("new best: %f rotated by: %3.3f%c\n",getRoutesLength(s),(angleDisp*100.0/cd.count),'%');
			best = s;
		}
	}

	return best;
}