#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "cerberusdef.h"
#include "benchmark.h"


// ==============Benchmark Vars==============
#define DEFAUL_RUNS 1000
#define BENC_SIZE_SAMPLES 100
RCV_DATA_PR_BUS __Rcv[BENC_SIZE_SAMPLES]; //-> input info
// 30 canais AC -> com calculo de frequências e detecção de defeitos + organização nos buffers todos de amostras com calculo de ADC para grandeza de engenharia
// 30 canais derivados (15 residuais + 15 ph-ph)
// 30 grupos AC base (DFT) e RMS de todas as fases

int _Debug;


// DSP Internal Algorithms
// ***************************************************************************
FLOAT32 FilterLPHP[2][AC_LP_HP_FILTER_ORDER] = {
													{0, 1},	// LowPass for voltage
													{-3.196625392, 3.196625392},	// HighPass for current
												};

FLOAT32 HighPassMgnComp[2] = { 0.049224 , 0.0155 };

FLOAT32 Cos_Sin[2*ONE_CYCLE_WINDOW-1][2]={
											{1.000000	,	0.000000},
 											{0.951057	,	-0.309017},
 											{0.809017	,	-0.587785},
 											{0.587785	,	-0.809017},
 											{0.309017	,	-0.951057},
 											{0.000000	,	-1.000000},
 											{-0.309017	,	-0.951057},
 											{-0.587785	,	-0.809017},
 											{-0.809017	,	-0.587785},
 											{-0.951057	,	-0.309017},
 											{-1.000000	,	-0.000000},
 											{-0.951057	,	0.309017},
 											{-0.809017	,	0.587785},
 											{-0.587785	,	0.809017},
 											{-0.309017	,	0.951057},
 											{-0.000000	,	1.000000},
 											{0.309017	,	0.951057},
 											{0.587785	,	0.809017},
 											{0.809017	,	0.587785},
 											{0.951057	,	0.309017},
 											{1.000000	,	0.000000},
 											{0.951057	,	-0.309017},
 											{0.809017	,	-0.587785},
 											{0.587785	,	-0.809017},
 											{0.309017	,	-0.951057},
 											{0.000000	,	-1.000000},
 											{-0.309017	,	-0.951057},
 											{-0.587785	,	-0.809017},
 											{-0.809017	,	-0.587785},
 											{-0.951057	,	-0.309017},
 											{-1.000000	,	-0.000000},
 											{-0.951057	,	0.309017},
 											{-0.809017	,	0.587785},
 											{-0.587785	,	0.809017},
 											{-0.309017	,	0.951057},
 											{-0.000000	,	1.000000},
 											{0.309017	,	0.951057},
 											{0.587785	,	0.809017},
 											{0.809017	,	0.587785},
										 };

FLOAT32 __OrthogCoef[ONE_CYCLE_WINDOW][15][3]=
{
	{	{0.550666	,	0.423694	,	0.825999},	//{A C B} 5 samples starting in 0
 		{0.452355	,	0.232034	,	0.452355},
 		{0.370571	,	0.118156	,	0.293788},
 		{0.302012	,	0.050970	,	0.227948},
 		{0.245225	,	0.014695	,	0.204775},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.166667	,	-0.000000	,	0.200000},
 		{0.145189	,	0.008374	,	0.196735},
 		{0.134224	,	0.018981	,	0.186474},
 		{0.130715	,	0.026697	,	0.169508},
 		{0.130519	,	0.028693	,	0.149165},	//{A C B} 15 samples starting in 0
		{0.129803	,	0.024968	,	0.129803},
 		{0.126240	,	0.017650	,	0.114770},
 		{0.119345	,	0.009606	,	0.105386},
 		{0.110050	,	0.003265	,	0.101061}, //{A C B} 19 samples starting in 0
	},
	{	{0.825999	,	0.423694	,	0.550666},	//{A C B} 5 samples starting in 1
 		{0.588741	,	0.187719	,	0.315968},
 		{0.432689	,	0.073025	,	0.231670},
 		{0.324899	,	0.019469	,	0.205061},
 		{0.250000	,	-0.000000	,	0.200000},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.169850	,	0.009796	,	0.196817},
 		{0.155034	,	0.021924	,	0.186891},
 		{0.150371	,	0.030712	,	0.170328},
 		{0.150112	,	0.033000	,	0.150112},
 		{0.149165	,	0.028693	,	0.130519},	//{A C B} 15 samples starting in 1
		{0.144479	,	0.020200	,	0.115127},
 		{0.135519	,	0.010908	,	0.105491},
 		{0.123659	,	0.003669	,	0.101073},
		{0.111111	,	-0.000000	,	0.100000}, //{A C B} 19 samples starting in 1
	},
	{	{1.048748	,	0.261857	,	0.327917},
 		{0.673032	,	0.071702	,	0.231677},
 		{0.456416	,	0.000000	,	0.207943},
 		{0.324899	,	-0.019469	,	0.205061},
 		{0.245225	,	-0.014695	,	0.204775},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.178183	,	0.015851	,	0.188484},
 		{0.170962	,	0.027099	,	0.170962},
 		{0.170328	,	0.030712	,	0.150371},
 		{0.169508	,	0.026697	,	0.130715},
 		{0.164249	,	0.017733	,	0.115435},
		{0.153549	,	0.007716	,	0.106057},
 		{0.139063	,	0.000000	,	0.101947},
 		{0.123659	,	-0.003669	,	0.101073},
		{0.110050	,	-0.003265	,	0.101061}, 
	},
	{	{1.133831	,	0.000000	,	0.242834},
 		{0.673032	,	-0.071702	,	0.231677},
 		{0.432689	,	-0.073025	,	0.231670},
 		{0.302012	,	-0.050970	,	0.227948},
 		{0.232725	,	-0.023776	,	0.217275},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.188484	,	0.015851	,	0.178183},
 		{0.186891	,	0.021924	,	0.155034},
 		{0.186474	,	0.018981	,	0.134224},
 		{0.181496	,	0.010197	,	0.118727},
 		{0.170011	,	0.000000	,	0.109673}, 
		{0.153549	,	-0.007716	,	0.106057},
 		{0.135519	,	-0.010908	,	0.105491},
 		{0.119345	,	-0.009606	,	0.105386},
		{0.107272	,	-0.005284	,	0.103839}, 
	},
	{	{1.048748	,	-0.261857	,	0.327917},
 		{0.588741	,	-0.187719	,	0.315968},
 		{0.370571	,	-0.118156	,	0.293788},
 		{0.264980	,	-0.063003	,	0.264980},
 		{0.217275	,	-0.023776	,	0.232725},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.196817	,	0.009796	,	0.169850},
 		{0.196735	,	0.008374	,	0.145189},
 		{0.192642	,	-0.000000	,	0.128057},
 		{0.181496	,	-0.010197	,	0.118727},
 		{0.164249	,	-0.017733	,	0.115435}, 
		{0.144479	,	-0.020200	,	0.115127},
 		{0.126240	,	-0.017650	,	0.114770},
 		{0.112366	,	-0.011874	,	0.112366},
		{0.103839	,	-0.005284	,	0.107272}, 
	},
	{	{0.825999	,	-0.423694	,	0.550666},
 		{0.452355	,	-0.232034	,	0.452355},
 		{0.293788	,	-0.118156	,	0.370571},
 		{0.227948	,	-0.050970	,	0.302012},
 		{0.204775	,	-0.014695	,	0.245225},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.200000	,	-0.000000	,	0.166667},
 		{0.196735	,	-0.008374	,	0.145189},
 		{0.186474	,	-0.018981	,	0.134224},
 		{0.169508	,	-0.026697	,	0.130715},
 		{0.149165	,	-0.028693	,	0.130519}, 
		{0.129803	,	-0.024968	,	0.129803},
 		{0.114770	,	-0.017650	,	0.126240},
 		{0.105386	,	-0.009606	,	0.119345},
		{0.101061	,	-0.003265	,	0.110050}, 
	},
	{	{0.550666	,	-0.423694	,	0.825999},
 		{0.315968	,	-0.187719	,	0.588741},
 		{0.231670	,	-0.073025	,	0.432689},
 		{0.205061	,	-0.019469	,	0.324899},
 		{0.200000	,	-0.000000	,	0.250000},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.196817	,	-0.009796	,	0.169850},
 		{0.186891	,	-0.021924	,	0.155034},
 		{0.170328	,	-0.030712	,	0.150371},
 		{0.150112	,	-0.033000	,	0.150112},
 		{0.130519	,	-0.028693	,	0.149165}, 
		{0.115127	,	-0.020200	,	0.144479},
 		{0.105491	,	-0.010908	,	0.135519},
 		{0.101073	,	-0.003669	,	0.123659},
		{0.100000	,	-0.000000	,	0.111111},
	},
	{	{0.327917	,	-0.261857	,	1.048748},
 		{0.231677	,	-0.071702	,	0.673032},
 		{0.207943	,	-0.000000	,	0.456416},
 		{0.205061	,	0.019469	,	0.324899},
 		{0.204775	,	0.014695	,	0.245225},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.188484	,	-0.015851	,	0.178183},
 		{0.170962	,	-0.027099	,	0.170962},
 		{0.150371	,	-0.030712	,	0.170328},
 		{0.130715	,	-0.026697	,	0.169508},
 		{0.115435	,	-0.017733	,	0.164249},
		{0.106057	,	-0.007716	,	0.153549},
 		{0.101947	,	-0.000000	,	0.139063},
 		{0.101073	,	0.003669	,	0.123659},
		{0.101061	,	0.003265	,	0.110050}, 
	},
	{	{0.242834	,	-0.000000	,	1.133831},
 		{0.231677	,	0.071702	,	0.673032},
 		{0.231670	,	0.073025	,	0.432689},
 		{0.227948	,	0.050970	,	0.302012},
 		{0.217275	,	0.023776	,	0.232725},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.178183	,	-0.015851	,	0.188484},
 		{0.155034	,	-0.021924	,	0.186891},
 		{0.134224	,	-0.018981	,	0.186474},
 		{0.118727	,	-0.010197	,	0.181496},
 		{0.109673	,	-0.000000	,	0.170011},
		{0.106057	,	0.007716	,	0.153549},
 		{0.105491	,	0.010908	,	0.135519},
 		{0.105386	,	0.009606	,	0.119345},
		{0.103839	,	0.005284	,	0.107272},
	},
	{	{0.327917	,	0.261857	,	1.048748},
 		{0.315968	,	0.187719	,	0.588741},
 		{0.293788	,	0.118156	,	0.370571},
 		{0.264980	,	0.063003	,	0.264980},
 		{0.232725	,	0.023776	,	0.217275},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.169850	,	-0.009796	,	0.196817},
 		{0.145189	,	-0.008374	,	0.196735},
 		{0.128057	,	-0.000000	,	0.192642},
 		{0.118727	,	0.010197	,	0.181496},
 		{0.115435	,	0.017733	,	0.164249}, 
		{0.115127	,	0.020200	,	0.144479},
 		{0.114770	,	0.017650	,	0.126240},
 		{0.112366	,	0.011874	,	0.112366},
		{0.107272	,	0.005284	,	0.103839},
	},
	{	{0.550666	,	0.423694	,	0.825999},
 		{0.452355	,	0.232034	,	0.452355},
 		{0.370571	,	0.118156	,	0.293788},
 		{0.302012	,	0.050970	,	0.227948},
 		{0.245225	,	0.014695	,	0.204775},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.166667	,	-0.000000	,	0.200000},
 		{0.145189	,	0.008374	,	0.196735},
 		{0.134224	,	0.018981	,	0.186474},
 		{0.130715	,	0.026697	,	0.169508},
 		{0.130519	,	0.028693	,	0.149165}, 
		{0.129803	,	0.024968	,	0.129803},
 		{0.126240	,	0.017650	,	0.114770},
 		{0.119345	,	0.009606	,	0.105386},
		{0.110050	,	0.003265	,	0.101061}, 
	},
	{	{0.825999	,	0.423694	,	0.550666},
 		{0.588741	,	0.187719	,	0.315968},
 		{0.432689	,	0.073025	,	0.231670},
 		{0.324899	,	0.019469	,	0.205061},
 		{0.250000	,	-0.000000	,	0.200000},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.169850	,	0.009796	,	0.196817},
 		{0.155034	,	0.021924	,	0.186891},
 		{0.150371	,	0.030712	,	0.170328},
 		{0.150112	,	0.033000	,	0.150112},
 		{0.149165	,	0.028693	,	0.130519},
		{0.144479	,	0.020200	,	0.115127},
 		{0.135519	,	0.010908	,	0.105491},
 		{0.123659	,	0.003669	,	0.101073},
		{0.111111	,	-0.000000	,	0.100000},
	},
	{	{1.048748	,	0.261857	,	0.327917},
 		{0.673032	,	0.071702	,	0.231677},
 		{0.456416	,	0.000000	,	0.207943},
 		{0.324899	,	-0.019469	,	0.205061},
 		{0.245225	,	-0.014695	,	0.204775},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.178183	,	0.015851	,	0.188484},
 		{0.170962	,	0.027099	,	0.170962},
 		{0.170328	,	0.030712	,	0.150371},
 		{0.169508	,	0.026697	,	0.130715},
 		{0.164249	,	0.017733	,	0.115435}, 
		{0.153549	,	0.007716	,	0.106057},
 		{0.139063	,	0.000000	,	0.101947},
 		{0.123659	,	-0.003669	,	0.101073},
		{0.110050	,	-0.003265	,	0.101061}, 
	},
	{	{1.133831	,	0.000000	,	0.242834},
 		{0.673032	,	-0.071702	,	0.231677},
 		{0.432689	,	-0.073025	,	0.231670},
 		{0.302012	,	-0.050970	,	0.227948},
 		{0.232725	,	-0.023776	,	0.217275},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.188484	,	0.015851	,	0.178183},
 		{0.186891	,	0.021924	,	0.155034},
 		{0.186474	,	0.018981	,	0.134224},
 		{0.181496	,	0.010197	,	0.118727},
 		{0.170011	,	0.000000	,	0.109673}, 
		{0.153549	,	-0.007716	,	0.106057},
 		{0.135519	,	-0.010908	,	0.105491},
 		{0.119345	,	-0.009606	,	0.105386},
		{0.107272	,	-0.005284	,	0.103839}, 
	},
	{	{1.048748	,	-0.261857	,	0.327917},
 		{0.588741	,	-0.187719	,	0.315968},
 		{0.370571	,	-0.118156	,	0.293788},
 		{0.264980	,	-0.063003	,	0.264980},
 		{0.217275	,	-0.023776	,	0.232725},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.196817	,	0.009796	,	0.169850},
 		{0.196735	,	0.008374	,	0.145189},
 		{0.192642	,	0.000000	,	0.128057},
 		{0.181496	,	-0.010197	,	0.118727},
 		{0.164249	,	-0.017733	,	0.115435}, 
		{0.144479	,	-0.020200	,	0.115127},
 		{0.126240	,	-0.017650	,	0.114770},
 		{0.112366	,	-0.011874	,	0.112366},
		{0.103839	,	-0.005284	,	0.107272}, 
	},
	{	{0.825999	,	-0.423694	,	0.550666},
 		{0.452355	,	-0.232034	,	0.452355},
 		{0.293788	,	-0.118156	,	0.370571},
 		{0.227948	,	-0.050970	,	0.302012},
 		{0.204775	,	-0.014695	,	0.245225},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.200000	,	0.000000	,	0.166667},
 		{0.196735	,	-0.008374	,	0.145189},
 		{0.186474	,	-0.018981	,	0.134224},
 		{0.169508	,	-0.026697	,	0.130715},
 		{0.149165	,	-0.028693	,	0.130519}, 
		{0.129803	,	-0.024968	,	0.129803},
 		{0.114770	,	-0.017650	,	0.126240},
 		{0.105386	,	-0.009606	,	0.119345},
		{0.101061	,	-0.003265	,	0.110050}, 
	},
	{	{0.550666	,	-0.423694	,	0.825999},
 		{0.315968	,	-0.187719	,	0.588741},
 		{0.231670	,	-0.073025	,	0.432689},
 		{0.205061	,	-0.019469	,	0.324899},
 		{0.200000	,	-0.000000	,	0.250000},
 		{0.200000	,	0.000000	,	0.200000},
 		{0.196817	,	-0.009796	,	0.169850},
 		{0.186891	,	-0.021924	,	0.155034},
 		{0.170328	,	-0.030712	,	0.150371},
 		{0.150112	,	-0.033000	,	0.150112},
 		{0.130519	,	-0.028693	,	0.149165},
		{0.115127	,	-0.020200	,	0.144479},
 		{0.105491	,	-0.010908	,	0.135519},
 		{0.101073	,	-0.003669	,	0.123659},
		{0.100000	,	-0.000000	,	0.111111}, 
	},
	{	{0.327917	,	-0.261857	,	1.048748},
 		{0.231677	,	-0.071702	,	0.673032},
 		{0.207943	,	-0.000000	,	0.456416},
 		{0.205061	,	0.019469	,	0.324899},
 		{0.204775	,	0.014695	,	0.245225},
 		{0.200000	,	0.000000	,	0.200000},
 		{0.188484	,	-0.015851	,	0.178183},
 		{0.170962	,	-0.027099	,	0.170962},
 		{0.150371	,	-0.030712	,	0.170328},
 		{0.130715	,	-0.026697	,	0.169508},
 		{0.115435	,	-0.017733	,	0.164249},
		{0.106057	,	-0.007716	,	0.153549},
 		{0.101947	,	-0.000000	,	0.139063},
 		{0.101073	,	0.003669	,	0.123659},
		{0.101061	,	0.003265	,	0.110050},
	},
	{	{0.242834	,	-0.000000	,	1.133831},
 		{0.231677	,	0.071702	,	0.673032},
 		{0.231670	,	0.073025	,	0.432689},
 		{0.227948	,	0.050970	,	0.302012},
 		{0.217275	,	0.023776	,	0.232725},
 		{0.200000	,	-0.000000	,	0.200000},
 		{0.178183	,	-0.015851	,	0.188484},
 		{0.155034	,	-0.021924	,	0.186891},
 		{0.134224	,	-0.018981	,	0.186474},
 		{0.118727	,	-0.010197	,	0.181496},
 		{0.109673	,	-0.000000	,	0.170011}, 
		{0.106057	,	0.007716	,	0.153549},
 		{0.105491	,	0.010908	,	0.135519},
 		{0.105386	,	0.009606	,	0.119345},
		{0.103839	,	0.005284	,	0.107272}, 
		
	},
	{	{0.327917	,	0.261857	,	1.048748},	//{A C B} 5 samples starting in 19
 		{0.315968	,	0.187719	,	0.588741},
 		{0.293788	,	0.118156	,	0.370571},
 		{0.264980	,	0.063003	,	0.264980},
 		{0.232725	,	0.023776	,	0.217275},
 		{0.200000	,	0.000000	,	0.200000},
 		{0.169850	,	-0.009796	,	0.196817},
 		{0.145189	,	-0.008374	,	0.196735},
 		{0.128057	,	-0.000000	,	0.192642},
 		{0.118727	,	0.010197	,	0.181496},
 		{0.115435	,	0.017733	,	0.164249},	
		{0.115127	,	0.020200	,	0.144479},
 		{0.114770	,	0.017650	,	0.126240},
 		{0.112366	,	0.011874	,	0.112366},
		{0.107272	,	0.005284	,	0.103839}, //{A C B} 19 samples starting in 19
	},
};

FLOAT32 RmsFactorDC[17] = {	//  sqrt(2)/Nsamples -> from 32 to 48 samples
0.044194174,
0.042854956,
0.041594517,
0.040406102,
0.03928371,
0.038221988,
0.037216146,
0.036261886,
0.035355339,
0.034493014,
0.033671751,
0.032888687,
0.032141217,
0.031426968,
0.030743773,
0.03008965,
0.029462783};

FLOAT32 RmsFactor[17] = {  //  2/Nsamples -> from 32 to 48 samples
0.0625,
0.060606061,
0.058823529,
0.057142857,
0.055555556,
0.054054054,
0.052631579,
0.051282051,
0.05,
0.048780488,
0.047619048,
0.046511628,
0.045454545,
0.044444444,
0.043478261,
0.042553191,
0.041666667};

#define ONE2FOUR_KHZ 0.25f

#define DFT_FullCycle_Comp 1u
#define DFT_HalfCycle_Comp 0u

SAMPLEWINDOW		SampleWindow;
SAMPLINGVARS		SamplingVars;
ACQUANTITYCONFIG	ACQuantityConfig[MAX_AC_QUANTITIES];
UINT32			msg;
DerivedSamplesCfg 	DerivedSamples[MAX_CALCULATED_SAMPLES];
INTERNALGROUP Groups[MAX_GROUPS];
ESTIMATIONVARS EstimationVars;
PHASOR PhasorBuffer[MAX_AC_LOCAL_QUANTITIES];

//==========================================


int start_calculation(int initial , int repeat){
    int _RUNS;
    int _Estimation=0;
    int _ISR=0;
    int i, j, sample, ch;
    struct timespec ts_start;
    struct timespec ts_end;
    
    ACQUANTITYCONFIG* pACInput = ACQuantityConfig;

    // if(argc<2){
    //     printf("[Ce] No runs set. Running with default value.\n");
    //     _RUNS = DEFAUL_RUNS;
    // }else{
    //     _RUNS = atoi(argv[2]);
    // }
    // printf("[Ce] Start Benchmark with %d runs\n", _RUNS);

    Init();

    // clock_gettime(CLOCK_MONOTONIC, &ts_start);

    while (_ISR < initial) {
        
        if(ProcessBusISR(&__Rcv[_ISR%BENC_SIZE_SAMPLES])){
            CalculateAC();
            _Estimation++;
        }
        _ISR++;
        //usleep(1000);
    }
	
	_ISR = 0;

    //===============================================================================================================================
    //============================================ Benchmark Set Start analysis (LEDs ON)?!!!===========================================
    //===============================================================================================================================
	GPIO_PinWrite(TEST_GPIO, TEST_GPIO_PIN, 1U);

    while (_ISR < repeat) {
        
        if(ProcessBusISR(&__Rcv[_ISR%BENC_SIZE_SAMPLES])){
            CalculateAC();
            _Estimation++;
        }
        _ISR++;
        //usleep(1000);
    }

	GPIO_PinWrite(TEST_GPIO, TEST_GPIO_PIN, 0U);

    //=============================================================================================================================
    //================================================== Benchmark End analysis (LEDs OFF )!!!=========================================
    //=============================================================================================================================

    // clock_gettime(CLOCK_MONOTONIC, &ts_end);
    // printf("[Ce] Exit Benchmark for %d runs: ISR run %d times and Estimation %d times and took %fus\n", _RUNS, _ISR, _Estimation, (ts_end.tv_nsec-ts_start.tv_nsec)/1000.0);
	return 0;
}

void Init(){
    int i,j,sample,ch;
    ACQUANTITYCONFIG* pACInput = ACQuantityConfig;

    // »»»»»»»»»»»»»»»»»»»»»»»»»»» INIT»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
    memset(__Rcv, 0, sizeof(__Rcv));
    for(i=0; i<BENC_SIZE_SAMPLES; i++){
        __Rcv[i].TimeStamp = 1000*i;
        __Rcv[i].SmpCnt = 4*i;
        for(sample=0; sample < RAW_SAMPLES_PER_ISR; sample++){
            for(ch=0; ch < MAX_AC_LOCAL_QUANTITIES; ch++){
                __Rcv[i].Smp[sample][ch]= 1000 * sin(0.07853981*i + ch*0.2); //2*pi*f*deltaT*i + ch*2*pi/30
            }
	    
        }
        for(ch=0; ch < MAX_AC_LOCAL_QUANTITIES; ch++){
            __Rcv[i].SmpAv[ch]= 1000 * sin(0.314159265*i + ch*0.2); //2*pi*f*deltaT*i + ch*2*pi/30
            //printf("%d ",__Rcv[i].SmpAv[ch]);
        }
        

    }

    	// Initialization of auxiliary variables
	memset(&SamplingVars, 0 ,sizeof(SamplingVars));
	SamplingVars.SamplePosition = 0;
	SamplingVars.LastSamplePosition = 0;
	SamplingVars.SampleIndex = 0;
	SamplingVars.RawSampleIndex = 0;
	SamplingVars.RawSampleIndexNext = LAST_CYCLE_RAW_WINDOW_SIZE;
	SamplingVars.LastSampleIndex = 0;	
	SamplingVars.AwakeCounter = ONE_CYCLE_WINDOW;
	SamplingVars.FaultDetectorHistoricSample = 0;
	SamplingVars.FaultDetectorStatus = ONE_CYCLE_WINDOW;
	SamplingVars.StabilityCounter = 0;
	SamplingVars.FaultDetectorSampleIndex=0;
	SamplingVars.StartFilterDelay = 0;
	SamplingVars.SampleCopy = 0;
	SamplingVars.UsedSampleBuffers = 0;
	SamplingVars.UsedFreqVars = 0;
	SamplingVars.ISRError = 0;
	SamplingVars.AcquisitionType = LOCAL_ACQUISITION;

	memset(&ACQuantityConfig, 0 ,sizeof(ACQuantityConfig));
	for (i = 0; i < MAX_AC_QUANTITIES; i++) {
		pACInput->Type = 0;
		pACInput->FilterType = i%2;
		pACInput->TypeInfo = AC_CURRENT_INPUT;
		pACInput->Origin = ORIG_LOCAL;
		pACInput->Offset.Offset = 0;
		pACInput->HighScale = 1.0;
		pACInput->LowScale = 1.0;
		pACInput->ConfiguredScale = ALL_SCALES;
		pACInput->RawOffset = i;
		pACInput->RawPrimaryConversion=i;
		pACInput->SecondaryRatedValue = i;
		pACInput->Ratio = 1.0;
		pACInput->MagnCorr = 1.0;
		pACInput->PhaseCorr = 0.0;
		pACInput->PrimaryConversion = i;
		pACInput->PrimaryConversionComp = i;
		pACInput->pGroupPhase = &(Groups[i].Estimation.A); //JP
		pACInput->pGroupPhase2h = NULL;  
		pACInput->pSecondEstimation = NULL; 
		pACInput->FDFrequencySecurity = TRUE;
		pACInput->pCoordGroupPhase = NULL;		
		pACInput->FaultDetectorActive	= FALSE;
	 // Input may trigger fault detecting (application decision)
		pACInput->DeltaFaultDetectorStart = CURRENT_FAULT_DETECTION_FACTOR;
		pACInput->DeltaFaultDetectorReset = CURRENT_STABILITY_DETECTION_FACTOR;
		pACInput->FaultDetected = FALSE;
		pACInput->HalfCycleCounter = 0;
		pACInput->OneCycleCounter = 0;
		pACInput->HalfCycleBeforeZeroTime = 0.0;
		pACInput->OneCycleBeforeZeroTime = 0.0;
		pACInput->Period = 0.0;
		pACInput->PeriodMem[0] = 0.0;
		pACInput->PeriodMem[1] = 0.0;
		pACInput->FreezeFreqUpdate = 0;
		pACInput->MemIndex = 0;
		pACInput->InputFreqInit = TOTAL_BLOCK_FREQUENCY_AFTER_INV;
		pACInput->MinMagFilterComp = MIN_MAG_FILTER_COMP_NORMAL;
		pACInput->MinMagFilterCompReset = MIN_MAG_FILTER_COMP_NORMAL_RESET;
		pACInput->DerivedFreVarsIndex1 = INVALID_IDX;
		pACInput->DerivedFreVarsIndex2 = INVALID_IDX;
		pACInput->FreqBckUpIdx = INVALID_IDX;
		pACInput->ValidPeriod = 0.0;
		pACInput->InvValidPeriod = 0;
		pACInput->FreqCompAlgo = 0.0;

		pACInput++;
	}

    memset(&SamplingVars, 0 ,sizeof(SamplingVars));
	SamplingVars.SamplePosition = 0;
	SamplingVars.LastSamplePosition = 0;
	SamplingVars.SampleIndex = 0;
	SamplingVars.RawSampleIndex = 0;
	SamplingVars.RawSampleIndexNext = LAST_CYCLE_RAW_WINDOW_SIZE;
	SamplingVars.LastSampleIndex = 0;	
	SamplingVars.AwakeCounter = ONE_CYCLE_WINDOW;
	SamplingVars.FaultDetectorHistoricSample = 0;
	SamplingVars.FaultDetectorStatus = ONE_CYCLE_WINDOW;
	SamplingVars.StabilityCounter = 0;
	SamplingVars.FaultDetectorSampleIndex=0;
	SamplingVars.StartFilterDelay = 0;
	SamplingVars.SampleCopy = 0;
	SamplingVars.UsedSampleBuffers = 0;
	SamplingVars.UsedFreqVars = 0;
	SamplingVars.ISRError = 0;
	SamplingVars.AcquisitionType = LOCAL_ACQUISITION;

    for(i=0; i<MAX_CALCULATED_SAMPLES; i++){
        SamplingVars.UsedSampleBuffers++;
        if(i<MAX_CALCULATED_SAMPLES/2){
            DerivedSamples[i].Type = DSC_TYPE_RESIDUAL_FROM_BASE;
            DerivedSamples[i].FirstParcel = i+1;
            DerivedSamples[i].SecondParcel = i+2;
            DerivedSamples[i].ThirdParcel = i+3;
        }else{
            DerivedSamples[i].Type = DSC_TYPE_VOLTAGE_PN2PP;
            DerivedSamples[i].FirstParcel = i-1;
            DerivedSamples[i].SecondParcel = i-2;
            DerivedSamples[i].ThirdParcel = i-3;
        }
    }

    memset(&Groups, 0, sizeof(Groups));
    for(i=0; i<MAX_GROUPS; i++){
        Groups[i].Config.GroupID = i+1;
        Groups[i].Config.Ratio = i*20;
        Groups[i].Config.RatedValue = (i+1) *200.3;
        Groups[i].Config.PrimaryConversion = (i+1) *1.3;
        Groups[i].Config.ACInputIndex_A_AB=i;
        Groups[i].Config.ACInputIndex_B_BC=UNUSED_INPUT;
        Groups[i].Config.ACInputIndex_C_CA=UNUSED_INPUT;
        Groups[i].Config.ACInputIndex_RES=UNUSED_INPUT;
    }
    
	

// »»»»»»»»»»»»»»»»»»»»»»»»»»» INIT»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
}

bool ProcessBusISR(RCV_DATA_PR_BUS* Rcv)
{
	INT32 j;
	INT32 SamplePos;
	FLOAT32 DeltaSample;
	FLOAT32 LastCycleLastSample;
	UINT16 LastCycleLastSampleInv;
	UINT16 LastCycleSampleIndexInv;
	FLOAT32 InterpoledZeroTime;
	INT32 NextRawSampleIndex=SamplingVars.RawSampleIndex+1;
	INT32 NextRMSRawIndex=SamplingVars.RawSampleIndexNext+1;
	INT32 RawSampleIndexNext=SamplingVars.RawSampleIndexNext;
    FLOAT32 NewPeriod;
    FLOAT32 DeltaNewPeriod;
    FLOAT32 AveragePeriod;

    INT32 MultCheckSamples[4] = {0};
    INT8 ZeroCrossSample_Pointer = 0;
    FLOAT32 ZerosCrossSamples[2] = {0};  // stored to calc. linear interpolation

    UINT16 Invalid1[MAX_AC_LOCAL_QUANTITIES], Invalid2[MAX_AC_LOCAL_QUANTITIES], Invalid3[MAX_AC_LOCAL_QUANTITIES], Invalid4[MAX_AC_LOCAL_QUANTITIES];
	UINT16 SumInv[MAX_AC_LOCAL_QUANTITIES];

		SamplePos=SamplingVars.SamplePosition+AC_LP_HP_FILTER_ORDER-1;

		SampleWindow.TimeStamp[SamplingVars.SamplePosition] = Rcv->TimeStamp;
		//Possible phase shift
		if (Rcv->TimeStamp < 500 && SamplingVars.FreqInitCounter < TOTAL_BLOCK_FREQUENCY_AFTER_SHIFT){
				SamplingVars.FreqInitCounter=TOTAL_BLOCK_FREQUENCY_AFTER_SHIFT;
		}


		for(j=0; j<MAX_AC_LOCAL_QUANTITIES; j++) {
			//Substitution module
			Invalid1[j]=Invalid(Rcv->Qual[0][j]);
			Invalid2[j]=Invalid(Rcv->Qual[1][j]);
			Invalid3[j]=Invalid(Rcv->Qual[2][j]);
			Invalid4[j]=Invalid(Rcv->Qual[3][j]);
			SumInv[j] = Invalid1[j] + Invalid2[j] + Invalid3[j] + Invalid4[j];
			if(SumInv[j]){
				if(SumInv[j]==1){
					SumInv[j]=0;
					if(Invalid1[j]){
						Rcv->Smp[0][j] = 2.0f*(Rcv->Smp[1][j]) - Rcv->Smp[2][j];
						ForceValid(&(Rcv->Qual[0][j]));
						Invalid1[j]=FALSE;
					}else if(Invalid2[j]){
						Rcv->Smp[1][j] = 0.5f*(Rcv->Smp[0][j] + Rcv->Smp[2][j]);
						ForceValid(&(Rcv->Qual[1][j]));
						Invalid2[j]=FALSE;
					}else if(Invalid3[j]){
						Rcv->Smp[2][j] = 0.5f*(Rcv->Smp[1][j] + Rcv->Smp[3][j]);
						ForceValid(&(Rcv->Qual[2][j]));
						Invalid3[j]=FALSE;
					}else {//if(Invalid4[j]){
						Rcv->Smp[3][j] = 2.0f*(Rcv->Smp[2][j]) - Rcv->Smp[1][j];
						ForceValid(&(Rcv->Qual[3][j]));
						Invalid4[j]=FALSE;
					}
					Rcv->SmpAv[j]= (Rcv->Smp[0][j]+Rcv->Smp[1][j]+Rcv->Smp[2][j]+Rcv->Smp[3][j])>>2;
				}else{
					if(Invalid1[j])Rcv->Smp[0][j]=0;
					if(Invalid2[j])Rcv->Smp[1][j]=0;
					if(Invalid3[j])Rcv->Smp[2][j]=0;
					if(Invalid4[j])Rcv->Smp[3][j]=0;
					Rcv->SmpAv[j]= 0;
				}
			}
			//end of Substitution module
    }

		for(j=0; j<MAX_AC_LOCAL_QUANTITIES; j++) {

			//Raw Samples Window (S0)
			SampleWindow.LastCycleRaw[j][SamplingVars.RawSampleIndex] = Rcv->Smp[1][j] * ACQuantityConfig[j].RawPrimaryConversion +  ACQuantityConfig[j].Offset.PrBusOffset;
			//SampleWindow.LastCycleRawQual[j][SamplingVars.RawSampleIndex] = Rcv->Qual[1][j];

			//Ram Sample Window (S0 duplicated - RMS calculus)
			SampleWindow.LastCycleRaw[j][SamplingVars.RawSampleIndexNext] = Rcv->Smp[1][j] * ACQuantityConfig[j].RawPrimaryConversion +  ACQuantityConfig[j].Offset.PrBusOffset;
			//SampleWindow.LastCycleRawQual[j][SamplingVars.RawSampleIndexNext] = Rcv->Qual[1][j];

			//Raw Samples Window (S1)
			SampleWindow.LastCycleRaw[j][NextRawSampleIndex] = Rcv->Smp[3][j] * ACQuantityConfig[j].RawPrimaryConversion +  ACQuantityConfig[j].Offset.PrBusOffset;
			//SampleWindow.LastCycleRawQual[j][NextRawSampleIndex] = Rcv->Qual[3][j];

			//Ram Sample Window (S1 duplicated - RMS calculus)
			SampleWindow.LastCycleRaw[j][NextRMSRawIndex] = Rcv->Smp[3][j] * ACQuantityConfig[j].RawPrimaryConversion +  ACQuantityConfig[j].Offset.PrBusOffset;
			//SampleWindow.LastCycleRawQual[j][NextRMSRawIndex] = Rcv->Qual[3][j];

			//Historic window
			SampleWindow.Historic[j][SamplePos] = (Rcv->SmpAv[j]) * ACQuantityConfig[j].PrimaryConversion  +  ACQuantityConfig[j].Offset.PrBusOffset;
			SampleWindow.HistoricQual[j][SamplePos] = Merge4Qual(Rcv->Qual[0][j], Rcv->Qual[1][j], Rcv->Qual[2][j], Rcv->Qual[3][j]);
			if (SamplePos >= SAMPLE_WINDOW_SIZE){
					SampleWindow.Historic[j][SamplePos-SAMPLE_WINDOW_SIZE] = SampleWindow.Historic[j][SamplePos];
					SampleWindow.HistoricQual[j][SamplePos-SAMPLE_WINDOW_SIZE] = SampleWindow.HistoricQual[j][SamplePos];
			}

			//Last cycle window
			LastCycleLastSample = SampleWindow.LastCycle[j][SamplingVars.SampleIndex];
			LastCycleLastSampleInv = Invalid(SampleWindow.LastCycleQual[j][SamplingVars.SampleIndex]);
			
			SampleWindow.LastCycle[j][SamplingVars.SampleIndex] =
				Filter(&(SampleWindow.Historic[j][SamplingVars.SamplePosition]),
				FilterLPHP[ACQuantityConfig[j].FilterType]) * ACQuantityConfig[j].PrimaryConversionComp;
			SampleWindow.LastCycleQual[j][SamplingVars.SampleIndex] =
				Merge2Qual(SampleWindow.HistoricQual[j][SamplingVars.SamplePosition],
				SampleWindow.HistoricQual[j][SamplePos]);
			SampleWindow.GeneralQual[j][SamplingVars.GeneralQualIndex] =
				Merge2Qual((UINT16)(!SamplingVars.ResetGeneralQual)*SampleWindow.GeneralQual[j][SamplingVars.GeneralQualIndex],
				SampleWindow.LastCycleQual[j][SamplingVars.SampleIndex]);

			// Fault detector
			SampleWindow.LastCycle[j][SamplingVars.SampleIndex + ONE_CYCLE_WINDOW]=
				SampleWindow.LastCycle[j][SamplingVars.SampleIndex];
			if(SamplingVars.FaultDetectorActive && ACQuantityConfig[j].FaultDetectorActive
				&& !LastCycleLastSampleInv && !LastCycleSampleIndexInv){// ACInput can trip fault dector

				DeltaSample = fabs(SampleWindow.LastCycle[j][SamplingVars.SampleIndex] - LastCycleLastSample);

				//Fault detector start
				if(SamplingVars.FaultDetectorStatus < QUARTER_CYCLE_WINDOW) {
					if (DeltaSample > ACQuantityConfig[j].DeltaFaultDetectorStart && (ACQuantityConfig[j].FDFrequencySecurity
							|| DeltaSample > ACQuantityConfig[j].pGroupPhase->Magnitude * FAULT_DETECTION_SLOP)) {
						if(SamplingVars.StartFilterDelay == 0 && SamplingVars.FaultDetectorStatus==0 ){
							SamplingVars.StartFilterDelay = AC_LP_HP_FILTER_ORDER + 1;//For stability of filter response
							SamplingVars.FreqInitCounter = TOTAL_BLOCK_FREQUENCY_AFTER_FAULT;//Block frequency calculus for 3 cycles after fault
							SamplingVars.AwakeCounter = BASE_RUN_TASK + AC_LP_HP_FILTER_ORDER;
						}
						ACQuantityConfig[j].FaultDetected=TRUE;
						SamplingVars.StabilityCounter=0;
					}
				//Fault destector reset
				}else if (DeltaSample > ACQuantityConfig[j].DeltaFaultDetectorReset && (ACQuantityConfig[j].FDFrequencySecurity
						|| DeltaSample >ACQuantityConfig[j].pGroupPhase->Magnitude * FAULT_STABILITY_SLOP)) {
					SamplingVars.StabilityCounter=0;
				}
			}
			//End of fault detector

            /***** ZeroCross Orig. at 4kHz Optimized *****/

            if (LastCycleSampleIndexInv) {
                // last sample index is not tested for validity because the TOTAL_BLOCK_FREQUENCY_AFTER_INV
                // ensures several samples valid before evaluation.
                ACQuantityConfig[j].InputFreqInit = TOTAL_BLOCK_FREQUENCY_AFTER_INV;
                ACQuantityConfig[j].FreezeFreqUpdate = FREEZE_FREQ_UPDATE_COUNT_AFTER_INV;
                ACQuantityConfig[j].Period = INVALID_PERIOD;

				
			}

            MultCheckSamples[0] = (FLOAT32)Rcv->Smp[0][j] * SamplingVars.LastSample4k[j];
            MultCheckSamples[1] = (FLOAT32)Rcv->Smp[1][j] * Rcv->Smp[0][j];
            MultCheckSamples[2] = (FLOAT32)Rcv->Smp[2][j] * Rcv->Smp[1][j];
            MultCheckSamples[3] = (FLOAT32)Rcv->Smp[3][j] * SamplingVars.LastSample4k[j];

            if ((ACQuantityConfig[j].HalfCycleCounter > THREE_HALF_CYCLE_WINDOW) && (MultCheckSamples[3] <= 0.0f)) {
                // Scan for Zero - from head to tail to count the samples (zero location)
                if (MultCheckSamples[0] <= 0.0f) {
                    ZeroCrossSample_Pointer = 0;
                    ZerosCrossSamples[0] = SamplingVars.LastSample4k[j];
                } else {
                    if (MultCheckSamples[1] <= 0.0f) {
                        ZeroCrossSample_Pointer = 1;
                    } else {
                        if (MultCheckSamples[2] <= 0.0f) {
                            ZeroCrossSample_Pointer = 2;
                        } else {
                            ZeroCrossSample_Pointer = 3;
                        }
                    }
                    ZerosCrossSamples[0] =
                        (FLOAT32)Rcv->Smp[ZeroCrossSample_Pointer - 1][j];  // Sample value at Index-1
                }

                ZerosCrossSamples[1] = (FLOAT32)Rcv->Smp[ZeroCrossSample_Pointer][j];  // Sample value at Index

                if (ZerosCrossSamples[0] == ZerosCrossSamples[1]) {
                    ACQuantityConfig[j].HalfCycleCounter += 4;  // Skip: zeros = add 4 samples to avoid NaN and inf
                } else {
                    ZeroCrossSample_Pointer++;                                          // Index starts with 0
                    ACQuantityConfig[j].HalfCycleCounter += (ZeroCrossSample_Pointer);  // Increment the samples counter

                    ACQuantityConfig[j].OneCycleCounter +=
                        ACQuantityConfig[j].HalfCycleCounter;  // Accumulate on the prev. halfcycle (to full cylce)

                    InterpoledZeroTime = (ZerosCrossSamples[1]) /
                                         ((ZerosCrossSamples[1]) - (ZerosCrossSamples[0]));  // Calc. Linear correction

                    NewPeriod = ACQuantityConfig[j].OneCycleCounter + ACQuantityConfig[j].OneCycleBeforeZeroTime -
                                InterpoledZeroTime;

                    AveragePeriod = 1/3 *
                                    (NewPeriod + ACQuantityConfig[j].PeriodMem[0] + ACQuantityConfig[j].PeriodMem[1]);

                    DeltaNewPeriod = (AveragePeriod - SamplingVars.PeriodMemMovAvg[j][!ACQuantityConfig[j].MemIndex]);
                    // fabs
                    if (ACQuantityConfig[j].FreezeFreqUpdate < FREEZE_FREQ_UPDATE_COUNT &&
                        fabs(DeltaNewPeriod) > MAX_PERIOD_VARIATION &&
                        fabs(DeltaNewPeriod /
                             (1.0f - (SamplingVars.PeriodMemMovAvg[j][!ACQuantityConfig[j].MemIndex] -
                                      SamplingVars.PeriodMemMovAvg[j][ACQuantityConfig[j].MemIndex]))) >
                            MAX_DELTA_PERIOD_VARIATION) {
                        ACQuantityConfig[j].FreezeFreqUpdate = FREEZE_FREQ_UPDATE_COUNT;
                    }

                    // Frequency is updated from rated frequency during SamplingVars.FreqInitCounter provided
                    // it is stable - but the applications are blocked from using frequency
                    if (!ACQuantityConfig[j].FreezeFreqUpdate) {
 
                        // **Compensation HP for the currents at this point:
                        // - Old Period is ACQuantityConfig[j].ValidPeriod in seconds
                        // - New period is AveragePeriod in samples
                        if (!ACQuantityConfig[j].InputFreqInit) {
                            if (ACQuantityConfig[j].ValidPeriod == INVALID_PERIOD &&
                                ACQuantityConfig[j].FreqBckUpIdx != INVALID_IDX) {
                                ACQuantityConfig[j].PrimaryConversionComp =
                                    ACQuantityConfig[ACQuantityConfig[j].FreqBckUpIdx].PrimaryConversionComp;
                            } else {
                                FilterMgnComp(ACQuantityConfig[j].Type, (AveragePeriod * ONE2FOUR_KHZ), ACQuantityConfig[j].ValidPeriod,
                                              &(ACQuantityConfig[j].PrimaryConversionComp));
                            }
                        }
                        //**End of compensation HP for the currents

                        // Avoiding change on SamplingVars.SamplingPeriod due to its great impact on other files.
                        ACQuantityConfig[j].Period = (AveragePeriod * ONE2FOUR_KHZ) * SamplingVars.SamplingPeriod;
                        ACQuantityConfig[j].ValidPeriod = ACQuantityConfig[j].Period;
                        ACQuantityConfig[j].InvValidPeriod = COUNT_INV_VALID_PERIOD;

                        // Fault detector security level decision
                        if (ACQuantityConfig[j].Period > SamplingVars.FaultDetectorFreqLimit[2] &&
                            ACQuantityConfig[j].Period < SamplingVars.FaultDetectorFreqLimit[1]) {
                            ACQuantityConfig[j].FDFrequencySecurity = TRUE;
                        } else if (ACQuantityConfig[j].Period > SamplingVars.FaultDetectorFreqLimit[0] ||
                                   ACQuantityConfig[j].Period < SamplingVars.FaultDetectorFreqLimit[3]) {
                            ACQuantityConfig[j].FDFrequencySecurity = FALSE;
                        }

                    } else {
                        ACQuantityConfig[j].FreezeFreqUpdate--;
                    }

                    ACQuantityConfig[j].PeriodMem[ACQuantityConfig[j].MemIndex] = NewPeriod;
                    SamplingVars.PeriodMemMovAvg[j][ACQuantityConfig[j].MemIndex] = AveragePeriod;

                    ACQuantityConfig[j].MemIndex = !(ACQuantityConfig[j].MemIndex);

                    ACQuantityConfig[j].OneCycleBeforeZeroTime = ACQuantityConfig[j].HalfCycleBeforeZeroTime;
                    ACQuantityConfig[j].HalfCycleBeforeZeroTime = InterpoledZeroTime;
                    ACQuantityConfig[j].OneCycleCounter = ACQuantityConfig[j].HalfCycleCounter;

                    ACQuantityConfig[j].HalfCycleCounter =
                        (4 - ZeroCrossSample_Pointer);  // add the remaining samples (after the zero cross)
                }
            } else {
                ACQuantityConfig[j].HalfCycleCounter += 4;  // No zero = add 4 samples
            }

            // Init freq counter by input
            if (ACQuantityConfig[j].InputFreqInit) {
                ACQuantityConfig[j].InputFreqInit--;
            }

            SamplingVars.LastSample4k[j] = Rcv->Smp[3][j];  // Keep the last sample of the current iteration

            // inv period memory
            if (ACQuantityConfig[j].InvValidPeriod) {
                ACQuantityConfig[j].InvValidPeriod--;
            } else {
                ACQuantityConfig[j].ValidPeriod = INVALID_PERIOD;
            }

			// end of frequency estimation

            /***** ZeroCross Orig. at 4kHz Optimized *****/

        }

		//Check derived sample calculus
		if(SamplingVars.UsedSampleBuffers){
			DerivedSampleCalculus(SamplePos, NextRawSampleIndex, RawSampleIndexNext, NextRMSRawIndex);
		}

		// Fault detection general decisions:
		if(SamplingVars.StartFilterDelay){//For stability of filter response
			SamplingVars.StartFilterDelay--;
			if(SamplingVars.StartFilterDelay==0){
				msg = FAULT_DETECTED;
				SamplingVars.FaultDetectorHistoricSample = SamplingVars.SamplePosition;
				SamplingVars.FaultDetectorSampleIndex = SamplingVars.SampleIndex;
				SamplingVars.FaultDetectorStatus = 1;
			}
		}else if (SamplingVars.FaultDetectorStatus) {
			if(SamplingVars.FaultDetectorStatus<FAULT_DETECTOR_MAX_TIME){
				SamplingVars.FaultDetectorStatus++;
			}
			SamplingVars.StabilityCounter++;
			//reset fault detector
			if(SamplingVars.StabilityCounter==FAULT_DETECTOR_STABILITY_COUNT){
				SamplingVars.StabilityCounter=0;
				for(j=0; j<MAX_AC_LOCAL_QUANTITIES; j++) {
					ACQuantityConfig[j].FaultDetected=FALSE;
				}
				SamplingVars.FaultDetectorStatus=0;
				SamplingVars.FaultDetectorHistoricSample = 0;
				SamplingVars.FaultDetectorSampleIndex = 0;
				msg = FAULT_RESET;
			}
		}

		//Init freq counter
		if(SamplingVars.FreqInitCounter){
			SamplingVars.FreqInitCounter--;
		}

		// Increment raw sample index (sample position in raw sample buffer)
		SamplingVars.RawSampleIndex+=2;
		SamplingVars.RawSampleIndexNext+=2;
		if (SamplingVars.RawSampleIndex == LAST_CYCLE_RAW_WINDOW_SIZE) {
			SamplingVars.RawSampleIndex = 0;
			SamplingVars.RawSampleIndexNext = LAST_CYCLE_RAW_WINDOW_SIZE;
		}
		// Increment sample index (sample position relative to one cycle)
		SamplingVars.LastSampleIndex = SamplingVars.SampleIndex;
		SamplingVars.SampleIndex++;
		if (SamplingVars.SampleIndex == ONE_CYCLE_WINDOW) {
			SamplingVars.SampleIndex = 0;
		}

		// Increment sample position in historic sample window
		SamplingVars.LastSamplePosition = SamplingVars.SamplePosition;
		SamplingVars.SamplePosition++;
		if (SamplingVars.SamplePosition == SAMPLE_WINDOW_SIZE) {
			SamplingVars.SamplePosition = 0;
		}

		SamplingVars.ResetGeneralQual = FALSE;


		if (--SamplingVars.AwakeCounter == 0) {
			SamplingVars.AwakeCounter = BASE_RUN_TASK;
			
			msg = NORMAL_RUN;
			SamplingVars.ResetGeneralQual = TRUE;
			SamplingVars.GeneralQualIndex++;
			if(SamplingVars.GeneralQualIndex == MAX_GENERAL_QUAL_ELEM){
				SamplingVars.GeneralQualIndex=0;
			}
            return TRUE;
		}
    
    return FALSE;

}


 void DerivedSampleCalculus(INT32 SamplePos, INT32 NextRawSampleIndex, INT32 RawSampleIndexNext, INT32 NextRMSRawIndex){
	INT32 i;

	/***************** Calculate Residual Samples *******************************/
	for(i = 0; i < SamplingVars.UsedSampleBuffers; i++){
		switch(DerivedSamples[i].Type){

			case DSC_TYPE_RESIDUAL_FROM_BASE:	//Residual (RES = A + B + C)
				SampleWindow.LastCycleDerived[i][SamplingVars.SampleIndex]=
				SampleWindow.LastCycle[DerivedSamples[i].FirstParcel][SamplingVars.SampleIndex]+
				SampleWindow.LastCycle[DerivedSamples[i].SecondParcel][SamplingVars.SampleIndex]+
				SampleWindow.LastCycle[DerivedSamples[i].ThirdParcel][SamplingVars.SampleIndex];

/*				SampleWindow.LastCycleDerivedQual[i][SamplingVars.SampleIndex]= Merge3Qual(
				SampleWindow.LastCycleQual[DerivedSamples[i].FirstParcel][SamplingVars.SampleIndex],
				SampleWindow.LastCycleQual[DerivedSamples[i].SecondParcel][SamplingVars.SampleIndex],
				SampleWindow.LastCycleQual[DerivedSamples[i].ThirdParcel][SamplingVars.SampleIndex]);*/

				SampleWindow.HistoricDerived[i][SamplingVars.SamplePosition]=
				SampleWindow.Historic[DerivedSamples[i].FirstParcel][SamplePos]+
				SampleWindow.Historic[DerivedSamples[i].SecondParcel][SamplePos]+
				SampleWindow.Historic[DerivedSamples[i].ThirdParcel][SamplePos];

				SampleWindow.HistoricDerivedQual[i][SamplingVars.SamplePosition]= Merge3Qual(
				SampleWindow.HistoricQual[DerivedSamples[i].FirstParcel][SamplePos],
				SampleWindow.HistoricQual[DerivedSamples[i].SecondParcel][SamplePos],
				SampleWindow.HistoricQual[DerivedSamples[i].ThirdParcel][SamplePos]);

				// For the raw buffer we must proceess two samples for each run.
				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][SamplingVars.RawSampleIndex]=
				SampleWindow.LastCycleRaw[DerivedSamples[i].FirstParcel][SamplingVars.RawSampleIndex]+
				SampleWindow.LastCycleRaw[DerivedSamples[i].SecondParcel][SamplingVars.RawSampleIndex]+
				SampleWindow.LastCycleRaw[DerivedSamples[i].ThirdParcel][SamplingVars.RawSampleIndex];

				/*SampleWindow.LastCycleRawQual[i+MAX_AC_LOCAL_QUANTITIES][SamplingVars.RawSampleIndex]=Merge3Qual(
				SampleWindow.LastCycleRawQual[DerivedSamples[i].FirstParcel][SamplingVars.RawSampleIndex],
				SampleWindow.LastCycleRawQual[DerivedSamples[i].SecondParcel][SamplingVars.RawSampleIndex],
				SampleWindow.LastCycleRawQual[DerivedSamples[i].ThirdParcel][SamplingVars.RawSampleIndex]);*/

				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][NextRawSampleIndex]=
				SampleWindow.LastCycleRaw[DerivedSamples[i].FirstParcel][NextRawSampleIndex]+
				SampleWindow.LastCycleRaw[DerivedSamples[i].SecondParcel][NextRawSampleIndex]+
				SampleWindow.LastCycleRaw[DerivedSamples[i].ThirdParcel][NextRawSampleIndex];

				/*SampleWindow.LastCycleRawQual[i+MAX_AC_LOCAL_QUANTITIES][NextRawSampleIndex]=Merge3Qual(
				SampleWindow.LastCycleRawQual[DerivedSamples[i].FirstParcel][NextRawSampleIndex],
				SampleWindow.LastCycleRawQual[DerivedSamples[i].SecondParcel][NextRawSampleIndex],
				SampleWindow.LastCycleRawQual[DerivedSamples[i].ThirdParcel][NextRawSampleIndex]);*/

				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][RawSampleIndexNext]=
				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][SamplingVars.RawSampleIndex];

				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][NextRMSRawIndex]=
				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][NextRawSampleIndex];
				break;
			
			case DSC_TYPE_VOLTAGE_PN2PP: // Voltage Phase-Phase (AB = A - B)
				SampleWindow.LastCycleDerived[i][SamplingVars.SampleIndex]=
				SampleWindow.LastCycle[DerivedSamples[i].FirstParcel][SamplingVars.SampleIndex]-
				SampleWindow.LastCycle[DerivedSamples[i].SecondParcel][SamplingVars.SampleIndex];

				/*SampleWindow.LastCycleDerivedQual[i][SamplingVars.SampleIndex]=Merge2Qual(
				SampleWindow.LastCycleQual[DerivedSamples[i].FirstParcel][SamplingVars.SampleIndex],
				SampleWindow.LastCycleQual[DerivedSamples[i].SecondParcel][SamplingVars.SampleIndex]);*/

				SampleWindow.HistoricDerived[i][SamplingVars.SamplePosition]=
				SampleWindow.Historic[DerivedSamples[i].FirstParcel][SamplePos]-
				SampleWindow.Historic[DerivedSamples[i].SecondParcel][SamplePos];

				SampleWindow.HistoricDerivedQual[i][SamplingVars.SamplePosition]=Merge2Qual(
				SampleWindow.HistoricQual[DerivedSamples[i].FirstParcel][SamplePos],
				SampleWindow.HistoricQual[DerivedSamples[i].SecondParcel][SamplePos]);

				// For the raw buffer we must proceess two samples for each run.
				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][SamplingVars.RawSampleIndex]=
				SampleWindow.LastCycleRaw[DerivedSamples[i].FirstParcel][SamplingVars.RawSampleIndex]-
				SampleWindow.LastCycleRaw[DerivedSamples[i].SecondParcel][SamplingVars.RawSampleIndex];

				/*SampleWindow.LastCycleRawQual[i+MAX_AC_LOCAL_QUANTITIES][SamplingVars.RawSampleIndex]=Merge2Qual(
				SampleWindow.LastCycleRawQual[DerivedSamples[i].FirstParcel][SamplingVars.RawSampleIndex],
				SampleWindow.LastCycleRawQual[DerivedSamples[i].SecondParcel][SamplingVars.RawSampleIndex]);*/

				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][NextRawSampleIndex]=
				SampleWindow.LastCycleRaw[DerivedSamples[i].FirstParcel][NextRawSampleIndex]-
				SampleWindow.LastCycleRaw[DerivedSamples[i].SecondParcel][NextRawSampleIndex];

				/*SampleWindow.LastCycleRawQual[i+MAX_AC_LOCAL_QUANTITIES][NextRawSampleIndex]=Merge2Qual(
				SampleWindow.LastCycleRawQual[DerivedSamples[i].FirstParcel][NextRawSampleIndex],
				SampleWindow.LastCycleRawQual[DerivedSamples[i].SecondParcel][NextRawSampleIndex]);*/

				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][RawSampleIndexNext]=
				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][SamplingVars.RawSampleIndex];

				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][NextRMSRawIndex]=
				SampleWindow.LastCycleRaw[i+MAX_AC_LOCAL_QUANTITIES][NextRawSampleIndex];
				break;

			default://No calculation required because it is not required.
				break;
		}
	}
	/*******************************************************************/

}


void FilterMgnComp(UINT8 Type, FLOAT32 NewPeriod , FLOAT32 OldPeriod, FLOAT32* PrimaryConversionComp){
	FLOAT32 AuxDelta=OldPeriod/SamplingVars.SamplingPeriod - NewPeriod;
	if(Type==AC_CURRENT_INPUT && NewPeriod > MIN_PERIOD_COMPENSABLE && NewPeriod < MAX_PERIOD_COMPENSABLE 
			&& AuxDelta < MAX_SAMPLES_VARIATION_PER_HALF_CYCLE && AuxDelta > -MAX_SAMPLES_VARIATION_PER_HALF_CYCLE){
		*PrimaryConversionComp = HighPassMgnComp[0]*NewPeriod+HighPassMgnComp[1];
	}else {
		*PrimaryConversionComp = 1.0f;
	}
	/*else{
		ConfigPtr->PrimaryConversionComp = 1;
	}*/
	//the value is maintained if the magnitude is high enough(the reset is made in after estimation)
	// this avoids variations during frequency re-calculus, when fault is detected or phase shift occurs and frequency is not nominal 
}

void CalculateAC()
{
	ACQUANTITYCONFIG *pConfig;
	FLOAT32 *pSamples=&(SampleWindow.LastCycle[0][0]);
	FLOAT32 UsedOrtCoef[3]; 	//Ortogonalization of phaselets
	FLOAT32 *pCos_Sin=&(Cos_Sin[0][0]);	//Matriz of con and -sen
	UINT8 quantity = 0;
	UINT8 i = MAX_GROUPS;
	INT32 Status;
	INTERNALGROUP* pGroup;
	PHASOR* pPhasor = PhasorBuffer;
	UINT16* pInpQual = EstimationVars.InpQual;
	UINT8 CorrIndex;
	FLOAT32 ReCorrFactor, ImCorrFactor, ReCorrected;
	UINT8 RunFastDft = FALSE;
    int _CalculateRMS=0;
	Status = SamplingVars.FaultDetectorStatus;

    if (Status >= QUARTER_CYCLE_WINDOW && Status < ONE_CYCLE_WINDOW) {// 5->19
        pSamples = &(SampleWindow.LastCycle[0][SamplingVars.FaultDetectorSampleIndex]);
        pCos_Sin = &(Cos_Sin[SamplingVars.FaultDetectorSampleIndex][0]);
        memcpy(&(UsedOrtCoef[0]),
               &(__OrthogCoef[SamplingVars.FaultDetectorSampleIndex][Status - QUARTER_CYCLE_WINDOW][0]),
               sizeof(UsedOrtCoef));
        RunFastDft = TRUE;
    }

	pConfig = &ACQuantityConfig[0];

	while (quantity < MAX_AC_LOCAL_QUANTITIES) {
		//For now the fast DFT still have a general quality merged from the last 1.25 cycles
		//ensuring safety for all possible calculations including RMS at max period (1.2 cycles)
		*pInpQual = Merge5Qual(&(SampleWindow.GeneralQual[quantity][0]));

        if (!RunFastDft){
            *pPhasor = Dft20(pSamples);
        } else {// RunFastDft
            *pPhasor = FastDft(pSamples, pCos_Sin, &(UsedOrtCoef[0]), Status);
        }

		pPhasor->Magnitude = Magnitude(pPhasor);

		pPhasor->Period = pConfig->Period;
		
		if (pConfig->pGroupPhase) {
            *pConfig->pGroupPhase = *pPhasor;
        }

		pSamples += LAST_CYCLE_WINDOW_SIZE;
		pConfig++;
		quantity++;
		pPhasor++;
		pInpQual++;
	}

	// Calculate RMS and update RMS calculation counter
    if(_CalculateRMS == 3){
        _CalculateRMS = 0;
        for(i = 0; i < MAX_AC_LOCAL_QUANTITIES ; i++) {
            RootMeanSquare(i);
        }
    }else{
        _CalculateRMS++;
    }


    //debug
    /*if((++_Debug%100)==0){
        printf("==\n");
        for(i = 0; i < MAX_AC_LOCAL_QUANTITIES ; i++) {
            printf("Phasor %d\n Mag=%f, Re=%f, Im=%f, Per=%f, rms=%f \n",i, PhasorBuffer[i].Magnitude, PhasorBuffer[i].Re, PhasorBuffer[i].Im, PhasorBuffer[i].Period, PhasorBuffer[i].Rms);
            for(int j = 0; j < 20 ; j++) {
                //printf("%f ", SampleWindow.LastCycle[i][j]);
            }
        }
        printf("==\n");
    }*/

}

void RootMeanSquare(int i){
	int idx = 0;
	float rms = 0;
	float dc=0;
	int size = 40;
	float rmsCompensated;

		
    size=round(50*(ACQuantityConfig[i].pGroupPhase->Period));
    
    if(size < 32){
        if(size < 31){
            size = 40;
        }else {
            size = 32;//Frequency DB
        }
    }
    if(size > 48){
        if(size > 49){
            size = 40;
        }else {
            size = (48);//Frequency DB
        }										
    }

	const float* samp = ( &(SampleWindow.LastCycleRaw[i][0]) + SamplingVars.RawSampleIndexNext - size);

    printf("%f ", *samp);
	for(idx = 0; idx < size; idx++){
		rms += samp[idx]*samp[idx];
		dc += samp[idx];
	}
	size-=32;
	
	dc*=RmsFactorDC[size];
	dc*=dc;
	rmsCompensated = RmsFactor[size]*rms - dc;
	if(rmsCompensated>0){
		ACQuantityConfig[i].pGroupPhase->Rms = sqrt(rmsCompensated);
	}else {
		ACQuantityConfig[i].pGroupPhase->Rms = 0;
	}

    PhasorBuffer[i].Rms = ACQuantityConfig[i].pGroupPhase->Rms;

}

PHASOR Dft20(FLOAT32 *pSamples)
{
	FLOAT32 TempGroup[10];
	PHASOR Result;

    TempGroup[0] = pSamples[0] - pSamples[10];
    TempGroup[1] = pSamples[1] - pSamples[9] - pSamples[11] + pSamples[19];
    TempGroup[2] = pSamples[2] - pSamples[8] - pSamples[12] + pSamples[18];
    TempGroup[3] = pSamples[3] - pSamples[7] - pSamples[13] + pSamples[17];
    TempGroup[4] = pSamples[4] - pSamples[6] - pSamples[14] + pSamples[16];
    TempGroup[5] = pSamples[5] - pSamples[15];
    TempGroup[6] = pSamples[1] + pSamples[9] - pSamples[11] - pSamples[19];
    TempGroup[7] = pSamples[2] + pSamples[8] - pSamples[12] - pSamples[18];
    TempGroup[8] = pSamples[3] + pSamples[7] - pSamples[13] - pSamples[17];
    TempGroup[9] = pSamples[4] + pSamples[6] - pSamples[14] - pSamples[16];

	Result.Re = K20 * TempGroup[0] + (mA20 * TempGroup[1]) + (mB20 * TempGroup[2]) +
							   (mC20 * TempGroup[3]) + (mD20 * TempGroup[4]);
	Result.Im = K20 * TempGroup[5] + (mE20 * TempGroup[6]) + (mF20 * TempGroup[7]) +
							   (mG20 * TempGroup[8]) + (mH20 * TempGroup[9]);
	Result.Im = - Result.Im;

	return Result;
}

PHASOR FastDft(FLOAT32 *pSamples,FLOAT32 *pCos_Sen, FLOAT32 *pCoefOrth, UINT8 Samples){
        PHASOR Result;
        FLOAT32 Re=0.0f;
        FLOAT32 Im=0.0f;
        while (Samples-- > 0){
            Re+=(*pSamples) * (*pCos_Sen);
            pCos_Sen++;
            Im+=(*pSamples) * (*pCos_Sen);
            pCos_Sen++;
            pSamples++;
        }
        Result.Re=Re * pCoefOrth[0] + Im * pCoefOrth[1];
        Result.Im=Re * pCoefOrth[1] + Im * pCoefOrth[2];  
        return Result;
}




