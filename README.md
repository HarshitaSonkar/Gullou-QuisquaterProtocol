# Gullou-Quisquater Protocol
Gullou-Quisquater Zero-Knowledge Authentication Protocol.
Implementation of Gullou-Quisquater Zero-Knowledge Authentication entity authentication protocol between two physical computers.
One computer act as the User (Claimant) and the other-one act as Authentication Server (Verifier). 
The protocol have three modules, 
1) Registration Module: using which a new user can register itself to the server and 
2) Authentication Module: using which a registered user can authenticate itself to the server, 
3) Renewal Module: using which a registered user can renew it’s security credentials (passwords, symmetric keys, public keys etc.).

Measure of average End-to-End Authentication Delay for authentication module. 
End-to-End Authentication Delay is defined as the duration of time starting from the instant when the user initiates the
authentication process to the instant when the user gets the authentication Success/Failure message from
the verifier.


g++ verifier.cpp -l:libcryptopp.a -std=c++11 -o verifier
g++ claimant.cpp  -l:libcryptopp.a -std=c++11 -o claimant
./verifier
./claimant 127.0.0.1 5001

