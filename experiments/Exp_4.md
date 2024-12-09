# Procedimentos para a Exp4

# Conectar o ether1 do Rc (Router) ao PY.12  --> NAT enabled por default
# Conectar o ether2 do Rc a uma porta na bridge111 --> Ou seja, temos que ligar o ether2 do Router a uma porta qualquer do Switch e configurar a mesma para estar conectada à bridgeY1

# Neste caso devemos estar no GKTerm do Switch (Preferencialmente TUX3 e baudrate 115200)
/interface bridge port remove [find interface=etherN] # Porta N do Switch (pode ser qualquer uma das que não utilizamos dos passos anteriores)
/interface bridge port add bridge=bridgeY1 interface=etherN

# Para configurarmos o IP Address do Rc temos de nos conectar à consola do Router (Temos de trocar o cabo que está conectado à consola do Switch para a consola do Router)
# No TUX3 (provavelmente), conectamo-nos ao Router usando o GKTerm
Serial Port: /dev/ttyS0
Baudrate: 115200
Username: admin
Password: <ENTER>

# Resetamos as configurações do Router
/system reset-configuration

# Configurar o IP Address do Rc pela consola do Router
/ip address add address=172.16.1.Y9/24 interface=ether1
/ip address print
/ip address add address=172.16.Y1.254/24 interface=ether2

# Configurar as routes defaults nos TUXs e no Router
# No caso do TUX3, o default será o TUX4
route add default gw 172.16.Y0.254

# No caso do TUX2 e TUX4, o Rc será o default
route add default gw 172.16.Y1.254

# No TUX2 adicionar route para 172.16.Y0.0/24 --> Acredito que já exista, porém devemos verificar
route -n
route add -net 172.16.Y0.0/24 gw 172.16.Y1.253 # em caso de não existir

# No caso do Router, devemos configurar na consola para chegar a 172.16.Y0.0/24
/ip route add dst-address=172.16.Y0.0/24 gateway=172.16.Y1.253
/ip route add dst-address=0.0.0.0/0 gateway=172.16.1.254 # Default gateway

# No TUX3, começar uma captura no Wireshark e para ping para o TUX2, TUX4 e Rc --> supostamente todas devem funcionar
ping 172.16.Y0.254 # para o TUX4
ping 172.16.Y1.1 # para o TUX2
ping 172.16.Y1.254 # para o Rc

# No TUX2
echo 0 > /proc/sys/net/ipv4/conf/eth0/accept_redirects | sysctl net.ipv4.conf.eth0.accept_redirects=0
echo 0 > /proc/sys/net/ipv4/conf/all/accept_redirects | sysctl net.ipv4.conf.all.accept_redirects=0 

# Remover a route até 172.16.Y0.0/24 via TUX4
route del -net 172.16.Y0.0 gw 172.16.Y1.253 netmask 255.255.255.0 # Talvez dê route del -net 172.16.Y0.0/24 gw 172.16.Y1.253 (Esta versão NÃO TENHO A CERTEZA)
route -n

# Ainda no TUX2, começar uma captura no Wireshark e dar ping para o TUX3, apesar de já não termos TUX4 como gateway, a ligação SUPOSTAMENTE é estabelecida usando Rc como Router
ping 172.16.Y0.1 # Analisar a captura para entender o caminho feito pelos packets ICMP ECHO e ECHO-REPLY

# Fazemos traceroute para poder verificar o percurso da ligação do TUX2 para o TUX3
traceroute -n 172.16.Y0.1

# Adicionar de novo a route que liga o TUX2 ao TUX4
route add -net 172.16.Y0.0/24 gw 172.16.Y1.253

# Damos traceroute novamente no TUX2 para o TUX3
traceroute -n 172.16.Y0.1

# Ainda no TUX2, quando NÃO existir uma route para 172.16.Y0.0/24 via TUX4, devemos ativar o acceptance of ICMP redirect e averiguar o que acontece
# Neste caso antes de voltar a adicionar a route via TUX4, tentamos com e sem accept_redirects
echo 1 > /proc/sys/net/ipv4/conf/eth0/accept_redirects / sysctl net.ipv4.conf.eth0.accept_redirects=1
echo 1 > /proc/sys/net/ipv4/conf/all/accept_redirects / sysctl net.ipv4.conf.all.accept_redirects=1

# No TUX3 fazer ping para o FTP server para verificar a ligação
ping 172.16.1.10

# Desativar o NAT do Router
/ip firewall nat disable 0

# No TUX3 voltamos a dar ping para 172.16.1.10 --> Desta vez com o NAT desligado, não deverá existir ligação
ping 172.16.1.10

# Reativar Nat do Router
/ip firewall nat enable 0