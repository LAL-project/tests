#include <iostream>
#include <vector>
using namespace std;

#include <lal/generation/rand_ulab_free_trees.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/output.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

typedef vector<integer> vecteur;

/* Make sure that the computation of r_n and alpha(m,q) is correct.
 * 
 * r_n: the number of unlabelled rooted trees of n vertices
 * alpha(m,q) from Wilf's paper (The Uniform Selection of Free Trees),
 * 		the number of rooted forests on m vertices where each tree
 * 		has at most q vertices.
 * 
 * 
 * 
 * 
 * 
 * 
 * Code from: https://github.com/marohnicluka/giac/blob/master/graphe.cc#L7149
 */

void compute_rutcounts(int n,vecteur &t) {
	t.resize(n+1);
	t[1]=1;
	integer sum,td;
	int nlast=1,i;
	while (n>nlast) {
		sum=0;
		for (int d=1;d<=nlast;++d) {
			i=nlast+1;
			td=t[d]*integer(d);
			for (int j=1;j<=nlast;++j) {
				i-=d;
				if (i<=0) break;
				sum += t[i]*td;
			}
		}
		++nlast;
		t[nlast]=sum/integer(nlast-1);
	}
}


int main(int argc, char *argv[]) {
	int n = atoi(argv[1]);
	
	
	/* THIS CODE CAN BE FOUND FOLLOWING THE LINK
	 * 
	 * https://github.com/marohnicluka/giac/blob/master/graphe.cc#L7149
	 */
	
	vecteur a;
	compute_rutcounts(n,a);
	
	/*cout << "Unlabelled rooted trees:" << endl;
	for (int i = 0; i <= n; ++i) {
		cout << "    " << i << ": " << a[i] << endl;
	}*/
	
	vecteur alpha(n);
	alpha[0]=1;
	integer sum,alphad;
	int m=0,i;
	while (n-1>m) {
		sum=0;
		for (int d=1;2*d<=n-1;++d) {
			i=m+1;
			alphad=a[d]*integer(d);
			for (int j=1;;++j) {
				i-=d;
				if (i<0) break;
				sum+=alpha[i]*alphad;
			}
		}
		++m;
		alpha[m]=sum/integer(m);
	}
	
	/*cout << "Alpha(m,q) for m=n-1 and fixed q:" << endl;
	for (int i = 0; i < n; ++i) {
		cout << "    " << i << ": " << alpha[i] << endl;
	}*/
	
	//cout << "----------------------------------" << endl;
	/* TEST LAL'S CODE
	 * 
	 * 
	 */
	
	rand_ulab_free_trees TreeGen(n);
	TreeGen.make_rand_tree();
	
	//cout << "rn equal?" << endl;
	for (size_t i = 0; i < n; ++i) {
		if (TreeGen.m_rn[i] != a[i]) {
			cerr << "Error!" << endl;
			cerr << "    i= " << i << endl;
			cerr << "    TreeGen.m_rn[i]= " << TreeGen.m_rn[i] << endl;
			cerr << "    a[i]= " << a[i] << endl;
			return 1;
		}
	}
	
	//cout << "alpha(m,q) equal?" << endl;
	for (size_t i = 0; i < n; ++i) {
		if (TreeGen.m_alpha[make_pair(i, (n-1)/2)] != alpha[i]) {
			cerr << "Error!" << endl;
			cerr << "    i=" << i << endl;
			cerr << "    TreeGen.m_alpha[make_pair(i, (n-1)/2)]= " << TreeGen.m_alpha[make_pair(i, (n-1)/2)] << endl;
			cerr << "    alpha[i]= " << alpha[i] << endl;
			return 1;
		}
	}
	return 0;
}
