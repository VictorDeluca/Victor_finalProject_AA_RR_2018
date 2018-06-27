#include <bits/stdc++.h>
#define INF 99999999
using namespace std;

int v,e,source,max_trucks; double graph[100][100],route_c,best,limit; 
vector<vector<int> > route,bestroute; vector<int> operable;

double cost(vector<vector<int> > vetor){
	int i,j; double ans = 0;
	for(j=0;j<vetor.size();j++){
		double helper = 0;
		for(i=1;i<vetor[j].size();i++) helper+=graph[vetor[j][i-1]][vetor[j][i]];
		helper+=graph[vetor[j][i-1]][source];

		ans = max(ans,helper);
	}
	return ans;
}

//Operable are trucks worth reversing values. Non-operable trucks can only receive insertions.
vector<vector<int> > generate(int trucks){
	operable.clear();

	int i; vector<vector<int> > ans; vector<int> helper; helper.push_back(source);
	for(i=0;i<trucks;i++) ans.push_back(helper);

	vector<int> possibilities; 
	for(i=0;i<v;i++) if(i!=source) possibilities.push_back(i);

	for(i=0;i<v-1;i++){
		int next = rand()%possibilities.size();
		int chosen = rand()%trucks;

		ans[chosen].push_back(possibilities[next]);
		possibilities.erase(possibilities.begin()+next);
	}

	for(i=0;i<trucks;i++) if(ans[i].size()>1) operable.push_back(i);

	return ans;
}

//Two transformations: Swap, which trades values between two trucks, and insert, which takes a value from
//a truck and inserts it in another. Reverse is the same transformation as in the original code.
vector<vector<int> > transform(vector<vector<int> > src, int trucks){
	int op = rand()%3; //0 = Swap; 1 = Insert; 2 = Reverse;

	if(op==0){
		int t1 = operable[rand()%operable.size()], t2 = operable[rand()%operable.size()];
		int v1 = rand()%(src[t1].size()-1), v2 = rand()%(src[t2].size()-1);
		v1++; v2++;

		int aux = src[t1][v1]; src[t1][v1] = src[t2][v2]; src[t2][v2] = aux;
	} else if(op==1){
		int t0 = rand()%operable.size();
		int t1 = operable[t0]; int t2 = rand()%trucks;
		if(t1==t2) return src;
		int v1 = rand()%(src[t1].size()-1); int v2 = rand()%(src[t2].size());
		v1++; v2++;

		int aux = src[t1][v1];
		src[t1].erase(src[t1].begin() + v1); src[t2].insert(src[t2].begin() + v2, aux);
		
		int i,j; vector<int> newoperable;
		for(i=0;i<trucks;i++)
			if(src[i].size()>1) newoperable.push_back(i);
		
		operable = newoperable;
	} else if(op==2){
		int t = operable[rand()%operable.size()];
		int v1 = rand()%(src[t].size()-1), v2 = rand()%(src[t].size()-1);
		v1++; v2++;

		reverse(src[t].begin() + min(v1,v2), src[t].begin() + max(v1,v2)); //reverse [left,right)
	}
	return src;
}

int main(){
	int i,j; srand(time(NULL));
	for(i=0;i<100;i++) for(j=0;j<100;j++) graph[i][j] = INF;

	scanf("%d%d%d%lf",&v,&e,&max_trucks,&limit);
	for(i=0;i<e;i++){
		int x,y; double z;
		scanf("%d%d%lf",&x,&y,&z);
		graph[x][y] = z;
	}
	scanf("%d",&source);
	int trucks = 1;
	while(trucks<=min(max_trucks,v)){
		bestroute = generate(trucks); best = INF;
		int attempts = 100; //We try 100 different executions with different starting vectors
		while(attempts--){
		//Initializing with an arbitrary source.
			route = generate(trucks);
			route_c = cost(route);

			double T = 100, it;
			while(T>0.1){ //Ending at a low temperature, here it's 0.
				it = 100;
				while(it--){ //Iterations for every temperature
					vector<vector<int> > helper = transform(route,trucks);

					if(cost(helper) < route_c){
						route_c = cost(helper);
						route = helper;

						if(cost(helper) < best){
							best = cost(helper);
							bestroute = helper;
						}
					} else {
						double delta = cost(helper) - route_c;
						double prob = exp(-delta/T);
						double r1 = (rand()%1000); r1/=1000;

						if(r1<=prob){
							route_c = cost(helper);
							route = helper;
						}
					}
				}

				T*=0.90;
			}
		}

		if(best <= limit) break;
		trucks++;
	}

	if(best>limit){
		printf("Impossible to deliver within time limit\n");
	} else{
		printf("%d trucks will be needed\n",trucks);
		printf("Time needed: %.4lf seconds\n",best);
		printf("Route suggested:\n");
		for(i=0;i<bestroute.size();i++){
			printf("Truck %d: ",i+1);
			for(j=0;j<bestroute[i].size();j++){
				printf("%d -> ",bestroute[i][j]);
			}
			if(bestroute[i].size() > 1) printf("%d\n",source);
			else printf("Do nothing\n");
		}
	}

	return 0;
}