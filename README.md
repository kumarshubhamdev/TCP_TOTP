1)This a Simple TCP Authentication Demonstration.  
2)Using TCP we can create a server client connection.  
3)But in this specific implementation , the server has a unique 32bit Key , that generates time based OTP more famously TOTP  
4)Check TOTP implementation for more on TOTP  
5)The Client is prompted to enter TOTP  
6)Each TOTP is valid for 30 sec , can be changed , but hard coded  
7)Server verfies by generating the TOTP on its side too  
8)Send the message after the verification if successful , a Success Green Color Prompt else client is re-prompted to enter the new TOTP  
9)Proof of Concept for only allowing authenticated clients!  

10)The Makefile generates server.exe  
11)client.c can be compiled by executing following commands  
12)Change the Server IP Address Macro in client.c to your server IP Address.  
>gcc -c client.c -o client.o  
>gcc client.o -o client.exe  

>Execute Server.exe on a computer   
>Execute client.c on other computer   
  
>See the magic ;)
