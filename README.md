# INF473X
Course provided by Ecole Polytechnique on cybersecurity and hacking tehcniques

## TCP-SYN-flooding
A demostration of TCP SYN flooding  
1. Download the destination virtual machine
    - [VM1](https://www.enseignement.polytechnique.fr/informatique/INF473X/hack-sigma-lubuntu-64.ova)  
      IP address 192.168.56.101, server port 2000, password `openpwd`  
    or
    - [VM2](https://www.enseignement.polytechnique.fr/informatique/INF473X/hack-sigma-lubuntu-64-additional.ova)
      IP address 192.168.56.102, password `openpwd`  
    in case your host does not support raw IP sockets;
2. Run command

        make 
        ./syn_flood

3. Enter 1 or 2.  
   The server should become unresponsive from a legitimate client connection, with a growth of the number of half-open connections.
