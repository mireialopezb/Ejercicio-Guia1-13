#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char buff[512];
	char buff2[512];
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9080);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 2) < 0)
		printf("Error en el Listen");
	
	
	// Atenderemos solo 7 peticione
	//for(int i=0;i<7;i++){
	
	for(;;)
	{ //bucle infinito
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexi?n\n");
		//sock_conn es el socket que usaremos para este cliente
		
		//Bucle de atención al cliente
		int terminar=0;
		while (terminar ==0)
		{
			
			// Ahora recibimos su nombre, que dejamos en buff
			ret=read(sock_conn,buff, sizeof(buff));
			printf ("Recibido\n");
			
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			buff[ret]='\0';
			
			//Escribimos el nombre en la consola
			
			printf ("Se ha conectado: %s\n",buff);
			
			
			char *p = strtok( buff, "/");
			int codigo =  atoi (p);
			char nombre[20];
			
			if(codigo!=0){
				p = strtok( NULL, "/");
				strcpy (nombre, p);
				
				printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
			}
			
			if (codigo == 0)
				terminar=1;
			
			else if (codigo ==1) //piden la longitd del nombre
				sprintf (buff2,"%d",strlen (nombre));
			
			else if (codigo==2)
				// quieren saber si el nombre es bonito
				if((nombre[0]=='M') || (nombre[0]=='S'))
				strcpy (buff2,"SI");
				else
					strcpy (buff2,"NO");
				
			else if (codigo==3) 
			//decir si es alto
			{
				p = strtok( NULL, "/");
				float altura = atof (p);
				if (altura>1.70)
					sprintf(buff2,"%s: eres una persona alta",nombre);
				else 
					sprintf (buff2,"%s: eres una persona baja",nombre);
					
			}
			
			else if (codigo==4)
			//decir si el nombre es palíndromo
			{
				int encontrado=0;
				char nombre_original[20];
				strcpy(nombre_original,nombre);
				nombre[0]=tolower(nombre[0]); //convertimos la inicial a miníscula
				int i=0;
				while (encontrado==0)
				{
					if(nombre[i]!=nombre[strlen(nombre)-1-i])
						encontrado = 1; 
					//si 2 letras (una empezando por el principio y otra por en final)
					//no coinciden, deja ser una palabra palíndrome
					
					if ((i==strlen(nombre)-1-i)||(i+1==strlen(nombre)-1-i))
						encontrado = 2; 
					//ha comparado todas las letras de la palabra
					// i==strlen(nombre)-1-i cuando strlen(nombre) es impar
					// i+1==strlen(nombre)-1-i cuando strlen(nombre) es par
					i++;
				}
				if(encontrado==2) 
					   //se ha recorrido toda la palabra y las letras eran iguales
						sprintf(buff2, "%s es un nombre palíndromo",nombre_original);
				else if (encontrado==1) 
					//se ha encontrado como mínimo 2 letras que no coincidian
					sprintf(buff2,"%s no es un nombre palíndromo", nombre_original);
			}
			
			else if (codigo==5)
			//devolver el nombre en mayúsculas
			{
				char nombre_mayusculas[20];
				strcpy(nombre_mayusculas,nombre)
				for (int i = 0; nombre_mayusculas[i] != '\0';i++)
				//recorre la cadena hasta que encuentra \0
				{
					nombre_mayusculas[i] = toupper(nombre_mayusculas[i]);
				}
				sprintf(buff2,"%s",nombre_mayusculas);
			}
			
			if(codigo!=0)
			{
				printf ("%s\n", buff2);
				// Y lo enviamos
				write (sock_conn,buff2, strlen(buff2));
			}
			
			
			// Se acabo el servicio para este cliente
			close(sock_conn); 
		}
	}
}
