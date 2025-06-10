# 🏥 SCAM - Sistema de Controle e Agendamento Médico

**Disciplina:** Programação Orientada a Objetos  
**Curso:** Sistemas de Informação  
**Instituição:** Universidade Estadual de Goiás  
**Data:** 13/05/2025  
**Objetivo:** Trabalho acadêmico para avaliação na disciplina de POO, sem fins comerciais.

---

## 📌 Sumário

- [Descrição do Projeto](#descrição-do-projeto)
- [Diagramas de Caso de Uso](#diagramas-de-caso-de-uso)
  - [Secretária](#secretária)
  - [Médico](#médico)
  - [Paciente](#paciente)
- [Diagrama de Classes](#diagrama-de-classes)
- [Requisitos do Sistema](#requisitos-do-sistema)
  - [Requisitos Funcionais](#requisitos-funcionais)
  - [Requisitos Não Funcionais](#requisitos-não-funcionais)
- [Especificações do Trabalho](#especificações-do-trabalho)
- [Definições](#definições)
- [Linguagem Utilizada](#linguagem-utilizada)

---

## 📖 Descrição do Projeto

O SCAM é um sistema desenvolvido com o objetivo de gerenciar o controle e agendamento de consultas médicas em uma clínica. O projeto foi elaborado para fins acadêmicos, aplicando os conceitos de Programação Orientada a Objetos.

---

## 🎯 Diagramas de Caso de Uso

### Secretária

![Diagrama de Caso de Uso - Secretária](https://github.com/user-attachments/assets/8b24264e-358f-478d-9b8b-dd8754a70796)

### Médico

![Diagrama de Caso de Uso - Médico](https://github.com/user-attachments/assets/58a72b11-ec83-4f80-9990-366077663517)

### Paciente

![Diagrama de Caso de Uso - Paciente](https://github.com/user-attachments/assets/2c533c70-24f1-467d-ad64-13a62d676434)

---

## 🧩 Diagrama de Classes

![Diagrama de Classes](https://github.com/user-attachments/assets/438abc7c-bdc6-4119-8b2d-d35f30e6ad3c)

---

## ✅ Requisitos do Sistema

### Requisitos Funcionais

- **[RQ-01]**: O sistema deve permitir o cadastro de secretárias, médicos e pacientes, incluindo informações como CPF, nome, e-mail, telefone e WhatsApp.
- **[RQ-02]**: Permitir o cadastro do número de matrícula, cargo (secretária, secretária-supervisora) e médicos assessorados por cada secretária.
- **[RQ-03]**: Somente a secretária supervisora pode cadastrar outras secretárias.
- **[RQ-04]**: Cada secretária pode assessorar no máximo três médicos.
- **[RQ-05]**: Apenas a secretária supervisora pode consultar as secretárias.
- **[RQ-06]**: Cada secretária pode cadastrar e consultar apenas os médicos que assessora.
- **[RQ-07]**: Cada secretária pode cadastrar e consultar apenas os pacientes dos médicos que assessora.
- **[RQ-08]**: Cada secretária pode cadastrar e consultar apenas a agenda dos médicos que assessora.
- **[RQ-09]**: Cada secretária pode cadastrar e consultar apenas a agenda dos pacientes dos médicos que assessora.
- **[RQ-10]**: As funcionalidades de cadastro devem permitir inclusão, alteração e exclusão das entidades relacionadas.
- **[RQ-11]**: O sistema deve permitir apenas três tentativas de autenticação inválidas.

## Requisitos Não Funcionais do Sistema SCAM

Esta análise detalha as características de qualidade e as restrições operacionais do sistema SCAM, com base na sua implementação em C++.
1. Segurança

Os requisitos de segurança definem como o sistema protege dados e funcionalidades contra acessos não autorizados.

    RNF-01: Controle de Acesso Baseado em Papel (RBAC)

        Descrição: O sistema implementa diferentes níveis de permissão para diferentes tipos de usuários. Uma Secretária-Supervisora possui mais privilégios que uma Secretária comum, que por sua vez tem mais privilégios que um Médico ou Paciente.

        Implementação: O código verifica o atributo cargo da secretária logada antes de permitir o acesso a funcionalidades críticas (ex: cadastrarSecretaria) e valida se uma secretária assessora um médico específico antes de permitir operações em sua agenda ou a de seus pacientes ([RQ-03], [RQ-05], [RQ-06], [RQ-07], [RQ-08], [RQ-09]).

    RNF-02: Autenticação Obrigatória

        Descrição: Nenhum usuário pode acessar as funcionalidades internas do sistema sem antes fornecer um login e senha válidos.

        Implementação: A função main direciona todos os fluxos de usuário para a função autenticar antes de exibir qualquer menu de funcionalidades.

    RNF-03: Proteção Contra Ataque de Força Bruta

        Descrição: O sistema limita o número de tentativas de login malsucedidas para mitigar o risco de ataques de adivinhação de senha.

        Implementação: A função autenticar implementa um laço for que permite no máximo 3 tentativas. Após a terceira falha, o acesso é bloqueado e a função retorna nullptr.

    RNF-04: Persistência de Dados Não Criptografada

        Descrição: Os dados dos usuários, consultas, etc., são armazenados em arquivos binários. Este formato não é legível por humanos, o que oferece uma camada de ofuscação, mas não criptografia. As senhas são armazenadas em texto plano dentro desses arquivos.

        Implementação: A biblioteca Cereal é usada para serializar os objetos diretamente para arquivos .bin (cereal::BinaryOutputArchive).

2. Usabilidade

A usabilidade descreve a facilidade com que um usuário pode interagir com o sistema.

    RNF-05: Interface de Linha de Comando (CLI)

        Descrição: A interação com o sistema ocorre inteiramente por meio de uma interface de texto, baseada em menus e entrada de dados pelo teclado.

        Implementação: O sistema utiliza std::cout para exibir menus e informações, e std::cin para capturar a entrada do usuário.

    RNF-06: Busca por Nome em Vez de Código

        Descrição: Para melhorar a experiência do usuário e reduzir a carga cognitiva, o sistema permite que a secretária busque médicos e pacientes pelo nome em vez de exigir a memorização de códigos numéricos.

        Implementação: A função encontrarCodigoPorNome foi criada para centralizar essa lógica de busca, sendo utilizada em todas as funcionalidades relevantes (cadastro de paciente, agendamento de consulta, consulta de agenda).

    RNF-07: Navegação Guiada e Feedback ao Usuário

        Descrição: O sistema apresenta ao usuário uma lista clara de opções em cada etapa (navegação por menus) e fornece respostas imediatas para cada ação, seja ela bem-sucedida ou não.

        Implementação: Os menus usam estruturas switch-case para direcionar o usuário, e mensagens de sucesso ("Paciente cadastrado com sucesso!"), erro ("ACESSO NEGADO: ..."), e status ("Fazendo logout...") são exibidas constantemente.

3. Confiabilidade e Robustez

Estes requisitos tratam da capacidade do sistema de funcionar corretamente e de lidar com erros.

    RNF-08: Persistência de Dados na Saída

        Descrição: Os dados devem ser salvos de forma a garantir sua durabilidade entre as sessões. O salvamento ocorre de forma atômica no final da execução.

        Implementação: Os dados são salvos nos arquivos .bin apenas quando o usuário sai do programa de forma controlada (opção "0" no menu principal). Isso implica que, em caso de uma falha abrupta (queda de energia, crash), as alterações da sessão atual serão perdidas.

    RNF-09: Tratamento de Entrada Inválida

        Descrição: O sistema deve ser capaz de lidar com entradas de dados em formato inesperado (texto onde se espera número, etc.) e com nomes não encontrados sem travar.

        Implementação: A função limparBufferEntrada é usada após a leitura de números para evitar erros de fluxo. A função encontrarCodigoPorNome inclui um laço e a opção "cancelar" para lidar de forma robusta com buscas malsucedidas.

4. Portabilidade

A portabilidade refere-se à capacidade do software de ser executado em diferentes ambientes.

    RNF-10: Independência de Plataforma

        Descrição: O código-fonte deve ser compilável e executável nos principais sistemas operacionais (Windows, macOS, Linux) sem a necessidade de alterações.

        Implementação: O uso exclusivo de bibliotecas padrão do C++ (iostream, vector, string, etc.) e da biblioteca Cereal (que é cross-platform e header-only) garante alta portabilidade. Não há chamadas a APIs específicas de um sistema operacional.

5. Manutenibilidade

Refere-se à facilidade com que o código pode ser entendido, modificado e estendido.

    RNF-11: Modularidade e Coesão

        Descrição: O código é organizado em componentes lógicos distintos (classes para entidades, funções para funcionalidades) para facilitar a manutenção.

        Implementação: O sistema é estruturado em classes (pessoa, secretaria, medico, paciente, Consulta), funções de gerenciamento (cadastrarPaciente), funções de menu (menuSecretaria) e uma função main que atua como orquestradora, demonstrando boa separação de responsabilidades.

    RNF-12: Extensibilidade via Polimorfismo

        Descrição: A arquitetura facilita a adição de novos tipos de usuários ou entidades no futuro.

        Implementação: O uso de herança (com a classe base pessoa) e de funções virtual permite que novos papéis (ex: Enfermeiro) possam ser adicionados herdando de pessoa com um impacto mínimo no resto do sistema, especialmente na persistência de dados, graças ao suporte a polimorfismo da biblioteca Cereal.

---

## 🛠️ Especificações do Trabalho

- Implementar a aplicação SCAM conforme as especificações estabelecidas.
- Especificar o Diagrama de Caso de Uso Geral do SCAM, representando as relações de generalização e especialização.
- Finalizar a especificação do Diagrama de Classes do SCAM.

---

## 📚 Definições

- **Generalização**: Quando uma classe compartilha atributos e métodos com outras classes, caracterizando uma relação de herança.
- **Especialização**: Quando uma classe derivada possui atributos ou métodos específicos que não são compartilhados com a classe base.

---

## 💻 Linguagem Utilizada

Este projeto foi desenvolvido utilizando a linguagem de programação **C++**, aplicando os conceitos de Programação Orientada a Objetos para a implementação das funcionalidades do sistema.

---

