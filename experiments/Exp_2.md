# Procedimentos para a Exp2

# Verificar qual é o S0 conectado à console do Switch (se for o S0 do TUX3 conectado, deveremos utilizar o TUX3 para a experiência, em caso de não funcionar no GKTerm, deveremos conectar a console do Switch a outro S0)
# Quando retiramos e voltamos a introduzir os cabos na Exp1, deveríamos ter o cuidado de verificar antecipadamente se o S0 já estaria associado ao TUX3

# Abrir o GKTerm no TUX selecionado e configurar a baudrate para 115200

# Resetar as configurações do Switch
admin
/system reset-configuration
y

# Ligar o cabo do E1 do TUX2 a uma porta do Switch e ter em conta o E1 tanto do TUX3 como do TUX4 (importante para depois configurar as bridges)
# No TUX2 configurar o seu E1 e registar o seu IP/MAC address
systemctl restart networking
ifconfig eth1 up
ifconfig eth1 172.16.Y1.1/24

# Voltar ao TUX associado ao S0 (em princípio o TUX3) e criar 2 bridges no Switch através do GKTerm
/interface bridge add name=bridgeY0
/interface bridge add name=bridgeY1

# Eliminar as portas, N, às quais o TUX2, 3 e 4 estão ligados (o número do ether é relativo à porta do Switch que cada TUX está conectado)
/interface bridge port remove [find interface=etherN]
/interface bridge port remove [find interface=etherN] 
/interface bridge port remove [find interface=etherN]

# Adicionar as novas portas (TUX3 e TUX4 - bridge0 / TUX2 - bridge1)
/interface bridge port add bridge=bridge110 interface=etherN # etherN = TUX3
/interface bridge port add bridge=bridge110 interface=etherN # etherN = TUX4
/interface bridge port add bridge=bridge111 interface=etherN # etherN = TUX2

# Verificar que as portas foram adicionadas corretamente
/interface bridge port print (brief)

# Começar a captura no eth1 do TUX3

# No TUX3, fazer ping para o TUX4 (é esperado uma resposta) e para o TUX2 (é esperado que dê unreachable)
ping 172.16.Y0.254 # ping para o TUX4
ping 172.16.Y1.1 # ping para o TUX2

# Parar a captura e salvar as logs (ao invés de salvar com prints, podemos também começar a salvar o próprio documento de captura)

# Começar novas capturas no eth1 do TUX2, TUX3 e TUX4
# No TUX3 fazer ping broadcast por alguns segundos, observar os resultados, parar as capturas e salvar as logs
ping -b 172.16.Y0.255

# Mesmos passos que anteriormente, começar novas capturas no eth1 do TUX2, TUX3 e TUX4, porém desta vez o ping broadcast deve ser feito no TUX2, observar também os resultados, parar as capturas e salvar as logs
ping -b 172.16.Y1.255

NOTA: No broadcast, apenas os dispositivos da mesma bridge devem receber os pacotes (no caso entre o TUX3 e o TUX4).