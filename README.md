# 🏥 SCAM - Sistema de Controle e Agendamento Médico

**Disciplina:** Programação Orientada a Objetos  
**Curso:** Sistemas de Informação  
**Instituição:** Universidade Estadual de Goiás  
**Data:** 10 de Junho de 2025  

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
- [Bibliotecas e Ferramentas](#bibliotecas-e-ferramentas)

---

## 📖 Descrição do Projeto

O **SCAM** é um sistema desenvolvido com o objetivo de gerenciar o controle e agendamento de consultas médicas em uma clínica. O projeto foi elaborado para fins acadêmicos, aplicando os conceitos de Programação Orientada a Objetos.

---

## 🎯 Diagramas de Caso de Uso

### Secretária  
_(Insira aqui a imagem do Diagrama de Caso de Uso da Secretária)_

### Médico  
_(Insira aqui a imagem do Diagrama de Caso de Uso do Médico)_

### Paciente  
_(Insira aqui a imagem do Diagrama de Caso de Uso do Paciente)_

---

## 🧩 Diagrama de Classes

_(Insira aqui a imagem do Diagrama de Classes)_

---

## ✅ Requisitos do Sistema

### Requisitos Funcionais

- **[RQ-01]**: Cadastro de secretárias, médicos e pacientes (CPF, nome, e-mail, telefone, WhatsApp).
- **[RQ-02]**: Cadastro do número de matrícula, cargo e médicos assessorados.
- **[RQ-03]**: Apenas a secretária supervisora pode cadastrar outras secretárias.
- **[RQ-04]**: Cada secretária pode assessorar no máximo três médicos.
- **[RQ-05]**: Apenas a secretária supervisora pode consultar as secretárias.
- **[RQ-06]**: Secretária pode consultar/cadastrar apenas médicos que assessora.
- **[RQ-07]**: Secretária pode consultar/cadastrar apenas pacientes dos médicos que assessora.
- **[RQ-08]**: Secretária pode consultar/cadastrar apenas agenda dos médicos que assessora.
- **[RQ-09]**: Secretária pode consultar/cadastrar apenas agenda dos pacientes dos médicos que assessora.
- **[RQ-10]**: Inclusão, alteração e exclusão de entidades nas funcionalidades de cadastro.
- **[RQ-11]**: Sistema permite no máximo três tentativas de autenticação inválidas.

---

### Requisitos Não Funcionais

#### 1. Segurança

- **RNF-01: Controle de Acesso Baseado em Papel (RBAC)**  
  Permissões baseadas no cargo. Implementado via verificação do atributo `cargo`.

- **RNF-02: Autenticação Obrigatória**  
  Acesso somente com login e senha válidos.

- **RNF-03: Proteção Contra Ataque de Força Bruta**  
  Máximo de 3 tentativas de login.

- **RNF-04: Persistência de Dados Não Criptografada**  
  Dados armazenados em arquivos binários via biblioteca Cereal. Senhas em texto plano.

#### 2. Usabilidade

- **RNF-05: Interface de Linha de Comando (CLI)**  
  Interação por menus em texto.

- **RNF-06: Busca por Nome em Vez de Código**  
  Facilita buscas por nome. Função `encontrarCodigoPorNome`.

- **RNF-07: Navegação Guiada e Feedback ao Usuário**  
  Uso de `switch-case` e mensagens de resposta claras.

#### 3. Confiabilidade e Robustez

- **RNF-08: Persistência de Dados na Saída**  
  Dados salvos apenas ao sair do sistema.

- **RNF-09: Tratamento de Entrada Inválida**  
  Uso de `limparBufferEntrada` e controle de fluxo robusto.

#### 4. Portabilidade

- **RNF-10: Independência de Plataforma**  
  Compatível com Windows, macOS e Linux. Uso apenas de bibliotecas padrão e Cereal.

#### 5. Manutenibilidade

- **RNF-11: Modularidade e Coesão**  
  Código dividido em classes e funções bem definidas.

- **RNF-12: Extensibilidade via Polimorfismo**  
  Uso de herança e funções virtuais. Facilita inclusão de novos tipos de usuários.

---

## 🛠️ Especificações do Trabalho

- Implementar a aplicação SCAM conforme as especificações.
- Criar o Diagrama de Caso de Uso Geral com relações de generalização/especialização.
- Finalizar o Diagrama de Classes do SCAM.

---

## 📚 Definições

- **Generalização**: Classe compartilha atributos e métodos com outras (herança).
- **Especialização**: Classe derivada possui atributos/métodos específicos.

---

## 💻 Linguagem Utilizada

O projeto foi desenvolvido em **C++**, utilizando princípios de Programação Orientada a Objetos.

---

## 🔧 Bibliotecas e Ferramentas

- **Cereal C++ Serialization Library**  
  Utilizada para persistência de dados com suporte a polimorfismo.  
  [Créditos](https://uscilab.github.io/cereal/)
