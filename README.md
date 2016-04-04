# NetControl

O NetControl é um painel de controle/configuração web autônomo (standalone) em Python que fornece um sistema de gerenciamento de servidores Debian GNU/Linux, além da autenticação/armazenamento-de-arquivos em qualquer estação (Windows e derivados de GNU/Linux) compatível com openLDAP.

Através de seus módulos é possível configurar os serviços do servidor pela Web. Sendo necessário somente um navegador web (recomendado: Firefox) e a liberação do IP de acesso. Sua interface web é simplificada e fornece seus recursos de forma fácil para os administradores de sistemas.

Dentre seus módulos, o de maior destaque é o LDAP, que permite que estações GNU/Linux e Windows funcionem sob o mesmo servidor LDAP (para autenticação de usuário) e de arquivos. Olhe a Figura 28 à 34.

Suas telas são apresentadas a seguir.

*NOTA: Seu desenvolvimento ocorreu dentre 2011 à 2012. Funciona corretamente com a versão Debian Squeeze e precisa de atualização. Estou procurando contribuidores com o projeto. :)*

### Tela de Acesso Inicial

|[![alt text](screenshots/01-Tela_inicial.png)](screenshots/01-Tela_inicial.png)|
:-------------:
|**Fig. 01. Tela inicial**|

|[![alt text](screenshots/02-Tela_de_autenticacao.png)](screenshots/02-Tela_de_autenticacao.png)|
:-------------:
|**Fig. 02. Tela de autenticacao**|

### Configurações de Acesso à Web

|[![alt text](screenshots/03-acesso_blok.png)](screenshots/03-acesso_blok.png)|
:-------------:
|**Fig. 03. Acesso-bloqueio**|

|[![alt text](screenshots/04-acesso_users.png)](screenshots/04-acesso_users.png)|
:-------------:
|**Fig. 04. Acesso-Usuarios**|

|[![alt text](screenshots/05-novo_sites_block.png)](screenshots/05-novo_sites_block.png)|
:-------------:
|**Fig. 05. Acesso-Bloqueio-Sites**|

|[![alt text](screenshots/06-novo_sites_lib.png)](screenshots/06-novo_sites_lib.png)|
:-------------:
|**Fig. 06. Acesso-Liberação-Sites**|

### Configuração do Apache

|[![alt text](screenshots/07-apache.png)](screenshots/07-apache.png)|
:-------------:
|**Fig. 07. Configuração do apache**|

|[![alt text](screenshots/10-apache.png)](screenshots/10-apache.png)|
:-------------:
|**Fig. 10. Configuração do Apache-02**|

### Backup do servidor

|[![alt text](screenshots/08-backup-01.png)](screenshots/08-backup-01.png)|
:-------------:
|**Fig. 08. Backup-01**|

|[![alt text](screenshots/09-backup-02.png)](screenshots/09-backup-02.png)|
:-------------:
|**Fig. 09. Backup-02**|

### Utilização do Cacic

|[![alt text](screenshots/11-cacic.png)](screenshots/11-cacic.png)|
:-------------:
|**Fig. 11. Cacic**|

### Configuração do DNS

|[![alt text](screenshots/12-DNS-01.png)](screenshots/12-DNS-01.png)|
:-------------:
|**Fig. 12. DNS-01**|

|[![alt text](screenshots/13-DNS-02.png)](screenshots/13-DNS-02.png)|
:-------------:
|**Fig. 13. DNS-02**|

|[![alt text](screenshots/14-DNS-03.png)](screenshots/14-DNS-03.png)|
:-------------:
|**Fig. 14. DNS-03**|

|[![alt text](screenshots/15-DNS-04.png)](screenshots/15-DNS-04.png)|
:-------------:
|**Fig. 15. DNS-04**|

|[![alt text](screenshots/16-DNS-05.png)](screenshots/16-DNS-05.png)|
:-------------:
|**Fig. 16. DNS-05**|

|[![alt text](screenshots/17-DNS-06.png)](screenshots/17-DNS-06.png)|
:-------------:
|**Fig. 17. DNS-06**|

|[![alt text](screenshots/18-DNS-07.png)](screenshots/18-DNS-07.png)|
:-------------:
|**Fig. 18. DNS-07**|

|[![alt text](screenshots/19-DNS-08.png)](screenshots/19-DNS-08.png)|
:-------------:
|**Fig. 19. DNS-08**|

|[![alt text](screenshots/20-DNS-10.png)](screenshots/20-DNS-10.png)|
:-------------:
|**Fig. 20. DNS-10**|

### Configuração de energia

|[![alt text](screenshots/21-energia.png)](screenshots/21-energia.png)|
:-------------:
|**Fig. 21. Energia**|

### Estações que possuem o cliente do NetControl

|[![alt text](screenshots/22-estacoes.png)](screenshots/22-estacoes.png)|
:-------------:
|**Fig. 22. Estacoes**|

### Configuração do Firewall (IPTables)

|[![alt text](screenshots/23-firewall.png)](screenshots/23-firewall.png)|
:-------------:
|**Fig. 23. Firewall-01**|

|[![alt text](screenshots/24-firewall1.png)](screenshots/24-firewall1.png)|
:-------------:
|**Fig. 24. Firewall-02**|

### Configuração do FTP

|[![alt text](screenshots/25-ftp.png)](screenshots/25-ftp.png)|
:-------------:
|**Fig. 25. FTP**|

### Gerenciador de arquivos

|[![alt text](screenshots/26-gerenciador-arquivos.png)](screenshots/26-gerenciador-arquivos.png)|
:-------------:
|**Fig. 26. Gerenciador de arquivos**|

### Gerenciador de tarefas

|[![alt text](screenshots/27-gerenciador-tarefas.png)](screenshots/27-gerenciador-tarefas.png)|
:-------------:
|**Fig. 27. Gerenciador de tarefas**|

### Configuração automática do LDAP

|[![alt text](screenshots/28-LDAP-01.png)](screenshots/28-LDAP-01.png)|
:-------------:
|**Fig. 28. LDAP-01**|

|[![alt text](screenshots/29-LDAP-02.png)](screenshots/29-LDAP-02.png)|
:-------------:
|**Fig. 29. LDAP-02**|

|[![alt text](screenshots/30-LDAP-03.png)](screenshots/30-LDAP-03.png)|
:-------------:
|**Fig. 30. LDAP-03**|

|[![alt text](screenshots/31-LDAP-04.png)](screenshots/31-LDAP-04.png)|
:-------------:
|**Fig. 31. LDAP-04**|

### Configuração dos usuários do LDAP

|[![alt text](screenshots/32-LDAP_USUARIOS-01.png)](screenshots/32-LDAP_USUARIOS-01.png)|
:-------------:
|**Fig. 32. LDAP-Usuários-01**|

|[![alt text](screenshots/33-LDAP_USUARIOS-02.png)](screenshots/33-LDAP_USUARIOS-02.png)|
:-------------:
|**Fig. 33. LDAP-Usuários-02**|

|[![alt text](screenshots/34-LDAP_USUARIOS-03.png)](screenshots/34-LDAP_USUARIOS-03.png)|
:-------------:
|**Fig. 34. LDAP-Usuários-03**|

### Registros (Logs) do sistema

|[![alt text](screenshots/35-Logs.png)](screenshots/35-Logs.png)|
:-------------:
|**Fig. 35. Logs**|

### Configuração de terminal remoto

|[![alt text](screenshots/36-LTSP-01.png)](screenshots/36-LTSP-01.png)|
:-------------:
|**Fig. 36. LTSP-01**|

|[![alt text](screenshots/37-LTSP-02.png)](screenshots/37-LTSP-02.png)|
:-------------:
|**Fig. 37. LTSP-02**|

|[![alt text](screenshots/38-LTSP-03.png)](screenshots/38-LTSP-03.png)|
:-------------:
|**Fig. 38. LTSP-03**|

### Acesso ao Munin

|[![alt text](screenshots/39-munin.png)](screenshots/39-munin.png)|
:-------------:
|**Fig. 39. Munin**|

### Pacotes do Sistema

|[![alt text](screenshots/40-pacotes-01.png)](screenshots/40-pacotes-01.png)|
:-------------:
|**Fig. 40. Pacotes-01**|

### Configuração da Rede

|[![alt text](screenshots/43-rede-01.png)](screenshots/41-rede-01.png)|
:-------------:
|**Fig. 43. Rede-01**|

|[![alt text](screenshots/41-rede-02.png)](screenshots/42-rede-02.png)|
:-------------:
|**Fig. 41. Rede-02**|

|[![alt text](screenshots/42-rede-03.png)](screenshots/43-rede-03.png)|
:-------------:
|**Fig. 42. Rede-03**|

### Exportação de arquivos com o Samba

|[![alt text](screenshots/46-samba.png)](screenshots/46-samba.png)|
:-------------:
|**Fig. 46. Samba**|

### Serviços do Sistema

|[![alt text](screenshots/47-servicos.png)](screenshots/47-servicos.png)|
:-------------:
|**Fig. 47. Serviços do sistema**|

### Sistemas de arquivos (fstab)

|[![alt text](screenshots/48-sist.arquivos.png)](screenshots/48-sist.arquivos.png)|
:-------------:
|**Fig. 48. Sistema de Arquivos**|

### Terminal online

|[![alt text](screenshots/49-terminal.png)](screenshots/49-terminal.png)|
:-------------:
|**Fig. 49. Terminal Online**|

### Configuração dos usuários locais

|[![alt text](screenshots/50-usuarios_local.png)](screenshots/50-usuarios_local.png)|
:-------------:
|**Fig. 50. Usuários Locais**|

## Configuração do WebMail

|[![alt text](screenshots/51-webmail-01.png)](screenshots/51-webmail-01.png)|
:-------------:
|**Fig. 51. Configuração do Webmail-01**|

|[![alt text](screenshots/52-roundcube-01.png)](screenshots/52-roundcube-01.png)|
:-------------:
|**Fig. 52. Acesso ao webmail (Roundcube-01)**|

|[![alt text](screenshots/53-roundcube-02.png)](screenshots/53-roundcube-02.png)|
:-------------:
|**Fig. 53. Acesso ao webmail (Roundcube-02)**|
