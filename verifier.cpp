#include "header.h"

#define ll long int

using namespace std;
using namespace CryptoPP;
using CryptoPP::Integer;
using CryptoPP::ModularArithmetic;

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   AutoSeededRandomPool ra;
   //Predifined variables
   ll N,E,Phi;
   N = 101;
   E = 5;
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
 
   portno = 5001;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
   
   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }
   
   /* If connection is established then start communicating */
   bzero(buffer,256);
   n = read( newsockfd,buffer,255 );
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   printf("%s\n",buffer);
   
   // Start Sending Pubic Variables
   bzero(buffer,256);
   string s = "";
   s+=to_string(N)+":"+to_string(E);

   strcpy(buffer, s.c_str()); // Sending N:E

   n = write(newsockfd,buffer,strlen(buffer));

   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   // End Sending Public Variables

   // Start Receiving Public Key
   bzero(buffer,256);
   n = read( newsockfd,buffer,255 );
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   string public_key(buffer);
   ll pub_key = stoi(public_key);
   Integer V(pub_key);
   cout<<"Got Public Key 'v' "<< V <<" from the Claiment" << endl;
   // End Receiving Public Key

   // Start Receiving Witness
   bzero(buffer,256);
   n = read( newsockfd,buffer,255 );
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   string witness(buffer);
   ll wit = stoi(witness);
   Integer X(wit);
   cout<<"Got Witness "<< X <<" from the Claiment" << endl;
   // End Receiving Witness

   //Start Sending Challenge
   Integer C(ra,1,E);
   cout << "Sending Challenge "<< C << " to the Claiment. "<<endl;
   ll challenge = C.ConvertToLong();
   string chall = to_string(challenge);
   bzero(buffer,256);
   strcpy(buffer,chall.c_str());
   n = write(newsockfd,buffer,strlen(buffer));

   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   //End Sending Challenge

   // Start Receiving Response
   bzero(buffer,256);
   n = read( newsockfd,buffer,255 );
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   string Y(buffer);
   ll resp = stoi(Y);
   Integer Response(resp);
   cout << "Got Response "<< Response << " from the Claiment. "<<endl;
   // End Receiving Response   

   //Start Checking For verification
   Integer Nu = Integer(N);
   Integer Ee = Integer(E);
   Integer TemP = a_exp_b_mod_c(Response,Ee,Nu);
   Integer Temp2 = a_exp_b_mod_c(V,C,Nu);
   ModularArithmetic mod(N);
   Integer X2=a_times_b_mod_c(Temp2,TemP,Nu);
   cout << "Comparing x "<< X << " and  y^e.v^c "<< X2<<endl;
   if (X2.Compare(X) == 0) {
      cout << "Granted"<<endl;
   } else {
      cout << "Denied" << endl;
   }

   //cout<<X2<<"Check it:\n";

   //End Checking for verification

      
   return 0;
}
