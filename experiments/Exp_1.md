# Procedimentos para a Exp1 no TUX3

# Retirar todos os cabos, exceto os pretos, colocando-os nos lugares pretendidos
# Ter também em conta onde está posicionado o S0 (de preferência deve estar posicionado para ser utilizado no TUX3)

# Reiniciar o serviço de rede
systemctl restart networking

# Ativar a interface eth1
ifconfig eth1 up

# Configurar o IP da interface eth1 do TUX3
ifconfig eth1 172.16.Y0.1/24

# Ver o endereço MAC associado à interface eth1
ifconfig eth1

# Testar conectividade com o TUX4 (IP 172.16.Y0.254)
ping 172.16.Y0.254 -c 20

# Verificar a tabela de rotas
route -n

# NOTA: A partir deste ponto, na Exp1, apenas deve ser feito no TUX3!
# Verificar tabela ARP
arp -a 

# Remover entrada ARP específica para TUX4 (ele referem no guião de eliminar as entries, mas não referem se são todas ou apenas esta)
arp -d 172.16.Y0.254
arp -a

# Abrir o WireShark no TUX3 para capturar pacotes (no eth1)

# Testar a conectividade novamente (com o TUX4) enquanto captura pacotes
ping 172.16.Y0.254 -c 10

# Parar a captura e avaliar os logs obtidos no WireShark
# Responder perguntas pós-experiência com base nos resultados


# Procedimentos para a Exp1 no TUX4

# Retirar todos os cabos, exceto os pretos, colocando-os nos lugares pretendidos
# Ter também em conta onde está posicionado o S0 (de preferência deve estar posicionado para ser utilizado no TUX3)

# Reiniciar o serviço de rede
systemctl restart networking

# Ativar a interface eth1
ifconfig eth1 up

# Configurar o IP da interface eth1 do TUX4
ifconfig eth1 172.16.Y0.254/24

# Ver o endereço MAC associado à interface eth1
ifconfig eth1

# Testar conectividade com o TUX3 (IP 172.16.Y0.1)
ping 172.16.Y0.1 -c 20

# Verificar a tabela de rotas
route -n

# NOTA: Restantes passos devem ser efetuados no TUX3!
# Responder perguntas pós-experiência com base nos resultados