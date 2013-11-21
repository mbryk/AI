int main(int argc, char **argv){
	int nodeAmts[3] = {inputs,hiddens,outputs};
	int L1[inputs][hiddens]; // Weights
	int L2[hiddens][outputs]; //Weights

	int i,j;
	for(j=0;j<hiddens;j++){
		for(i=0;i<inputs;i++){
			L1[i][j] = rand();	
		} 
		for(k=0;k<outputs;k++){
			L2[j][k] = rand();
		}
	}
	for(e=0;e<length(examples);e++){
		for(i=0;i<length(x);i++){
			node[0][i]->activation = x[i];
		}
		for(l=1;l<3;l++){
			for(j=0;j<nodeAmts[l];j++){
				node[l][j]->in = 0;
				for(i=0;i<nodeAmts[l-1];i++){
					node[l][j]->in += L[i][j]*node[l-1][i]->activation;
				}
				node[l][j]->activation = g(in);
			}
		}
		for(j=0;j<nodeAmts[2];j++){
			node[2][j]->delta = gp(node[2][j]->in)*y[j]-node[2][j]->activation;
		}
		for(l=1;l>0;l--){
			for(i=0;i<nodeAmts[l];i++){
				delta = 0;
				for(j=0;j<nodeAmts[l+1];j++){
					delta += L[i][j] * node[l+1][j]->delta;
				}
				node[l][i]->delta = gp(node[2][j]->in)*delta;
			}
		}
		for(j=0;j<hiddens;j++){
			for(i=0;i<inputs;i++){
				L1[i][j] += alpha * node[0][i]->activation * node[1][i]->delta;
			} 
			for(k=0;k<outputs;k++){
				L2[j][k] += alpha * node[1][j]->activation * node[2][k]->delta;
			}
		}
	}
}
int g(int in){

	return 0;
}
int gprime(int in){

	return 0;
}