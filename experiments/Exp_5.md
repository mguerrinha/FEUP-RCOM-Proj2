# Procedimentos para a Exp5 

# No TUX2, TUX3 e TUX4 editar o arquivo /etc/resolv.conf
sudo nano /etc/resolv.conf

# Adicionando a seguinte linha (em todos)
nameserver 10.227.20.3

# Podemos verificar se foi bem adicionado com:
cat /etc/resolv.conf # se não houver permissões suficientes, tentar com o sudo

# No terminal de cada TUX(2,3 e 4) tentar dar ping do google para verificar se podem ser usados nomes como host
ping google.com # Isto irá enviar pacotes ICMP para o nome de domínio e, se o DNS estiver configurado corretamente, os pacotes serão resolvidos

# Podemos também testar com o navegador, abrindo uma página qualquer como o Google por exemplo

# Iniciar captura no Wireshark dos TUXs (suponho que seja no eth1 pois é o que temos vindo a configurar) e verificar os packets relacionados ao DNS
# Usar o ping google.com (e podemos até usar o filtro `dns` --> barra de filtros de captura)