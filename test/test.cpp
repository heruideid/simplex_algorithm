#include <iostream>
#include<set>
#include<vector>
#include<fstream>
//#define DEBUG
const double INF = 1e8;
const int maxn = 100;
using namespace std;

struct slack_form {
	set<int> N;
	set<int> B;
	vector<vector<double>> A;
	vector<double> b;
	vector<double> c;
	double v;
	int m, n;
	slack_form(istream& fin) {
		fin >> m >> n;
		for (int i = 0; i < m + n + 1; i++) A.push_back(vector<double>());
		for (auto& v : A) {
			for (int i = 0; i < n+m+1; i++) v.push_back(0);
		}

		for (int i = 0; i < m + n + 1; i++) b.push_back(0);
		for (int i = 0; i < m + n + 1; i++) c.push_back(0);

		for (int i = 1; i <= n; i++)	N.insert(i);
		for (int i = 1; i <= m; i++) B.insert(i + n);
		for (int i = 1; i <= m; i++) {
			for (int j = 1; j <= n; j++) 	fin >> A[n + i][j];
			fin >> b[n + i];
		}
		for (int i = 1; i <= n; i++) 	fin >> c[i];
		v = 0;
	}
};

void Pivot(set<int>&N, set<int>& B, 
	vector<vector<double>>& A, vector<double>& b, 
	vector<double>& c, double& v, int l, int e) {
	b[e] = b[l] / A[l][e];
	for (auto& j : N) {
		if (j != e) A[e][j] = A[l][j] / A[l][e];
	}
	A[e][l] = 1 / A[l][e];
	//
	for (auto& i : B) {
		if (i != l) {
			b[i] = b[i] - A[i][e] * b[e];
			for (auto& j : N) {
				if (j != e)	A[i][j] = A[i][j] - A[i][e] * A[e][j];
			}
			A[i][l] = -A[i][e] * A[e][l];
		}
	}
	//
	v = v + c[e] * b[e];
	for (auto& j : N) {
		if (j != e) c[j] = c[j] - c[e] * A[e][j];
	}
	c[l] = -c[e] * A[e][l];
	//
	N.erase(e);
	N.insert(l);
	B.erase(l);
	B.insert(e);
}

int find_argmin_in_delt(set<int>&B, vector<double>& delt) {
	if (B.empty()) throw exception("B is empty!");
	int ans = -1;
	double value = INF;
	for (auto& index_num : B) {
		if (delt[index_num] <= value) {
			ans = index_num;
			value = delt[index_num];
		}
	}
	return ans;
}

int find_argmin_in_b(int m,int n,vector<double>& b) {
	int ans = 1+n;
	double value = b[1+n];
	for (int i = 1 + n; i <= n + m; i++) {
		if (b[i] < b[ans]) {
			value = b[i];
			ans = i;
		}
	}
	return ans;
}


int Select_e(vector<double>& c, set<int>& N) {
	for (auto index_num : N) {
		if (c[index_num] > 0) return index_num;
	}
	return -1;
}

inline bool exist_lawful_e(int e) {
	return e != -1;
}

int get_number_in_N(set<int>& N) {
	if (N.empty()) throw exception("N is empty");
	for (auto number : N) {
		return number;
	}
	throw exception("unknow error");
}

void print(slack_form& new_form) {
	for (auto& num : new_form.B) {
		cout << "x" << num << "=" << new_form.b[num];
		for (int i = 0; i <= new_form.n + new_form.m; i++) {
			if (new_form.A[num][i] != 0&&
				new_form.N.find(i)!=new_form.N.end()) {
				if (new_form.A[num][i] > 0) {
					cout << "-" << new_form.A[num][i] << "*x" << i;
				}
				else cout << "+" << -new_form.A[num][i] << "*x" << i;
			}
		}
		cout << endl;
	}
	cout << "obj=";
	for (int i = 0; i <= new_form.n + new_form.m; i++) {
		if (new_form.c[i] != 0&&
			new_form.N.find(i)!=new_form.N.end()) {
			cout << "+" << new_form.c[i] << "*x" << i;
		}
	}
	cout << endl;
}

void update_form(slack_form& form,slack_form& new_form) {
#ifdef DEBUG
	cout << "new_form is:\n";
	print(new_form);
#endif
	for (int i = 1; i < form.m + form.n + 1; i++) new_form.c[i] = form.c[i];
	new_form.v = form.v;
	for (int i = 1; i <= form.n; i++) {
		if (new_form.B.find(i) != new_form.B.end()) {
			for (auto& j : new_form.N)
				new_form.c[j] += new_form.c[i] * new_form.A[i][j];
			new_form.v += new_form.c[i] * new_form.b[i];
			new_form.c[i] = 0;
		}
	}
	//clean x0's trace
	new_form.N.erase(0);
	for (int i = 0; i <= new_form.m + new_form.n; i++) {
		new_form.A[i][0] = 0;
		new_form.A[0][i] = 0;
	}
	new_form.b[0] = new_form.c[0]=0;
	//
	form = new_form;
#ifdef DEBUG
	cout << "form is:\n" << endl;
	print(form);
#endif
}

//TO_DO
void Initialize_Simplex(slack_form& form) {
	int k = find_argmin_in_b(form.m,form.n,form.b);
	if (form.b[k] >= 0) {
#ifdef DEBUG
		cout << "初始解为可行解" << endl;
#endif
		return;
	}
	
	slack_form new_form=form;
	for (int i = 1; i <= new_form.n; i++) new_form.c[i] = 0;
	new_form.c[0] = -1;

	for (int i = new_form.n + 1; i <= new_form.n + new_form.m;i++) {
		new_form.A[i][0] = -1;
	}
	new_form.N.insert(0);
	//
	Pivot(new_form.N, new_form.B, new_form.A, new_form.b,
		new_form.c, new_form.v, k, 0);
	//
	int e = Select_e(new_form.c, new_form.N);
	while (exist_lawful_e(e)) {
		vector<double> delt(new_form.m+new_form.n+1);
		for (auto& i : new_form.B) {
			if (new_form.A[i][e] > 0)
				delt[i] = new_form.b[i] / new_form.A[i][e];
			else
				delt[i] = INF;
		}
		int l = find_argmin_in_delt(new_form.B, delt);
		if (delt[l] == INF) {
			cout << "unbounded\n";
			exit(0);
		}
		else 
			Pivot(new_form.N, new_form.B, new_form.A, new_form.b,
				new_form.c, new_form.v, l, e);
		e = Select_e(new_form.c, new_form.N);
	}
	//
	double x0 = (new_form.N.find(0)!=new_form.N.end()) ? 
		0:new_form.b[0] ;
	if (x0 != 0) {
		cout << "infeasible\n";
		exit(0);
	}
	else {
		if (new_form.N.find(0) == new_form.N.end()) {
			int l = get_number_in_N(new_form.N);
			Pivot(new_form.N, new_form.B, new_form.A, new_form.b,
				new_form.c, new_form.v, 0, l);
		}
		update_form(form,new_form);
	}

}


void Simplex(slack_form& form) {
	Initialize_Simplex(form);
	set<int>& N = form.N;
	set<int>& B = form.B;
	vector<vector<double>>& A = form.A;
	vector<double>& b = form.b;
	vector<double>& c = form.c;
	double& v = form.v;
	int m = form.m, n = form.n;
	int e = Select_e(c, N);
	while (exist_lawful_e(e)) {
		vector<double> delt(m+n+1);
		for (auto& i : B) {
			if (A[i][e] > 0) delt[i] = b[i] / A[i][e];
			else delt[i] = INF;
		}
		int l = find_argmin_in_delt(B,delt);
		if (delt[l] == INF) {
			cout << "unbounded" << endl;
			return;
		}
		else {
			Pivot(N, B, A, b, c, v, l, e);
		}
		e = Select_e(c, N);
	}
	//TO Revise
	cout << "max object value=" << v << endl;
	for (int i = 1; i <= n; i++) {
		if (B.find(i) != B.end()) {
			cout << "x" << i << "=" << b[i] << endl;
		}
		else  cout << "x" << i << "=0" << endl;
	}
}

void LP_Simplex() {
	ifstream fin;
	fin.open("in.txt");
	
	slack_form form(fin);
	//cout<<form.N.size()<<endl;
	
	fin.close();
#ifdef DEBUG
	cout << "read file is over" << endl;
#endif
	Simplex(form);
}


int main()
{
	LP_Simplex();
}
