#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <Eigen/Dense>
#include "Circuit.h"

using namespace std;
using namespace Eigen;

void calculation_MOS(const MOSINFO& mos, const ModelINFO& model, double Vgs, double Vds, double& gm, double& gds )
{
	if(mos.MOStype == "n")
	{
		if(Vgs <= model.VT)
		{
			//截止区
			gm = 0.0;
			gds = 0.0;
		}
		else if(Vds <= Vgs- model.VT)
		{
			//线性区
			gm = model.MU * model.COX * (mos.MOSW / mos.MOSL) * Vds;
			gds = model.MU * model.COX * (mos.MOSW / mos.MOSL) * (Vgs - Vds - model.VT);
		}
		else
		{
			gm = model.MU * model.COX * (mos.MOSW / mos.MOSL) * (Vgs - model.VT) * (1 + model.LAMBDA * Vds);
			gds = model.LAMBDA * 0.5 * model.MU * model.COX * (mos.MOSW / mos.MOSL) * pow(Vgs - model.VT, 2);
		}
	}
	else if(mos.MOStype == "p")
	{
		Vgs = -Vgs;
		Vds = -Vds;
		if(Vgs <= -model.VT)
		{
			gm = 0.0;
			gds = 0.0;
		}
		else if(Vds <= Vgs + model.VT)
		{
			gm = model.MU * model.COX * (mos.MOSW / mos.MOSL) * Vds;
			gds = model.MU * model.COX * (mos.MOSW / mos.MOSL) * (Vgs - Vds + model.VT);
		}
		else
		{
			gm = model.MU * model.COX * (mos.MOSW / mos.MOSL) * (Vgs + model.VT) * (1 + model.LAMBDA * Vds);
			gds = model.LAMBDA * 0.5 * model.MU * model.COX * (mos.MOSW / mos.MOSL) * pow(Vgs + model.VT, 2);
		}
	}
}

void GenMatrix(
	const CircuitData& circuitdata,
	int nodeNum,
	MatrixXd& A,
	VectorXd& b,
	vector<string>& x
)
{
	int nodeNums = nodeNum + 1;  // 包含接地节点
	int currentVarIndex = nodeNums;  // 当前额外变量索引
	int INum = 0;

	// 初始化矩阵和向量
	A = MatrixXd::Zero(nodeNums, nodeNums);
	b = VectorXd::Zero(nodeNums);
	x.resize(nodeNums);
	for (int i = 0; i < nodeNums; ++i) {
		x[i] = "v_" + to_string(i);
	}
	//对于电阻
	for(const auto& r : circuitdata.rinfo)
	{
		int pNum1 = stoi(r.N1) + 1;
		int pNum2 = stoi(r.N2) + 1;
		const string& name = r.Name;
		double value = r.Value;
		A(pNum1, pNum1) += 1.0 / value;
		A(pNum1, pNum2) -= 1.0 / value;
		A(pNum2, pNum1) -= 1.0 / value;
		A(pNum2, pNum2) += 1.0 / value;
	}
	for(const auto& source:circuitdata.sourceinfo)
	{
		double svalue = source.SourceDcValue;
		x.push_back("I_" + source.SourceName);
		INum++;
		A.conservativeResize(nodeNums + INum, nodeNums + INum);
		b.conservativeResize(nodeNums + INum);
		A(nodeNums + INum - 1, nodeNums + INum - 1) = 0;
		b(nodeNums + INum - 1) = 0;

		int pNum1 = stoi(source.SourceN1) + 1;
		int pNum2 = stoi(source.SourceN2) + 1;

		A(pNum1, nodeNums + INum - 1) += 1.0;
		A(pNum2, nodeNums + INum - 1) -= 1.0;
		A(nodeNums + INum - 1, pNum1) += 1.0;
		A(nodeNums + INum - 1, pNum2) -= 1.0;
		b(nodeNums + INum - 1) = source.SourceDcValue;
	}
	for(const auto& l: circuitdata.linfo)
	{
		x.push_back("I_" + l.Name);
		INum++;
		A.conservativeResize(nodeNums + INum, nodeNums + INum);
		b.conservativeResize(nodeNums + INum);
		A(nodeNums + INum - 1, nodeNums + INum - 1) = 0;
		b(nodeNums + INum - 1) = 0;

		int pNum1 = stoi(l.N1) + 1;
		int pNum2 = stoi(l.N2) + 1;
		A(pNum1, nodeNums + INum - 1) += 1.0;
		A(pNum2, nodeNums + INum - 1) -= 1.0;
		A(nodeNums + INum - 1, pNum1) += 1.0;
		A(nodeNums + INum - 1, pNum2) -= 1.0;
		b(nodeNums + INum - 1) = 0.0;  // 电压源为 0
	}
	for(const auto& cv:)
}
