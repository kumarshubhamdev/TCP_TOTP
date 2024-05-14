>This a Simple TCP Authentication Demonstration.
>Using TCP we can create a server client connection.
>But in this specific implementation , the server has a unique 32bit Key , that generates time based OTP more famously TOTP
>Check TOTP implementation for more on TOTP
>The Client is prompted to enter TOTP
>Each TOTP is valid for 30 sec , can be changed , but hard coded
>Server verfies by generating the TOTP on its side too
>Send the message after the verification if successful , a Success Green Color Prompt else client is re-prompted to enter the new TOTP
>Proof of Concept for only allowing authenticated clients!

>The Makefile generates server.exe
>client.c can be compiled by executing following commands
>Change the Server IP Address Macro in client.c to your server IP Address.
gcc -c client.c -o client.o
gcc client.o -o client.exe

>Execute Server.exe on a computer 
>Execute client.c on other computer 

>See the magic ;)
