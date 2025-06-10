/******************************************************************************
 *
 * SCAM - Sistema de Controle de Agendamento Médico
 *
 * Versão completa com controle de acesso baseado em função (RBAC),
 * limite de tentativas de login, persistência de dados e criação de
 * usuário administrador padrão.
 *
 * Código corrigido e revisado para evitar dangling pointers após realocação
 * de vetores.
 *
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <fstream>
#include <memory>
#include <cctype>

// --- Inclusões da Biblioteca Cereal para Serialização ---
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/binary.hpp>

// --- Declarações Forward das Classes ---
class pessoa;
class secretaria;
class medico;
class paciente;
class Consulta;

// --- Definição da Classe Base: pessoa ---
class pessoa {
protected:
    int codigo;
    std::string nome;
    std::string cpf;
    std::string email;
    std::string telefone;
    std::string whatsapp;
    std::string login;
    std::string senha;

public:
    // Construtor padrão
    pessoa() : codigo(0) {}
    // Construtor parametrizado
    pessoa(int cod, std::string n, std::string cpf_val, std::string mail, std::string tel, std::string whats, std::string l, std::string s)
        : codigo(cod), nome(n), cpf(cpf_val), email(mail), telefone(tel), whatsapp(whats), login(l), senha(s) {}
    // Destrutor virtual para garantir a destruição correta das classes derivadas
    virtual ~pessoa() = default;

    // --- Getters ---
    int getCodigo() const { return codigo; }
    std::string getNome() const { return nome; }
    std::string getLogin() const { return login; }
    std::string getSenha() const { return senha; }
    std::string getCpf() const { return cpf; }

    // --- Métodos Virtuais para Polimorfismo ---
    virtual void exibirInformacoes() const {
        std::cout << "----------------------------------------\n";
        std::cout << "Código: " << codigo << "\n";
        std::cout << "Nome: " << nome << "\n";
        std::cout << "CPF: " << cpf << "\n";
        std::cout << "Login: " << login << "\n";
        std::cout << "E-mail: " << email << "\n";
        std::cout << "Telefone: " << telefone << "\n";
        std::cout << "WhatsApp: " << whatsapp << "\n";
    }

    // Função para serialização com Cereal
    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(codigo), CEREAL_NVP(nome), CEREAL_NVP(cpf), CEREAL_NVP(email), 
                CEREAL_NVP(telefone), CEREAL_NVP(whatsapp), CEREAL_NVP(login), CEREAL_NVP(senha));
    }
};

// --- Definição da Classe Derivada: secretaria ---
class secretaria : public pessoa {
private:
    std::string cargo; // "Secretária" ou "Secretária-Supervisora"
    std::vector<int> medicosAssessorados;

public:
    secretaria() : pessoa(), cargo("Secretária") {}
    secretaria(int cod, std::string n, std::string cpf_val, std::string mail, std::string tel, std::string whats, std::string l, std::string s, std::string crg)
        : pessoa(cod, n, cpf_val, mail, tel, whats, l, s), cargo(crg) {}

    // Getters específicos
    std::string getCargo() const { return cargo; }
    const std::vector<int>& getMedicosAssessorados() const { return medicosAssessorados; }

    // [RQ-04] Regra de negócio: Adicionar médico, com limite de 3
    bool adicionarMedicoAssessorado(int codMedico) {
        if (medicosAssessorados.size() < 3) {
            medicosAssessorados.push_back(codMedico);
            return true;
        }
        std::cout << "ERRO: Esta secretária já assessora o número máximo de 3 médicos.\n";
        return false;
    }

    // Verifica se a secretária assessora um médico específico
    bool assessoraMedico(int codMedico) const {
        return std::find(medicosAssessorados.begin(), medicosAssessorados.end(), codMedico) != medicosAssessorados.end();
    }

    // Remove um médico da lista de assessorados
    void removerMedicoAssessorado(int codMedico) {
        auto it = std::remove(medicosAssessorados.begin(), medicosAssessorados.end(), codMedico);
        if (it != medicosAssessorados.end()) {
            medicosAssessorados.erase(it, medicosAssessorados.end());
        }
    }

    void exibirInformacoes() const override {
        pessoa::exibirInformacoes();
        std::cout << "Cargo: " << cargo << "\n";
        std::cout << "Médicos Assessorados (Códigos): ";
        if (medicosAssessorados.empty()) {
            std::cout << "Nenhum\n";
        } else {
            for (int cod : medicosAssessorados) {
                std::cout << cod << " ";
            }
            std::cout << "\n";
        }
    }

    template <class Archive>
    void serialize(Archive& archive) {
        archive(cereal::base_class<pessoa>(this), CEREAL_NVP(cargo), CEREAL_NVP(medicosAssessorados));
    }
};

// --- Definição da Classe Derivada: medico ---
class medico : public pessoa {
private:
    std::string especialidade;
    std::string crm;

public:
    medico() : pessoa() {}
    medico(int cod, std::string n, std::string cpf_val, std::string mail, std::string tel, std::string whats, std::string l, std::string s, std::string esp, std::string crm_val)
        : pessoa(cod, n, cpf_val, mail, tel, whats, l, s), especialidade(esp), crm(crm_val) {}

    void exibirInformacoes() const override {
        pessoa::exibirInformacoes();
        std::cout << "Especialidade: " << especialidade << "\n";
        std::cout << "CRM: " << crm << "\n";
    }

    template <class Archive>
    void serialize(Archive& archive) {
        archive(cereal::base_class<pessoa>(this), CEREAL_NVP(especialidade), CEREAL_NVP(crm));
    }
};

// --- Definição da Classe Derivada: paciente ---
class paciente : public pessoa {
private:
    int codigoMedicoResponsavel;

public:
    paciente() : pessoa(), codigoMedicoResponsavel(0) {}
    paciente(int cod, std::string n, std::string cpf_val, std::string mail, std::string tel, std::string whats, std::string l, std::string s, int codMed)
        : pessoa(cod, n, cpf_val, mail, tel, whats, l, s), codigoMedicoResponsavel(codMed) {}

    int getCodigoMedicoResponsavel() const { return codigoMedicoResponsavel; }

    void exibirInformacoes() const override {
        pessoa::exibirInformacoes();
        std::cout << "Código do Médico Responsável: " << codigoMedicoResponsavel << "\n";
    }

    template <class Archive>
    void serialize(Archive& archive) {
        archive(cereal::base_class<pessoa>(this), CEREAL_NVP(codigoMedicoResponsavel));
    }
};

// --- Definição da Classe Consulta ---
class Consulta {
private:
    int codigoConsulta;
    int codigoMedico;
    int codigoPaciente;
    std::string dataHora;
    std::string motivo;
    std::string status; // "Agendada", "Concluída", "Cancelada"

public:
    Consulta() : codigoConsulta(0), codigoMedico(0), codigoPaciente(0), status("Agendada") {}
    Consulta(int cc, int cm, int cp, std::string dh, std::string m, std::string st = "Agendada")
        : codigoConsulta(cc), codigoMedico(cm), codigoPaciente(cp), dataHora(dh), motivo(m), status(st) {}

    int getCodigoConsulta() const { return codigoConsulta; }
    int getCodigoMedico() const { return codigoMedico; }
    int getCodigoPaciente() const { return codigoPaciente; }
    
    void exibirInformacoes() const {
        std::cout << "----------------------------------------\n";
        std::cout << "Consulta Cód.: " << codigoConsulta << " | Status: " << status << "\n";
        std::cout << "Médico (Cód.): " << codigoMedico << " | Paciente (Cód.): " << codigoPaciente << "\n";
        std::cout << "Data e Hora: " << dataHora << "\n";
        std::cout << "Motivo: " << motivo << "\n";
    }

    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(codigoConsulta), CEREAL_NVP(codigoMedico), CEREAL_NVP(codigoPaciente),
                CEREAL_NVP(dataHora), CEREAL_NVP(motivo), CEREAL_NVP(status));
    }
};

// Registros Cereal para Polimorfismo (obrigatório para herança)
CEREAL_REGISTER_TYPE(secretaria);
CEREAL_REGISTER_TYPE(medico);
CEREAL_REGISTER_TYPE(paciente);
CEREAL_REGISTER_POLYMORPHIC_RELATION(pessoa, secretaria);
CEREAL_REGISTER_POLYMORPHIC_RELATION(pessoa, medico);
CEREAL_REGISTER_POLYMORPHIC_RELATION(pessoa, paciente);

// --- Funções Utilitárias e de Sistema ---

// Limpa o buffer de entrada para evitar erros com std::getline
void limparBufferEntrada() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Remove espaços em branco e caracteres não imprimíveis do final de uma string.
void trimEnd(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// Valida o formato do CPF (deve conter 11 dígitos numéricos)
bool validarFormatoCPF(const std::string& cpf) {
    if (cpf.length() != 11) {
        return false;
    }
    for (char const &c : cpf) {
        if (std::isdigit(c) == 0) {
            return false;
        }
    }
    return true;
}

// Verifica se o CPF já existe no sistema
bool cpfJaExiste(const std::string& cpf,
                 const std::vector<secretaria>& secretarias,
                 const std::vector<medico>& medicos,
                 const std::vector<paciente>& pacientes) {
    for (const auto& s : secretarias) {
        if (s.getCpf() == cpf) return true;
    }
    for (const auto& m : medicos) {
        if (m.getCpf() == cpf) return true;
    }
    for (const auto& p : pacientes) {
        if (p.getCpf() == cpf) return true;
    }
    return false;
}

// Lê uma opção de menu de forma robusta, garantindo que seja um número dentro do intervalo
int lerOpcaoMenu(const std::string& promptMensagem, int minOpcao, int maxOpcao) {
    int escolha;
    while (true) {
        std::cout << promptMensagem;
        std::cin >> escolha;
        if (std::cin.fail()) {
            std::cout << "Entrada inválida. Por favor, insira um número.\n";
            std::cin.clear(); // Limpa os flags de erro
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta a entrada inválida
        } else if (escolha < minOpcao || escolha > maxOpcao) {
            std::cout << "Opção inválida. Por favor, escolha entre " << minOpcao << " e " << maxOpcao << ".\n";
            limparBufferEntrada(); // Limpa o buffer mesmo se for um número fora do range
        } else {
            limparBufferEntrada(); // Limpa o newline restante após uma leitura bem-sucedida de número
            return escolha;
        }
    }
}


// Encontra o código de uma entidade (médico, paciente, etc.) pelo nome
template <typename T>
int encontrarCodigoPorNome(const std::string& prompt, const std::vector<T>& colecao) {
    std::string nomeBusca;
    if (colecao.empty()) {
        std::cout << "Não há registros para buscar.\n";
        return -1;
    }
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, nomeBusca);

        if (nomeBusca == "cancelar") {
            return -1;
        }

        for (const auto& item : colecao) {
            if (item.getNome() == nomeBusca) {
                return item.getCodigo();
            }
        }
        std::cout << "Nenhum registro encontrado com o nome '" << nomeBusca << "'. Tente novamente ou digite 'cancelar' para sair.\n";
    }
}


// [RQ-11] Autenticação com limite de 3 tentativas
template <typename T>
T* autenticar(std::vector<T>& usuarios, const std::string& tipoUsuario) {
    std::string login, senha;
    for (int i = 0; i < 3; ++i) {
        std::cout << "\n--- Autenticação de " << tipoUsuario << " (Tentativa " << i + 1 << "/3) ---\n";
        std::cout << "Login: ";
        std::getline(std::cin, login);
        std::cout << "Senha: ";
        std::getline(std::cin, senha);

        for (auto& user : usuarios) {
            if (user.getLogin() == login && user.getSenha() == senha) {
                std::cout << "Login bem-sucedido! Bem-vindo(a), " << user.getNome() << ".\n";
                return &user;
            }
        }
        std::cout << "Login ou senha inválidos. " << 2 - i << " tentativa(s) restante(s).\n";
    }
    std::cout << "Número máximo de tentativas de login atingido. Acesso negado.\n";
    return nullptr;
}

// Salva um container de dados (vector) em um arquivo binário
template <typename T>
void salvarEmArquivo(const T& dados, const std::string& nomeArquivo) {
    std::ofstream os(nomeArquivo, std::ios::binary);
    if (os) {
        cereal::BinaryOutputArchive archive(os);
        archive(dados);
    }
}

// Carrega um container de dados (vector) de um arquivo binário
template <typename T>
void carregarDeArquivo(T& dados, const std::string& nomeArquivo) {
    std::ifstream is(nomeArquivo, std::ios::binary);
    if (is) {
        cereal::BinaryInputArchive archive(is);
        archive(dados);
    }
}

// Gera um novo código único para qualquer entidade (secretaria, medico, paciente, consulta)
template <typename... VectorTypes>
int gerarNovoCodigo(const VectorTypes&... vetores) {
    int maxCodigo = 0;
    auto max_in_vector = [&](const auto& vec) {
        for (const auto& item : vec) {
            if constexpr (std::is_same_v<std::decay_t<decltype(item)>, Consulta>) {
                 if (item.getCodigoConsulta() > maxCodigo) maxCodigo = item.getCodigoConsulta();
            } else {
                 if (item.getCodigo() > maxCodigo) maxCodigo = item.getCodigo();
            }
        }
    };
    (max_in_vector(vetores), ...);
    return maxCodigo + 1;
}

// --- Funções de Gerenciamento (com regras de negócio) ---

// [RQ-03] Apenas secretárias supervisoras podem cadastrar outras secretárias
void cadastrarSecretaria(secretaria* secLogada, std::vector<secretaria>& secretarias, const std::vector<medico>& medicos, const std::vector<paciente>& pacientes, int& proximoCodigo) {
    if (secLogada->getCargo() != "Secretária-Supervisora") {
        std::cout << "ACESSO NEGADO: Apenas secretárias supervisoras podem cadastrar novas secretárias.\n";
        return;
    }
    
    std::cout << "\n--- Cadastro de Nova Secretária ---\n";
    std::string nome, cpf, email, tel, whats, login, senha, cargo;
    
    std::cout << "Nome completo: "; std::getline(std::cin, nome);

    bool cpfValido = false;
    do {
        std::cout << "CPF (11 números, sem pontos ou traços, ou digite 'cancelar' para interromper): ";
        std::getline(std::cin, cpf);
        if (cpf == "cancelar") {
            std::cout << "Cadastro de secretária interrompido.\n";
            return;
        }
        if (!validarFormatoCPF(cpf)) {
            std::cout << "Formato de CPF inválido. Deve conter 11 dígitos numéricos. Tente novamente.\n";
        } else if (cpfJaExiste(cpf, secretarias, medicos, pacientes)) {
            std::cout << "ERRO: CPF já cadastrado no sistema. Tente outro.\n";
        } else {
            cpfValido = true;
        }
    } while (!cpfValido);
    
    std::cout << "E-mail: "; std::getline(std::cin, email);
    std::cout << "Telefone: "; std::getline(std::cin, tel);
    std::cout << "WhatsApp: "; std::getline(std::cin, whats);
    std::cout << "Login: "; std::getline(std::cin, login);
    std::cout << "Senha: "; std::getline(std::cin, senha);
    
    std::cout << "Selecione o Cargo:\n";
    std::cout << "1. Secretária\n";
    std::cout << "2. Secretária-Supervisora\n";
    int cargoOpcao = lerOpcaoMenu("Escolha: ", 1, 2);

    if (cargoOpcao == 1) {
        cargo = "Secretária";
    } else {
        cargo = "Secretária-Supervisora";
    }

    secretarias.emplace_back(proximoCodigo, nome, cpf, email, tel, whats, login, senha, cargo);
    std::cout << "Secretária cadastrada com sucesso! Código: " << proximoCodigo << "\n";
    proximoCodigo++;
}

void cadastrarMedico(secretaria* secLogada, std::vector<medico>& medicos, const std::vector<secretaria>& secretarias, const std::vector<paciente>& pacientes, int& proximoCodigo) {
    std::cout << "\n--- Cadastro de Novo Médico ---\n";
    std::string nome, cpf, email, tel, whats, login, senha, especialidade, crm;
    
    std::cout << "Nome completo: "; std::getline(std::cin, nome);

    bool cpfValido = false;
    do {
        std::cout << "CPF (11 números, sem pontos ou traços, ou digite 'cancelar' para interromper): ";
        std::getline(std::cin, cpf);
        if (cpf == "cancelar") {
            std::cout << "Cadastro de médico interrompido.\n";
            return;
        }
        if (!validarFormatoCPF(cpf)) {
            std::cout << "Formato de CPF inválido. Deve conter 11 dígitos numéricos. Tente novamente.\n";
        } else if (cpfJaExiste(cpf, secretarias, medicos, pacientes)) {
            std::cout << "ERRO: CPF já cadastrado no sistema. Tente outro.\n";
        } else {
            cpfValido = true;
        }
    } while (!cpfValido);

    std::cout << "E-mail: "; std::getline(std::cin, email);
    std::cout << "Telefone: "; std::getline(std::cin, tel);
    std::cout << "WhatsApp: "; std::getline(std::cin, whats);
    std::cout << "Login: "; std::getline(std::cin, login);
    std::cout << "Senha: "; std::getline(std::cin, senha);
    std::cout << "Especialidade: "; std::getline(std::cin, especialidade);
    std::cout << "CRM: "; std::getline(std::cin, crm);

    medicos.emplace_back(proximoCodigo, nome, cpf, email, tel, whats, login, senha, especialidade, crm);
    std::cout << "Médico cadastrado com sucesso! Código: " << proximoCodigo << "\n";
    
    // Associa automaticamente à secretária que o cadastrou, se não for supervisora
    if (secLogada->getCargo() != "Secretária-Supervisora") {
        if (secLogada->adicionarMedicoAssessorado(proximoCodigo)) {
            std::cout << "Médico (Cód. " << proximoCodigo << ") associado automaticamente a você.\n";
        }
    }
    
    proximoCodigo++;
}

void cadastrarPaciente(secretaria* secLogada, std::vector<paciente>& pacientes, const std::vector<medico>& medicos, const std::vector<secretaria>& secretarias, int& proximoCodigo) {
    std::cout << "\n--- Cadastro de Novo Paciente ---\n";
    std::string nome, cpf, email, tel, whats, login, senha;
    
    int codMedicoResp = encontrarCodigoPorNome("Digite o NOME COMPLETO do médico responsável: ", medicos);
    if (codMedicoResp == -1) {
        std::cout << "Cadastro de paciente cancelado.\n";
        return;
    }

    // [RQ-07] Valida a permissão para o médico encontrado
    if (secLogada->getCargo() != "Secretária-Supervisora" && !secLogada->assessoraMedico(codMedicoResp)) {
        std::cout << "ACESSO NEGADO: Você não assessora este médico.\n";
        return; 
    }
    
    std::cout << "Médico (Cód. " << codMedicoResp << ") selecionado.\n";
    std::cout << "Nome completo do paciente: "; std::getline(std::cin, nome);

    bool cpfValido = false;
    do {
        std::cout << "CPF (11 números, sem pontos ou traços, ou digite 'cancelar' para interromper): ";
        std::getline(std::cin, cpf);
        if (cpf == "cancelar") {
            std::cout << "Cadastro de paciente interrompido.\n";
            return;
        }
        if (!validarFormatoCPF(cpf)) {
            std::cout << "Formato de CPF inválido. Deve conter 11 dígitos numéricos. Tente novamente.\n";
        } else if (cpfJaExiste(cpf, secretarias, medicos, pacientes)) {
            std::cout << "ERRO: CPF já cadastrado no sistema. Tente outro.\n";
        } else {
            cpfValido = true;
        }
    } while (!cpfValido);

    std::cout << "E-mail: "; std::getline(std::cin, email);
    std::cout << "Telefone: "; std::getline(std::cin, tel);
    std::cout << "WhatsApp: "; std::getline(std::cin, whats);
    std::cout << "Login: "; std::getline(std::cin, login);
    std::cout << "Senha: "; std::getline(std::cin, senha);
    
    pacientes.emplace_back(proximoCodigo, nome, cpf, email, tel, whats, login, senha, codMedicoResp);
    std::cout << "Paciente cadastrado com sucesso! Código: " << proximoCodigo << "\n";
    proximoCodigo++;
}

// Função para agendar consulta, solicitando todos os dados
void agendarConsulta(secretaria* secLogada, std::vector<Consulta>& consultas, const std::vector<medico>& medicos, const std::vector<paciente>& pacientes, int& proximoCodigo) {
    std::cout << "\n--- Agendamento de Nova Consulta ---\n";
    std::string dataHora, motivo;

    // 1. Seleciona o Médico
    int codMedico = encontrarCodigoPorNome("Nome do Médico: ", medicos);
    if (codMedico == -1) {
        std::cout << "Agendamento cancelado.\n";
        return;
    }
    // [RQ-08] Valida Permissão da Agenda
    if (secLogada->getCargo() != "Secretária-Supervisora" && !secLogada->assessoraMedico(codMedico)) {
        std::cout << "ACESSO NEGADO: Você não gerencia a agenda deste médico.\n";
        return;
    }
    
    // 2. Seleciona o Paciente
    int codPaciente = encontrarCodigoPorNome("Nome do Paciente: ", pacientes);
    if (codPaciente == -1) {
        std::cout << "Agendamento cancelado.\n";
        return;
    }

    // 3. Coleta informações da consulta
    std::cout << "Data e Hora da consulta (ex: DD/MM/AAAA HH:MM): ";
    std::getline(std::cin, dataHora);
    std::cout << "Motivo da consulta: ";
    std::getline(std::cin, motivo);

    // 4. Cria e armazena a consulta
    consultas.emplace_back(proximoCodigo, codMedico, codPaciente, dataHora, motivo);
    std::cout << "Consulta agendada com sucesso! Código: " << proximoCodigo << "\n";
    proximoCodigo++;
}


// Exclui um médico, com validações
void excluirMedico(secretaria* secLogada,
                   std::vector<medico>& medicos,
                   std::vector<paciente>& pacientes,
                   std::vector<Consulta>& consultas,
                   std::vector<secretaria>& secretarias) {
    std::cout << "\n--- Excluir Médico ---\n";

    if (secLogada->getCargo() != "Secretária-Supervisora") {
        std::cout << "ACESSO NEGADO: Apenas secretárias supervisoras podem excluir médicos.\n";
        return;
    }

    if (medicos.empty()) {
        std::cout << "Não há médicos cadastrados para excluir.\n";
        return;
    }

    int codMedicoExcluir = encontrarCodigoPorNome("Digite o NOME COMPLETO do médico a ser excluído (ou 'cancelar'): ", medicos);
    if (codMedicoExcluir == -1) {
        std::cout << "Exclusão de médico cancelada.\n";
        return;
    }

    std::string nomeMedicoExcluido = "Desconhecido";
    for(const auto& med : medicos) {
        if(med.getCodigo() == codMedicoExcluir) {
            nomeMedicoExcluido = med.getNome();
            break;
        }
    }

    // Validação: Verifica se o médico tem pacientes associados
    for (const auto& pac : pacientes) {
        if (pac.getCodigoMedicoResponsavel() == codMedicoExcluir) {
            std::cout << "ERRO: Não é possível excluir o médico '" << nomeMedicoExcluido << "' (Cód. " << codMedicoExcluir
                      << ") pois ele possui pacientes associados (ex: Paciente '" << pac.getNome() << "', Cód. " << pac.getCodigo() << ").\n";
            std::cout << "Reatribua ou remova os pacientes deste médico antes de tentar excluí-lo.\n";
            return;
        }
    }

    // Validação: Verifica se o médico tem consultas associadas
    for (const auto& con : consultas) {
        if (con.getCodigoMedico() == codMedicoExcluir) {
            std::cout << "ERRO: Não é possível excluir o médico '" << nomeMedicoExcluido << "' (Cód. " << codMedicoExcluir
                      << ") pois ele possui consultas associadas (ex: Consulta Cód. " << con.getCodigoConsulta() << ").\n";
            std::cout << "Cancele ou finalize as consultas deste médico antes de tentar excluí-lo.\n";
            return;
        }
    }

    // Remoção do médico
    auto it_med = std::remove_if(medicos.begin(), medicos.end(),
                                  [codMedicoExcluir](const medico& m) {
                                      return m.getCodigo() == codMedicoExcluir;
                                  });

    if (it_med != medicos.end()) {
        medicos.erase(it_med, medicos.end());
        std::cout << "Médico '" << nomeMedicoExcluido << "' (Cód. " << codMedicoExcluir << ") excluído com sucesso.\n";

        // Remove o médico da lista de assessorados de todas as secretárias
        for (auto& sec : secretarias) {
            sec.removerMedicoAssessorado(codMedicoExcluir);
        }
        std::cout << "Médico removido das listas de assessoria das secretárias, se aplicável.\n";

    } else {
        std::cout << "ERRO: Médico '" << nomeMedicoExcluido << "' (Cód. " << codMedicoExcluir << ") não foi encontrado para remoção, embora tenha sido selecionado.\n";
    }
}

// --- Funções de listagem com controle de acesso ---

// [RQ-05] Apenas supervisora pode consultar a lista de secretárias.
void listarSecretarias(const secretaria* secLogada, const std::vector<secretaria>& secretarias) {
    if (secLogada->getCargo() != "Secretária-Supervisora") {
        std::cout << "ACESSO NEGADO: Apenas secretárias supervisoras podem consultar a lista de secretárias.\n";
        return;
    }
    std::cout << "\n--- Lista de Todas as Secretárias ---\n";
    if (secretarias.empty()) {
        std::cout << "Nenhuma secretária cadastrada.\n";
    } else {
        for (const auto& s : secretarias) {
            s.exibirInformacoes();
        }
    }
}

// [RQ-06] Secretária consulta médicos que assessora, supervisora consulta todos
void listarMedicos(const secretaria* secLogada, const std::vector<medico>& medicos) {
    std::cout << "\n--- Lista de Médicos ---\n";
    bool encontrou = false;
    if (medicos.empty()){
        std::cout << "Nenhum médico cadastrado.\n";
        return;
    }
    for (const auto& m : medicos) {
        if (secLogada->getCargo() == "Secretária-Supervisora" || secLogada->assessoraMedico(m.getCodigo())) {
            m.exibirInformacoes();
            encontrou = true;
        }
    }
    if (!encontrou) {
        std::cout << "Nenhum médico encontrado sob sua assessoria.\n";
    }
}

// [RQ-07] Secretária consulta pacientes dos médicos que assessora
void listarPacientes(const secretaria* secLogada, const std::vector<paciente>& pacientes) {
     std::cout << "\n--- Lista de Pacientes ---\n";
    bool encontrou = false;
    if (pacientes.empty()){
        std::cout << "Nenhum paciente cadastrado.\n";
        return;
    }
    for (const auto& p : pacientes) {
        if (secLogada->getCargo() == "Secretária-Supervisora" || secLogada->assessoraMedico(p.getCodigoMedicoResponsavel())) {
            p.exibirInformacoes();
            encontrou = true;
        }
    }
    if (!encontrou) {
        std::cout << "Nenhum paciente encontrado sob sua assessoria.\n";
    }
}

// Consulta a agenda de um médico específico
void consultarAgendaMedica(const secretaria* secLogada, const std::vector<Consulta>& consultas, const std::vector<medico>& medicos) {
    int codMedico = encontrarCodigoPorNome("Digite o nome do médico para ver a agenda: ", medicos);
    if (codMedico == -1) return;

    // [RQ-08] Valida permissão
    if (secLogada->getCargo() != "Secretária-Supervisora" && !secLogada->assessoraMedico(codMedico)) {
        std::cout << "ACESSO NEGADO: Você não tem permissão para ver a agenda deste médico.\n";
        return;
    }
    
    std::cout << "\n--- Agenda do Médico (Cód. " << codMedico << ") ---\n";
    bool encontrada = false;
    for(const auto& c : consultas) {
        if (c.getCodigoMedico() == codMedico) {
            c.exibirInformacoes();
            encontrada = true;
        }
    }
    if (!encontrada) std::cout << "Nenhuma consulta encontrada para este médico.\n";
}

// [RQ-09] Secretária consulta agenda de pacientes dos médicos que assessora
void consultarAgendaPaciente(const secretaria* secLogada, const std::vector<Consulta>& consultas, const std::vector<paciente>& pacientes) {
    int codPaciente = encontrarCodigoPorNome("Digite o nome do paciente para ver a agenda: ", pacientes);
    if (codPaciente == -1) return;

    int codMedicoResponsavel = -1;
    for(const auto& p : pacientes) {
        if (p.getCodigo() == codPaciente) {
            codMedicoResponsavel = p.getCodigoMedicoResponsavel();
            break;
        }
    }

    if (codMedicoResponsavel == -1) {
        std::cout << "Paciente não encontrado.\n";
        return;
    }

    if (secLogada->getCargo() != "Secretária-Supervisora" && !secLogada->assessoraMedico(codMedicoResponsavel)) {
        std::cout << "ACESSO NEGADO: Você não tem permissão para ver a agenda deste paciente.\n";
        return;
    }
    
    std::cout << "\n--- Agenda do Paciente (Cód. " << codPaciente << ") ---\n";
    bool encontrada = false;
    for(const auto& c : consultas) {
        if (c.getCodigoPaciente() == codPaciente) {
            c.exibirInformacoes();
            encontrada = true;
        }
    }
    if (!encontrada) std::cout << "Nenhuma consulta encontrada para este paciente.\n";
}

// --- Sub-Menus de Gerenciamento ---

void menuGerenciarMedicos(secretaria* secLogada, std::vector<medico>& medicos, std::vector<paciente>& pacientes, std::vector<Consulta>& consultas, std::vector<secretaria>& secretarias, int& proximoCodigo) {
    int escolha;
    do {
        std::cout << "\n-- Gerenciar Médicos --\n";
        std::cout << "1. Cadastrar Novo Médico\n";
        std::cout << "2. Listar Médicos\n";
        std::cout << "3. Excluir Médico\n";
        std::cout << "0. Voltar\n";
        escolha = lerOpcaoMenu("Escolha: ", 0, 3);
        
        switch(escolha) {
            case 1: cadastrarMedico(secLogada, medicos, secretarias, pacientes, proximoCodigo); break;
            case 2: listarMedicos(secLogada, medicos); break;
            case 3: excluirMedico(secLogada, medicos, pacientes, consultas, secretarias); break;
            case 0: break;
        }
    } while (escolha != 0);
}

void menuGerenciarPacientes(secretaria* secLogada, std::vector<paciente>& pacientes, const std::vector<medico>& medicos, const std::vector<secretaria>& secretarias, int& proximoCodigo) {
    int escolha;
    do {
        std::cout << "\n-- Gerenciar Pacientes --\n";
        std::cout << "1. Cadastrar Novo Paciente\n";
        std::cout << "2. Listar Pacientes\n";
        std::cout << "0. Voltar\n";
        escolha = lerOpcaoMenu("Escolha: ", 0, 2);
        
        switch(escolha) {
            case 1: cadastrarPaciente(secLogada, pacientes, medicos, secretarias, proximoCodigo); break;
            case 2: listarPacientes(secLogada, pacientes); break;
            case 0: break;
        }
    } while (escolha != 0);
}

void menuGerenciarConsultas(secretaria* secLogada, std::vector<Consulta>& consultas, const std::vector<medico>& medicos, const std::vector<paciente>& pacientes, int& proximoCodigo) {
    int escolha;
    do {
        std::cout << "\n-- Gerenciar Consultas --\n";
        std::cout << "1. Agendar Nova Consulta\n";
        std::cout << "2. Listar Todas as Consultas (Permitidas)\n";
        std::cout << "0. Voltar\n";
        escolha = lerOpcaoMenu("Escolha: ", 0, 2);
        
        switch(escolha) {
            case 1: agendarConsulta(secLogada, consultas, medicos, pacientes, proximoCodigo); break;
            case 2: {
                 std::cout << "\n--- Lista de Consultas ---\n";
                 bool encontrou = false;
                 for (const auto& c : consultas) {
                     if (secLogada->getCargo() == "Secretária-Supervisora" || secLogada->assessoraMedico(c.getCodigoMedico())) {
                         c.exibirInformacoes();
                         encontrou = true;
                     }
                 }
                 if (!encontrou) std::cout << "Nenhuma consulta encontrada para as suas permissões.\n";
                break;
            }
            case 0: break;
        }
    } while (escolha != 0);
}

// --- Menus Principais de Usuário ---

void menuSecretaria(int codigoSecLogada, std::vector<secretaria>& secretarias, std::vector<medico>& medicos, std::vector<paciente>& pacientes, std::vector<Consulta>& consultas, int& proximoCodigo) {
    int escolha;
    do {
        // Busca o ponteiro da secretária logada a cada iteração para garantir que seja válido
        secretaria* secLogada = nullptr;
        for (auto& sec : secretarias) {
            if (sec.getCodigo() == codigoSecLogada) {
                secLogada = &sec;
                break;
            }
        }

        // Se a secretária não for encontrada (pode ter sido removida), força o logout.
        if (!secLogada) {
            std::cout << "Erro crítico: Usuário logado não foi encontrado. Fazendo logout forçado.\n";
            break;
        }

        std::cout << "\n=================================================";
        std::cout << "\n--- Menu Secretária: " << secLogada->getNome() << " (" << secLogada->getCargo() << ") ---";
        std::cout << "\n=================================================\n";
        std::cout << "1. Gerenciar Médicos\n";
        std::cout << "2. Gerenciar Pacientes\n";
        std::cout << "3. Gerenciar Consultas\n";
        std::cout << "4. Consultar Agenda de Médico\n";
        std::cout << "5. Consultar Agenda de Paciente\n";
        int maxOpcao = 5;
        if (secLogada->getCargo() == "Secretária-Supervisora") {
            std::cout << "--- Opções da Supervisora ---\n";
            std::cout << "6. Cadastrar Nova Secretária\n";
            std::cout << "7. Listar Todas as Secretárias\n";
            maxOpcao = 7;
        }
        std::cout << "-------------------------------------------------\n";
        std::cout << "0. Sair (Logout)\n";
        escolha = lerOpcaoMenu("Escolha: ", 0, maxOpcao);

        switch (escolha) {
            case 1: menuGerenciarMedicos(secLogada, medicos, pacientes, consultas, secretarias, proximoCodigo); break; 
            case 2: menuGerenciarPacientes(secLogada, pacientes, medicos, secretarias, proximoCodigo); break;
            case 3: menuGerenciarConsultas(secLogada, consultas, medicos, pacientes, proximoCodigo); break;
            case 4: consultarAgendaMedica(secLogada, consultas, medicos); break;
            case 5: consultarAgendaPaciente(secLogada, consultas, pacientes); break;
            case 6:
                if (secLogada->getCargo() == "Secretária-Supervisora") {
                    cadastrarSecretaria(secLogada, secretarias, medicos, pacientes, proximoCodigo);
                } else {
                    std::cout << "Opção inválida.\n";
                }
                break;
            case 7:
                 if (secLogada->getCargo() == "Secretária-Supervisora") {
                    listarSecretarias(secLogada, secretarias);
                 } else {
                     std::cout << "Opção inválida.\n";
                 }
                break;
            case 0: std::cout << "Fazendo logout...\n"; break;
            default: std::cout << "Opção inválida. Tente novamente.\n"; break;
        }
    } while (escolha != 0);
}

void menuMedico(int codigoMedLogado, std::vector<medico>& medicos, const std::vector<Consulta>& consultas) {
    medico* medLogado = nullptr;
    for (auto& med : medicos) {
        if (med.getCodigo() == codigoMedLogado) {
            medLogado = &med;
            break;
        }
    }

    if (!medLogado) {
        std::cout << "Erro crítico: Médico logado não foi encontrado.\n";
        return;
    }

    std::cout << "\n--- Menu Médico: " << medLogado->getNome() << " ---\n";
    std::cout << "--- Sua Agenda de Consultas ---\n";
    bool encontrou = false;
    for(const auto& c : consultas) {
        if (c.getCodigoMedico() == medLogado->getCodigo()) {
            c.exibirInformacoes();
            encontrou = true;
        }
    }
    if (!encontrou) std::cout << "Nenhuma consulta na sua agenda.\n";
    std::cout << "\nPressione Enter para sair...";
    std::cin.get();
}

void menuPaciente(int codigoPacLogado, std::vector<paciente>& pacientes, const std::vector<Consulta>& consultas) {
    paciente* pacLogado = nullptr;
    for (auto& pac : pacientes) {
        if (pac.getCodigo() == codigoPacLogado) {
            pacLogado = &pac;
            break;
        }
    }

    if (!pacLogado) {
        std::cout << "Erro crítico: Paciente logado não foi encontrado.\n";
        return;
    }

    std::cout << "\n--- Menu Paciente: " << pacLogado->getNome() << " ---\n";
    std::cout << "--- Suas Consultas Agendadas ---\n";
    bool encontrou = false;
    for (const auto& c : consultas) {
        if (c.getCodigoPaciente() == pacLogado->getCodigo()) {
            c.exibirInformacoes();
            encontrou = true;
        }
    }
    if (!encontrou) std::cout << "Nenhuma consulta agendada para você.\n";
    std::cout << "\nPressione Enter para sair...";
    std::cin.get();
}


// --- Função Principal ---
int main() {
    std::vector<secretaria> secretarias;
    std::vector<medico> medicos;
    std::vector<paciente> pacientes;
    std::vector<Consulta> consultas;
    
    // Carrega os dados dos arquivos binários
    carregarDeArquivo(secretarias, "secretarias.bin");
    carregarDeArquivo(medicos, "medicos.bin");
    carregarDeArquivo(pacientes, "pacientes.bin");
    carregarDeArquivo(consultas, "consultas.bin");

    // [Requisito Chave]: Cria usuário admin se o sistema estiver vazio
    if (secretarias.empty()) {
        std::cout << "AVISO: Nenhum usuário encontrado. Criando secretária supervisora padrão.\n";
        std::cout << "=> Login: admin, Senha: admin\n";
        secretarias.emplace_back(1, "Admin Supervisora", "00000000000", "admin@scam.com", "0", "0", "admin", "admin", "Secretária-Supervisora");
    }

    int proximoCodigo = gerarNovoCodigo(secretarias, medicos, pacientes, consultas);

    int tipoUsuario;
    do {
        std::cout << "\n\n### BEM-VINDO AO SISTEMA SCAM ###\n";
        std::cout << "1. Entrar como Secretária\n";
        std::cout << "2. Entrar como Médico\n";
        std::cout << "3. Entrar como Paciente\n";
        std::cout << "0. Sair do Programa\n";
        tipoUsuario = lerOpcaoMenu("Escolha uma opção: ", 0, 3);

        switch (tipoUsuario) {
            case 1: {
                secretaria* secAutenticada = autenticar(secretarias, "Secretária");
                if (secAutenticada) {
                    // Passa o CÓDIGO do usuário para o menu, não o ponteiro
                    menuSecretaria(secAutenticada->getCodigo(), secretarias, medicos, pacientes, consultas, proximoCodigo);
                }
                break;
            }
            case 2: {
                medico* medAutenticado = autenticar(medicos, "Médico");
                if (medAutenticado) {
                    // Passa o CÓDIGO do usuário para o menu, não o ponteiro
                    menuMedico(medAutenticado->getCodigo(), medicos, consultas);
                }
                break;
            }
            case 3: {
                paciente* pacAutenticado = autenticar(pacientes, "Paciente");
                if (pacAutenticado) {
                    // Passa o CÓDIGO do usuário para o menu, não o ponteiro
                    menuPaciente(pacAutenticado->getCodigo(), pacientes, consultas);
                }
                break;
            }
            case 0:
                std::cout << "Saindo... Salvando todos os dados.\n";
                break;
            default:
                std::cout << "Opção inválida. Tente novamente.\n";
                break;
        }
    } while (tipoUsuario != 0);

    // Salva todos os dados em arquivos antes de encerrar
    salvarEmArquivo(secretarias, "secretarias.bin");
    salvarEmArquivo(medicos, "medicos.bin");
    salvarEmArquivo(pacientes, "pacientes.bin");
    salvarEmArquivo(consultas, "consultas.bin");
    
    std::cout << "Programa finalizado.\n";

    return 0;
}
