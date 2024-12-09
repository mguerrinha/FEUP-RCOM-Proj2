# Procedimentos para a Exp3

# Configurar e conectar o eth2 do TUX4
ifconfig eth2 up
ifconfig eth2 172.16.Y1.253/24

# Adicionar o eth2 do TUX4 à bridge1
/interface bridge port remove [find interface=etherN] # etherN = porta N no Switch conectado ao eth2 do TUX4
/interface bridge port add bridge=bridgeN1 interface=etherN

# No TUX4, ativar ip forwarding e desativar ICMP
echo 1 > /proc/sys/net/ipv4/ip_forward

echo 0 > /proc/bridgeY0sys/net/ipv4/icmp_echo_ignore_broadcasts

# Observar MAC e IP no TUX4-eth1 e TUX4-eth2
ifconfig

# Adicionar as seguintes rotas no TUX2 e TUX3 para que estes se consigam altançar um ou outro através do TUX4

# No TUX2: (Para chegar ao TUX3 usamos como gateway o TUX4)
route add -net 172.16.Y0.0/24 gw 172.16.Y1.253

# No TUX3: (Para chegar ao TUX2 usamos como gateway o TUX4)
route add -net 172.16.Y1.0/24 gw 172.16.Y0.254

# Verificar as rotas em todos os TUX
route -n

# Começar a capturar no Wireshark do TUX3

# Fazer ping para os seguintes addresses a partir do TUX3
ping 172.16.Y0.254
ping 172.16.Y1.253
ping 172.16.Y1.1

# Parar a captura e salvar os logs

# Começar a captura no TUX4 tanto do eth1 como do eth2 (2 Interfaces)

# Limpar as tabelas ARP nos 3 TUXes
arp -d 172.16.Y1.1 # no TUX4
arp -d 172.16.Y0.1 # no TUX4
arp -d 172.16.Y0.254 # no TUX3
arp -d 172.16.Y1.253 # no TUX2

# No TUX3, fazer ping para o TUX2 durante alguns segundos
ping 172.16.Y1.1

# Parar as capturas no TUX4 e salvar as logs