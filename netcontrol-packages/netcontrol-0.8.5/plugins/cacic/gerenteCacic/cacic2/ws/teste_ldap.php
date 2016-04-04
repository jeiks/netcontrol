<? 
require_once('../include/library.php');

		// Identificador para Autenticação no Servidor de Autenticação
		$id_servidor_autenticacao	  	= '2'; 
		$nm_nome_acesso_autenticacao  	= 'anderson.peterle'; 
		$te_senha_acesso_autenticacao 	= 'mesj07012345'; 
		
			$arrServidores = getValores('servidores_autenticacao',  
										'nm_servidor_autenticacao,
										te_ip_servidor_autenticacao,
										id_tipo_protocolo,
										nu_versao_protocolo,
										te_base_consulta_raiz,
										te_base_consulta_folha,
										te_atributo_identificador,
										te_atributo_retorna_nome,
										te_atributo_retorna_email'   , 
										'id_servidor_autenticacao = '.$id_servidor_autenticacao.' AND in_ativo="S"');
		
			/*	
			
			// APLICACAO DO BLOCO DE CODIGO DO JARBAS PEIXOTO - URMS
			// 1 - Fazer a pesquisa no OpenLDAP 
			// 2 - Contar os resultados
			// 2.1 - Caso resultado = 0 ERRO
			// 2.2 - Caso resultado > 1 ERRO
			// 2.3 - Caso resultado = 1 OK
			// 3 - No script, acontece a obtencao do DN do unico usuario encontrado
			// 4 - A partir do DN obtido, realizar o BIND.
			*/
		
			// Comunicação com o servidor de Autenticação
	
			$te_atributo_retorna_nome	= $arrServidores['te_atributo_retorna_nome'];
			$te_atributo_retorna_email	= $arrServidores['te_atributo_retorna_email'];		
			$te_host 					= $arrServidores['nm_servidor_autenticacao'];
	
			$ldap = ldap_connect($te_host,389);
			
			ldap_set_option($ldap, LDAP_OPT_PROTOCOL_VERSION, $arrServidores['nu_versao_protocolo']);
			//GravaTESTES('SetSession: Após SET_OPTION => '.ldap_error($ldap)); 			
			
			//ldap_start_tls($ldap);
	
			if (ldap_errno($ldap) == 0) 
				{
				//GravaTESTES('SetSession: Tentando BIND Identificado com "'.$nm_nome_acesso_autenticacao.' / **********"');
					
				$bindTesteAnonimo=ldap_bind($ldap);    // this is an "anonymous" bind, typically
							   // read-only access
				//GravaTESTES("Bind Anônimo retornou " . $bindTesteAnonimo);
					
				//ob_start();
				$strBind = $arrServidores['te_atributo_identificador'].'='.$nm_nome_acesso_autenticacao.','.$arrServidores['te_base_consulta_folha'].','.$arrServidores['te_base_consulta_raiz'];
				//GravaTESTES('SetSession: strBind => "'.$strBind);				
				$bind = ldap_bind($ldap, "$strBind", $te_senha_acesso_autenticacao);
				//ob_end_flush();
				
				if (ldap_errno($ldap) == 0)
					{
					$strRootDN = $arrServidores['te_base_consulta_raiz'];
					$strNodeDN = $arrServidores['te_base_consulta_folha'];
					//GravaTESTES('SetSession: Preparando Search : '.'"'.$strNodeDN.','.$strRootDN.'" / '.'"'.$arrDominios['te_atributo_identificador'].'='.$nm_nome_acesso_dominio.'"');
					$searchResults = ldap_search($ldap, "$strNodeDN,$strRootDN",$arrServidores['te_atributo_identificador'].'='.$nm_nome_acesso_autenticacao);
				
					//GravaTESTES('SetSession: Após SEARCH => '.ldap_error($ldap));		
					// OK! Dados encontrados!
					if (!$searchResults === false)
						{
						$arrLDAPdata = array();
						$ldapResults = ldap_get_entries($ldap, $searchResults);
						for ($item = 0; $item < $ldapResults['count']; $item++) 
							{
							for ($attribute = 0; $attribute < $ldapResults[$item]['count']; $attribute++) 
								{
								$data = $ldapResults[$item][$attribute];
								$arrLDAPdata[$data]=$ldapResults[$item][$data][0];            									GravaTESTES('data => "'.$data.'"');
									GravaTESTES('arrLDAPdata['.$data.'] => "'.$arrLDAPdata[$data].'"');
								}
							}
					
						//GravaTESTES('SetSession: Após GET_ENTRIES => '.ldap_error($ldap));		
						$nm_nome_completo  		= $arrLDAPdata[strtolower($arrServidores['te_atributo_retorna_nome'])];					
						$te_email 				= $arrLDAPdata[strtolower($arrServidores['te_atributo_retorna_email'])];					
						//GravaTESTES('SetSession: Após GET_ENTRIES => nm_nome_completo: '.$nm_nome_completo);							
						//GravaTESTES('SetSession: Após GET_ENTRIES => te_email: '.$te_email);												
						
						}
					}
				}
?>
