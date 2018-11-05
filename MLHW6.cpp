#include <iostream>
#include <armadillo>
#include <fstream>
#include <math.h>

using namespace std;
using namespace arma;

// Armadillo documentation is available at:
// http://arma.sourceforge.net/docs.html

int
main(int argc, char** argv)
  {
	  
	char theline [100] ;
	
	double data[500][11];
	// initial
	for(int i=0; i<500; i++){
		for(int j=0; j<11; j++){
			data[i][j] = 0;
		}
	}
	double gamma = 0.125;
	double lambda = 1000;

	//read file
	fstream fin;
	fin.open("lssvm.txt",ios::in);

	//分割字串存入二維陣列
	char *token = NULL;
	char *next_token = NULL;
	char seps[]   = " ,\t\n";
	int a=0;
	while(fin.getline(theline,sizeof(theline),'\n')){
		int b = 0;
		token = strtok_s( theline, seps, &next_token);
		while (token != NULL){
			data[a][b] = atof(token);
            token = strtok_s( NULL, seps, &next_token);
			b++;
		}
		a++;
	}
	
	double train[400][11];
	double test[100][11];
	for(int i=0; i<400; i++){
		for(int j=0; j<11; j++){
			train[i][j] = data[i][j];
		}
	}
	for(int i=0; i<100; i++){
		for(int j=0; j<11; j++){
			test[i][j] = data[i+400][j];
		}
	}

	mat Mtrain(400,10);
	for(int i=0; i<400; i++){
		for(int j=0; j<10; j++){
			Mtrain(i,j) = train[i][j];
		}
	}
	mat Mtest(100,10);
	for(int i=0; i<100; i++){
		for(int j=0; j<10; j++){
			Mtrain(i,j) = test[i][j];
		}
	}
	mat ytrain(400,1);
	for(int i=0; i<400; i++){
		ytrain(i,0) = train[i][10];
	}
	
	mat k(400,400);
	for(int i=0; i<400; i++){
		for(int j=0; j<400; j++){
			k(i,j) = 0;
		}
	}
	for(int i=0; i<400; i++){
		for(int j=0; j<400; j++){
			double temp[10] = {0,0,0,0,0,0,0,0,0,0};
			for(int k=0; k<10; k++){
				temp[k] = Mtrain(i,k) - Mtrain(j,k);
			}
			double x = 0;
			for(int k=0; k<10; k++){
				x = x + temp[k]*temp[k];
			}
			k(i,j) = exp((-1)*gamma*x);
		}
	}
	mat I(400,400);
	for(int i=0; i<400; i++){
		for(int j=0; j<400; j++){
			if(i==j){
				I(i,j) = 1;
			}else{
				I(i,j) = 0;
			}
		}
	}
	mat beforeinverse(400,400);
	beforeinverse = lambda*I+k;

	mat afterinverse = inv(beforeinverse);

	mat beta = afterinverse * ytrain;


	mat ktest(400,100);
	for(int i=0; i<400; i++){
		for(int j=0; j<100; j++){
			ktest(i,j) = 0;
		}
	}
	for(int i=0; i<400; i++){
		for(int j=0; j<100; j++){
			double temp[10] = {0,0,0,0,0,0,0,0,0,0};
			for(int k=0; k<10; k++){
				temp[k] = Mtrain(i,k) - Mtest(j,k);
			}
			double x = 0;
			for(int k=0; k<10; k++){
				x = x + temp[k]*temp[k];
			}
			ktest(i,j) = exp((-1)*gamma*x);
		}
	}

	double eout = 0;
	for(int i=0; i<100; i++){
		double temp = 0;
		for(int j=0; j<400; j++){
			temp = temp + beta(j,0)*k(j,i);
		}
		int ans = 0;
		if(temp>=0){
			ans = 1;
		}else{
			ans = -1;
		}
		if(ans==test[i][10]){

		}else{
			eout++;
		}

	}
	eout = eout/100;
	cout<<eout<<endl;
	
  system("pause");
  return 0;
  }