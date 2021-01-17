//Thomas Papaloukas, icsd14155
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

gmp_randstate_t stat;

int main(void){
	mpz_t p,q,seed,m,plain,n,q_1,p_1,f,f_1,gcd,e,d,cipher,decrypted;
	char str[100]={'0'},array[2];
	char buff[100]={'0'};
	long sd=0;
	int i,k=0,primetemp=0,j=0;
	unsigned long int ascii[100];

	gmp_randinit(stat, GMP_RAND_ALG_LC, 120);
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(m);
	mpz_init(seed);
	mpz_init(plain);
	mpz_init(q_1);
	mpz_init(p_1);
	mpz_init(f);
	mpz_init(f_1);
	mpz_init(gcd);
	mpz_init(e);
	mpz_init(d);
	mpz_init(plain);
	mpz_init(cipher);
	mpz_init(decrypted);

	srand( (unsigned) getpid() );
	sd = rand();
	mpz_set_ui(seed, sd);

	gmp_randseed(stat, seed);

	mpz_urandomb(p, stat, 512);
	mpz_urandomb(q, stat, 512);

	printf("Please enter your message:\n");
	fgets(str, 50, stdin);

	for(i=0;i<strlen(str);i++){
		ascii[i] = (unsigned long int) str[i];
		//printf("%ld",ascii[i]);
	}
	for(i=0;i<strlen(str)*2;i+=2){
		sprintf(&buff[i], "%ld", ascii[k]/10);
		sprintf(&buff[i+1], "%ld", ascii[k]%10);
		//printf("ascii[%d]/10 = %ld\n",ascii[k]/10);
		//printf("ascii[%d}%10 = %ld\n",ascii[k]%10);
		k++;
	}
	//puts(buff);
	mpz_set_str(plain, buff, 10);	
	//gmp_printf("Plaintext: %Zd\n",plain);
	
	//Find primes p,q  based on probability function
	while(primetemp==0){
		mpz_urandomb(p, stat, 512);
		primetemp = mpz_probab_prime_p(p,10);
	}
	primetemp=0;
	while(primetemp==0){
		mpz_urandomb(q, stat, 512);
		primetemp = mpz_probab_prime_p(q,10);
	}
	gmp_printf("p = %Zd\n",p); gmp_printf("q = %Zd\n",q);	
	
	//Key Creation, n = p*q
	mpz_mul(n,p,q);
	mpz_sub_ui(p_1, p, 1); // p - 1
	mpz_sub_ui(q_1, q, 1); // q - 1
	mpz_lcm(f, p_1, q_1); // f(n) = n-1 = (p*q)-1 = (p-1)*(q-1)
			     //  n = q*p, because both p,q prime numbers
	
	//Search for e, in order to 1 < e < f and gcd(e,f)=1
	mpz_sub_ui(f_1, f, 1);
	//gmp_printf("f = %Zd\n",f);
	//gmp_printf("f-1 = %Zd",f_1);
	
	do{
		mpz_urandomm(e, stat, f_1);
		mpz_gcd(gcd,e,f);
	}while( mpz_cmpabs_ui(gcd,1) );
	
	gmp_printf("e = %Zd\n",e);

	//Find d = e ^ (-1) mod f
	mpz_invert(d, e, f);
	gmp_printf("d = %Zd\n",d);
	
	//Encryption
	mpz_powm(cipher, plain, e, n);
	gmp_printf("Cipher Text : %Zd",cipher);
	
	//Decryption
	mpz_powm(decrypted, cipher, d, n);
	gmp_printf("Decrypter Text : %Zd\n",decrypted);
	puts("");
	
	mpz_get_str(buff, 10, decrypted);	
	
	//Plaing text to chars
	j=0;
	for(i=0;i<strlen(buff);i+=2){
	  array[0] = buff[i];
	  array[1] = buff[i+1];
	  ascii[j] = atoi(array);
	  j++;
	}
	i=0;
	do{
		str[i] = (char) ascii[i];
		i++;
	}while(!ascii[i]);
	puts(str);
	
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(f);
	mpz_clear(e);
	mpz_clear(p_1);
	mpz_clear(q_1);
	mpz_clear(f_1);
	mpz_clear(d);
	mpz_clear(gcd);
	mpz_clear(m);
	mpz_clear(plain);
	mpz_clear(cipher);
	mpz_clear(decrypted);
	mpz_clear(seed);
		
	return 0;
}
