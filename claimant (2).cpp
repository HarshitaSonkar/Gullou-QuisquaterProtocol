#include "header.h"

using namespace std;
using namespace CryptoPP;
using CryptoPP::Integer;
using CryptoPP::ModularArithmetic;

int main(int argc, char *argv[]) {
	int sockfd, portno, n;
	ll N,E;
	AutoSeededRandomPool ra;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];

	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);

	/* Create a socket point */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	server = gethostbyname(argv[1]);

	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	/* Now connect to the server */
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR connecting");
		exit(1);
	}

	while (1) {

		/* Now ask for a message from the user, this message
		 * will be read by server
		 */
		cout << "Regestring with Server" << endl;

		bzero(buffer,256);
		n = write(sockfd,"Requesting Public variables",27);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
		//Start Getting the public variables
		bzero(buffer,256);
		n = read(sockfd, buffer, 255);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}

		string public_variables(buffer);
		//cout<<buffer<<endl;
		N = stoi(public_variables);
		cout<<"Got Public Key "<< N << endl;

		int temp = public_variables.find(":")+1;
		string te = public_variables.substr(temp,public_variables.size());// Extracting E
		E = stoi(te);
		Integer Nu = Integer(N);
		Integer Ee = Integer(E);
		//End Getting the public Variable

		//Start Calculating private Key
		Integer S(ra,1,N-1);
		Integer Se =a_exp_b_mod_c(S,Ee,Nu);
		//End Calculating private Key

		//Start Calculating public Key and Sending
		ModularArithmetic mod(N);
		Integer V=mod.MultiplicativeInverse(Se);
		//cout<<"N:E:S:V"<<Nu<<" "<<Ee<<" "<<Se<<" "<<V<<" $\n";
		ll mmmm = V.ConvertToLong();
		bzero(buffer,256);
		public_variables = to_string(mmmm);
		strcpy(buffer,public_variables.c_str());
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
		cout<<"Sent Public Key 'v' "<< V <<" to the Verifier" << endl;
		//End Calculating public Key and Sending

		cout << "Registration Complete" << endl;
		//Start Calculating witness
		Integer R(ra,1,N-1);
		Integer Re = a_exp_b_mod_c(R,Ee,Nu);
		//cout<<Re<<"HoldUp\n";
		//End Calculating witness

		//Start Sending Witness
		mmmm = Re.ConvertToLong();
		bzero(buffer,256);
		public_variables = to_string(mmmm);
		strcpy(buffer,public_variables.c_str());
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
		cout<<"Sent Witness 'x' "<< Re <<" to the Verifier" << endl;
		//End Sending Witness

		//Start Receiving Challenge
		bzero(buffer,256);
		n = read(sockfd, buffer, 255);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}

		string challenge(buffer);
		ll C = stoi(challenge);
		Integer CH = Integer(C);
		cout<<"Recieved Challenge 'c' "<< CH <<" from the Verifier" << endl;
		//End Receiving Challenge

		//Start Calculating Response
		Integer Temp = a_exp_b_mod_c(S,CH,Nu);
		Integer Y =mod.Multiply(Temp,R);
		//End Calculating Response

		//Start Sending Response
		mmmm = Y.ConvertToLong();
		bzero(buffer,256);
		public_variables = to_string(mmmm);
		strcpy(buffer,public_variables.c_str());
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
		cout<<"Sent response 'y' "<< Y <<" to the Verifier" << endl;
		//End Sending Response

		int ip = 0;
		cout<<"Want to Renew Registration : <1 : Renew | 2 : Close>"<<endl;
		cin>>ip;
		if (ip == 2) {
			break;
		}
	}
	return 0;
}
