#include <stdio.h> 
#include <stdlib.h> /* pour exit */ 
#include <sys/types.h> /* pour socket */ 
#include <sys/socket.h> 
#include <string.h> /* pour memset */ 
#include <netinet/in.h> /* pour struct sockaddr_in */ 
#include <arpa/inet.h> /* pour htons et inet_aton */
#include <unistd.h> // for close

  #define LG_MESSAGE 256
  #define BLEU "\033[00;34m"
  #define ROUGE "\033[00;31m"
  #define NORMAL "\033[00m"
  #define BLEUCLAIR "\033[01;34m"

int main() {
  int sock;
  struct sockaddr_in adresse; /* adresse et numero de port du serveur */
  socklen_t longueurAdresse;
  char messageEnvoi[LG_MESSAGE]; /* le message envoye */
  char messageRecu[LG_MESSAGE]; /* le message recu */
  int nbo_ecrits, nbo_lus, retour;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(-1);
  }
  printf("Socket creee avec succes ! (%d)\n", sock);
  longueurAdresse = sizeof(adresse);
  memset( & adresse, 0x00, longueurAdresse);
  adresse.sin_family = AF_INET;
  adresse.sin_port = htons(IPPORT_USERRESERVED); // = 5000
  inet_aton("127.0.0.1", & adresse.sin_addr);
  // On se connecte au serveur
  if ((connect(sock, (struct sockaddr * ) & adresse, longueurAdresse)) == -1) {
    perror("connect");
    close(sock);
    exit(-2);
  }
  printf(ROUGE "Connexion au serveur reussie avec succes !\n" NORMAL);
  printf("Votres port est : (%d)\n", & adresse.sin_port);
  printf("Votres adresse ip est : (%d)\n", & adresse.sin_addr);
  // On initialise à 0 les messages
  memset(messageEnvoi, 0x00, LG_MESSAGE * sizeof(char));
  
  memset(messageRecu, 0x00, LG_MESSAGE * sizeof(char));
  // Envoie un message au serveur

int x ;
    printf("Envoyer message ( Choix entre 1 et 4 ) : \n");
    printf("- 1 : Message en Bleu\n");
    printf("- 2 : Message en Rouge\n");
    printf("- 3 : Message en Blanc\n");
    printf("- 4 : Message personnalisé \n");
    printf("Mettre le Chiffre : ");
    scanf("%d",&x);

    switch(x){
      case 1 :  sprintf(messageEnvoi, BLEU "C'est le Client 1 le message est en bleu\n"NORMAL);
        nbo_ecrits = write(sock, messageEnvoi, strlen(messageEnvoi));
          break; 
          // ecrit un message et l'envoi au client
      case 2 :  sprintf(messageEnvoi, ROUGE "C'est le Client 1 le message est en rouge\n"NORMAL);
        nbo_ecrits = write(sock, messageEnvoi, strlen(messageEnvoi));
          break; 
          // ecrit un message et l'envoi au client
      case 3 :  sprintf(messageEnvoi, NORMAL"C'est le Client 1 le message est en Blanc\n"NORMAL);
        nbo_ecrits = write(sock, messageEnvoi, strlen(messageEnvoi));
          break;
           // ecrit un message et l'envoi au client
      case 4 :  printf("Ecrit un message : ");
      scanf("%s",messageEnvoi);
        nbo_ecrits = write(sock, messageEnvoi, strlen(messageEnvoi));
          break;
      default : printf("Selectionner un chiffre entre 1 et 3 \n");break;

    }

  // On ecrits les données vers le serveur 
  nbo_ecrits = write(sock, messageEnvoi, strlen(messageEnvoi));
  switch (nbo_ecrits) {
  case -1:
    /* en cas d'une erreur */
    perror("write");
    close(sock);
    exit(-3);
  case 0:
    /* la socket est fermee */
    fprintf(stderr, "La socket a ete fermee par le serveur !\n\n");
    close(sock);
    return 0;
  default:
    /* envoi de n octets */
    printf("Message %s a ete envoye avec succes (%d octets)\n\n", messageEnvoi, nbo_ecrits);
  }


  /* On recoit le message envoyer serveur */
  nbo_lus = read(sock, messageRecu, LG_MESSAGE * sizeof(char)); /* on attend un message de TAILLE fixe */
  switch (nbo_lus) {
  case -1:
    perror("read");
    close(sock);
    exit(-4);
  case 0:
    /* la socket est fermee */
    fprintf(stderr, "La socket a ete fermee par le serveur !\n\n");
    close(sock);
    return 0;
  default:
    /* reception de nbo_lus octets */
    printf("Message recu du serveur : %s (%d octets)\n\n", messageRecu, nbo_lus);
  }

  // On ferme toujours la socket avant de quitter
  close(sock);
  return EXIT_SUCCESS;
}
