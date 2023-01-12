/*
 ============================================================================
 Name        : Esercizio_3_RdC_Client.c
 Author      : checcolino
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//portabilità
#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

//Librerie
#include <stdio.h>


//COSTANTI
#define BUFFERSIZE 512
#define PROTOPORT 27015

//funzioni
void ErrorHandler(char *errorMessage) {
printf ("%s", errorMessage);
}
void ClearWinSock() {
#if defined WIN32
WSACleanup();
#endif
}
void ClearArray(char* a, int j) {

	for(int i = 0; i < j; i++) {

		a[i] = 0;
	}
}


int main(void) {
//inizializzo winsock32

	#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		ErrorHandler("Error at WSAStartup()\n");
		return -1;
	}
	#endif

//crezione di una variabile int che conterrà descrittore della socket
int ClientSocket;

  //chiamo la funzione socket() assegnando il valore di ritorno dalla variabile appena creata
	ClientSocket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);

  //valuto la presenza di errori per assicurarmi che la socket sia valida
  if(ClientSocket < 0 )
	{
		ErrorHandler("socket creation failed\n");
		return -1;
	}


  //Assegnazione di un indirizzo alla socket (solo lato server)

  //Creo un elemento di tipo sockaddr_in
  int porta;

  printf("Inserire numero porta (27015):\n");
  scanf("%d", &porta);
  printf("\n");
  fflush(stdin);

  struct sockaddr_in sad;
  memset(&sad, 0, sizeof(sad));
  //Avvaloro l'elemento creato
  //Assegno porta e ip alla socket
  sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	sad.sin_port = htons( porta );


  //richiesta di connessione del client al server
	if (connect(ClientSocket, (struct sockaddr*) &sad, sizeof(sad)) <0)
	{
		ErrorHandler("Failed to connect.");
		return 0;
	}



	char msg[BUFFERSIZE];
	ClearArray(msg, BUFFERSIZE);

	//ricezione della stringa di connessione
	if(recv(ClientSocket, msg, BUFFERSIZE-1, 0) <=0 ) {

		ErrorHandler("Messaggio non ricevuto");
		closesocket(ClientSocket);
		ClearWinSock();
		return -1;
	}

	else {

		puts(msg);
	}
	ClearArray(msg, BUFFERSIZE);

		int flag_chiusura = 1; //flag che gestisce il caso di chiusura della comunicazione 1= vero, 0 = falso

		while(flag_chiusura) {
	for(int i=0;i<2;i++){
			printf("Inserire la stringa da inviare al server\n");

						gets(msg);
						fflush(stdin);

						if(send(ClientSocket, msg, strlen(msg), 0) != strlen(msg))
						{
							ErrorHandler("Messaggio non inviato");
							closesocket(ClientSocket);
							ClearWinSock();
							return -1;
						}
						ClearArray(msg, BUFFERSIZE);



	}

	char bye[BUFFERSIZE] = "bye";
	char msg1[BUFFERSIZE];
	ClearArray(msg1, BUFFERSIZE);

	if(recv(ClientSocket, msg1, BUFFERSIZE-1, 0) <=0 ) {

		ErrorHandler("Messaggio non ricevuto");
		closesocket(ClientSocket);
		ClearWinSock();
		return -1;
	}


	int r1;

	r1 = strcmp(bye,msg1);

	if(r1 == 0){
		closesocket(ClientSocket);
		ClearWinSock();
		return -1;
	}
	else{
		printf("\nStringa concatenata: ** %s **\n\n",msg1);
		system("pause");
	}










}



// CHIUSURA DELLA CONNESSIONE
closesocket(ClientSocket);
ClearWinSock();
printf("\n"); // Print a final linefeed
system("pause");
return(0);
}









