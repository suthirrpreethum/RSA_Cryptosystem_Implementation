#include<iostream>
#include<stdio.h>
#include<cstdlib>
#include <ctime>
#include <math.h>
#include <vector>
using namespace std;

unsigned long long int p,q,publicKey_e,n,bearcatiiMsg_M,encryptedMsg_C,decryptedMsg_P,totient_T,privateKey_d=1;

//Naive power
unsigned long long int power(unsigned long long int base,unsigned long long int powers)
{
	if (powers == 1)
		return (base);
   	else if (powers%2==0)
		return(power(base*base,powers/2));
   else
		return(base*power(base*base,(powers-1)/2));

}

//Euclids GCD
int gcd(unsigned long long int a, unsigned long long int b)
{
    unsigned long long int rem;
    int gcdfact;
    while (b!=0)
    {
        rem = a % b;
        a = b;
        b = rem;
    }
    gcdfact = a;
    return gcdfact;
}

//BEARCATII Conversion table
int BEARCATIIencode(string a)
{
    int  d1, conversion =0, base = 0, bearcatiiMsg[a.size()];
    for(int i=0;i<a.size();i++)
    {
        d1=a[i];
        if(d1>96)
            bearcatiiMsg[i]=d1-96;
        else if(d1==32)
            bearcatiiMsg[i]=0;
    }
	


    //conversion of message to base 27.
   	 int s = a.size()-1;
    for(int i=s;i>=0;i--)
    {
         base = bearcatiiMsg[i]*pow(27,s-i);
         conversion = conversion + base;
    }
    return conversion;
}

//Plain Text Conversion
string BEARCATIIdecode(unsigned long long int decryptedBercatiiMsg)
{
	vector<int> decodedMessageInInt;
	string decryptedMessage;
	while(decryptedBercatiiMsg>0)
	{
		int temp=decryptedBercatiiMsg%27;
		decodedMessageInInt.push_back(temp);
		decryptedBercatiiMsg /=27;
	}
	
    for (int i = decodedMessageInInt.size()-1; i >=0 ; i--)
	{
		if(decodedMessageInInt.at(i)==0)
			decryptedMessage.push_back(' ');
		else
		{
			char temp=(char)(decodedMessageInInt.at(i)+96);
			decryptedMessage.push_back(temp);
		}
	}
	cout<<"\nThe decrypted message is \'"<<decryptedMessage<<"\'";
		return decryptedMessage;
}

//Power Module function
unsigned long long int powerModulo(unsigned long long int num, unsigned long long int power,unsigned long long int modulo )
{
	unsigned long long int value=num%modulo;
	for(unsigned long long int i=2;i<=power;i++)
	{
		value=(value*num)%modulo;
	}
	return value;
}

//Cryptosystem main function
void RSA_Cryptosystem()
{
    unsigned long long int a,b;
   encryptedMsg_C= powerModulo(bearcatiiMsg_M,publicKey_e,n) ;
    decryptedMsg_P= powerModulo(encryptedMsg_C,privateKey_d,n);

    cout<<endl<<"The original BEARCATII message bearcatiiMsg_M is "<<bearcatiiMsg_M;
    cout<<endl<<"The encrypted BEARCATII message encryptedMsg_C is "<<encryptedMsg_C;
    cout<<endl<<"The decrypted BEARCATII message decryptedMsg_P is "<<decryptedMsg_P;

}

//ModInverse function to generate private key
int modInverse(int a, int b)
{
    a = a%b;
    for (int x=1; x<b; x++)
       if ((a*x) % b==1)
          return x;
}

//Miller Rabin Primality Test
//Miller Rabin Primality Test
bool millerTest(int m, int n)
{
    int a,b;
    a = 2+rand()%(n-2);
    b = powerModulo(a,m,n);
    if (b!=1 && b!=n-1)
       return false;
    else if (b == n-1)
       return true;
    b = (b*b)%n;
    if (b!=1)
        return false;
    else
        return true;
    return false;
}

bool primecheck(int n, int k)
{
    int m = n - 1;
    while (m%2 == 0)
        m/=2;
    for (int i=0; i<k;i++)
         if (millerTest(m,n) == false)
              return false;
    return true;
}
int main()
{
	int gcdfact,x,i=0,k=4;
	string message=" ";
	bool p_prime = false, q_prime=false;

	//Random number generation
	srand(time(NULL));
	while (!p_prime)
	{
	    p = rand()%100+2050;
		if (p==1)
	        continue;
	    p_prime=primecheck(p,k);
	}

	while (!q_prime)
	{
    	q = rand()%100+2050;
		if (q==p || q==1)
    	    continue;
    	q_prime=primecheck(q,k);
	}

	cout << "First Random Prime Number p is " << p <<endl <<"Second Random Prime Number q is "<<q<<endl;
	n = p*q;
	cout <<endl<< "The Value of n=p*q is " << n <<endl;

	totient_T=(p-1)*(q-1);
	cout<<endl<<"The Value of Toitent function (p-1 * q-1) is "<<totient_T<<endl;

	cout <<endl<<"Enter the Public Key " <<endl;
	cin >> publicKey_e;

	gcdfact = gcd(totient_T,publicKey_e);
	while (gcdfact != 1)
	{
	    cout<< "Public Key is not a Co-Prime with toitent function. Enter a different number"   << endl;
	    cin >> publicKey_e;
	    gcdfact = gcd(totient_T,publicKey_e);
	}
	cout << endl<<"The GCDfact of publicKey_e and toitent function is "<<gcdfact<<endl;

	//Private key Generation
	privateKey_d = modInverse(publicKey_e,totient_T);
	cout<<endl<<"The generated Private key based on Toitent function is "<<privateKey_d<<endl;

	cin.ignore();
    cout <<endl<<"Enter the message: ";
    getline(cin,message);
	
	while(1)
	{
		cout <<endl<<"The entered Text message is "<<message<<endl;

		bearcatiiMsg_M = BEARCATIIencode(message);
		cout<<endl<<"The Bearcatii conversion of the above text message is "<<bearcatiiMsg_M<<endl;

		RSA_Cryptosystem();
		BEARCATIIdecode(decryptedMsg_P);

		cout <<endl<<endl<< "Try Again.....Enter the message"<< endl;
		getline(cin,message);
	}
	return 0;

}


