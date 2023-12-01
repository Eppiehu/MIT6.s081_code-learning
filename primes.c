#include "user/user.h"
#include "kernel/types.h"
#include "kernel/stat.h"


int prime(int p)
{
	if(p<=1)
	{
		return 0;
	}
	for(int i=2;i<p;i++)
	{
		if(p%i==0)
		{
			return 0;
		}
	}
	return 1;
} 


int main(){
    int p[2];
    pipe(p);
    int nums[34];
    int n; 
    int re = 0; 
    for(n = 2;n<=35;n++){
        nums[re] = n;
        re ++;
    }
    if(fork() > 0){
        int n;
        close(p[0]);
        for(n = 0;n<=33;n++){
            write(p[1],&nums[n],4);
        }
        close(p[1]);
        wait(0);
    }
    else{
        int num;
        close(p[1]);
        int n;
        for(n = 0;n<=33;n++){
            read(p[0],&num,4);
            if(prime(num) == 1){
                printf("prime %d\n",num);
            }          
        }
        exit(0);
    }
    exit(0);
}
