#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <limits>//Adicionado

//Necessário instalar a biblioteca Cereal para serialização
//Procure um tutorial de instalação da biblioteca Cereal para o seu sistema operacional.
// Certifique-se de que a biblioteca Cereal está instalada e configurada corretamente no seu ambiente de desenvolvimento.
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/cereal.hpp>
using namespace std;

//CLASSES A FRENTE
//Classe pessoa é a classe base para todas as pessoas no sistema, como secretárias, médicos e pacientes.
// Ela contém informações comuns a todas as pessoas, como código, CPF, login, senha, nome, email, telefone e WhatsApp.
class pessoa {
protected:
    int codigo;
    int cpf[12];
    string login;
    string senha;
    string nome;
    string email;
    string telefone;
    string whatsapp;

public:
    // Serialização usando Cereal
    template <class Archive>
    void serialize(Archive& ar) {
        ar(codigo, cpf, login, senha, nome, email, telefone, whatsapp);
    }

    // Início das funções
    // Construtor padrão
    pessoa() {
        codigo = 0;
        for (int i = 0; i < 12; i++) {
            cpf[i] = 0;
        }
        nome = "";
        email = "";
        telefone = "";
        whatsapp = "";
    }

    void setLogin(string loginin) { login = loginin; }
    string getLogin() const { return login; }

    void setSenha(string senhain) { senha = senhain; }
    string getSenha() const { return senha; }

    void setCodigo(int codin) { codigo = codin; }
    int getCodigo() const { return codigo; }

    void setCpf(int cpfin[12]) {
        for (int i = 0; i < 12; i++) {
            cpf[i] = cpfin[i];
        }
    }
    const int* getCpf() const { return cpf; }

    void setNome(string nomein) { nome = nomein; }
    string getNome() const { return nome; }

    void setEmail(string emailin) { email = emailin; }
    string getEmail() const { return email; }

    void setTelefone(string telefonein) { telefone = telefonein; }
    string getTelefone() const { return telefone; }

    void setWhatsapp(string whatsappin) { whatsapp = whatsappin; }
    string getWhatsapp() const { return whatsapp; }
};

// Classe secretaria herda de pessoa e adiciona informações específicas para secretárias, como cargo e códigos de médicos assessorados.
// A classe secretaria também implementa a serialização usando Cereal.
class secretaria : public pessoa {
private:
    short cargo; // 0 - secretária, 1 - supervisora
    int codigosMedicos[3]; // Cada secretária pode ter até 3 médicos

public:
    // Serialização usando Cereal
    // A classe secretaria herda de pessoa, então usamos cereal::base_class<pessoa>
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<pessoa>(this), cargo, codigosMedicos);
    }

    // Início das funções
    // Construtor padrão
    secretaria() {
        cargo = 0;
        for (int i = 0; i < 3; i++) codigosMedicos[i] = -1;
    }

    void setCargo(short c) { cargo = c; }
    short getCargo() const { return cargo; }

    void setCodigoMedicoAssessorado(int index, int cod) {
        if (index >= 0 && index < 3) codigosMedicos[index] = cod;
    }

    int getCodigoMedicoAssessorado(int index) const {
        if (index >= 0 && index < 3) return codigosMedicos[index];
        return -1;
    }
};

class medico : public pessoa {
    
};

class paciente : public pessoa {
private:
    int codigoMedicoResponsavel;

public:
    // Serialização usando Cereal
    // A classe paciente herda de pessoa, então usamos cereal::base_class<pessoa>
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<pessoa>(this), codigoMedicoResponsavel);
    }

    //Início das funções
    void setCodigoMedicoResponsavel(int cod) { codigoMedicoResponsavel = cod; }
    int getCodigoMedicoResponsavel() const { return codigoMedicoResponsavel; }
};

class Consulta {
private:
    int codigoConsulta;
    string data;
    string horario;
    int codigoMedico;
    int codigoPaciente;

public:
    // Serialização usando Cereal
    // A classe Consulta não herda de pessoa, então não usamos cereal::base_class<pessoa>
    template <class Archive>
    void serialize(Archive& ar) {
        ar(codigoConsulta, data, horario, codigoMedico, codigoPaciente);
    }

    //Início das funções
    void setCodigoConsulta(int cod) { codigoConsulta = cod; }
    int getCodigoConsulta() const { return codigoConsulta; }

    void setData(string d) { data = d; }
    string getData() const { return data; }

    void setHorario(string h) { horario = h; }
    string getHorario() const { return horario; }

    void setCodigoMedico(int cod) { codigoMedico = cod; }
    int getCodigoMedico() const { return codigoMedico; }

    void setCodigoPaciente(int cod) { codigoPaciente = cod; }
    int getCodigoPaciente() const { return codigoPaciente; }

    void exibirConsulta() const {
        cout << "\nConsulta #" << codigoConsulta << endl;
        cout << "Data: " << data << ", Horário: " << horario << endl;
        cout << "Código Médico: " << codigoMedico << ", Código Paciente: " << codigoPaciente << endl;
        cout << "-----------------------------\n";
    }
};

// FUNÇÕES A FRENTE

//Funções para guardar vetores de médicos, pacientes e secretárias usando Cereal e sobrecarga de funções

// Salva vetores de pessoas em um arquivo binário com o nome "RegistrosPessoas.dat"
void salvarEmArquivo(const vector<pessoa>& pessoasP) {
    std::ofstream arquivo("RegistrosPessoas.dat", std::ios::binary | std::ios::trunc);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }
    cereal::BinaryOutputArchive archive(arquivo);
    archive(pessoasP);
    arquivo.close();
}

// Sobrecarga da função salvarEmArquivo para aceitar um vetor de secretárias
void salvarEmArquivo(const vector<secretaria>& secretariasP) {
    std::ofstream arquivo("RegistrosSecretarias.dat", std::ios::binary | std::ios::trunc);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }
    cereal::BinaryOutputArchive archive(arquivo);
    archive(secretariasP);
    arquivo.close();
}

// Sobrecarga da função salvarEmArquivo para aceitar um vetor de médicos
void salvarEmArquivo(const vector<medico>& medicosP) {
    std::ofstream arquivo("RegistrosMedicos.dat", std::ios::binary | std::ios::trunc);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }
    cereal::BinaryOutputArchive archive(arquivo);
    archive(medicosP);
    arquivo.close();
}

// Sobrecarga da função salvarEmArquivo para aceitar um vetor de pacientes
void salvarEmArquivo(const vector<paciente>& pacientesP) {
    std::ofstream arquivo("RegistrosPacientes.dat", std::ios::binary | std::ios::trunc);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }
    cereal::BinaryOutputArchive archive(arquivo);
    archive(pacientesP);
    arquivo.close();
}

//Sobrecarga da função salvarEmArquivo para aceitar um vetor de consultas
void salvarEmArquivo(const vector<Consulta>& consultasP) {
    std::ofstream arquivo("RegistrosConsultas.dat", std::ios::binary | std::ios::trunc);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }
    cereal::BinaryOutputArchive archive(arquivo);
    archive(consultasP);
    arquivo.close();
}

//Todas as funções de carregarArquivo são sobrecarregadas para aceitar diferentes tipos de vetores, e modificam o vetor passado como argumento.
// Função para carregar os vetores de pessoas
void carregarArquivo(vector<pessoa>& pessoasP) {
    std::ifstream arquivo("RegistrosPessoas.dat", std::ios::binary);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para leitura." << endl;
        return;
    }
    cereal::BinaryInputArchive archive(arquivo);
    archive(pessoasP);
    arquivo.close();
}

// Sobrecarga da função carregarArquivo para carregar secretárias
void carregarArquivo(vector<secretaria>& secretariasP) {
    std::ifstream arquivo("RegistrosSecretarias.dat", std::ios::binary);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para leitura." << endl;
        return;
    }
    cereal::BinaryInputArchive archive(arquivo);
    archive(secretariasP);
    arquivo.close();
}
// Sobrecarga da função carregarArquivo para carregar médicos
void carregarArquivo(vector<medico>& medicosP) {
    std::ifstream arquivo("RegistrosMedicos.dat", std::ios::binary);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para leitura." << endl;
        return;
    }
    cereal::BinaryInputArchive archive(arquivo);
    archive(medicosP);
    arquivo.close();
}

// Sobrecarga da função carregarArquivo para carregar pacientes
void carregarArquivo(vector<paciente>& pacientesP) {
    std::ifstream arquivo("RegistrosPacientes.dat", std::ios::binary);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para leitura." << endl;
        return;
    }
    cereal::BinaryInputArchive archive(arquivo);
    archive(pacientesP);
    arquivo.close();
}

// Sobrecarga da função carregarArquivo para carregar consultas
void carregarArquivo(vector<Consulta>& consultasP) {
    std::ifstream arquivo("RegistrosConsultas.dat", std::ios::binary);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para leitura." << endl;
        return;
    }
    cereal::BinaryInputArchive archive(arquivo);
    archive(consultasP);
    arquivo.close();
}

//Funções de verificação

bool autenticar(const vector<secretaria>& secretariasP, const string& login, const string& senha) {
    for (const auto& secretaria : secretariasP) {
        if (secretaria.getLogin() == login && secretaria.getSenha() == senha) {
            return true; // Retorna 0 se a autenticação for bem-sucedida
        }
    }
    return false; // Retorna -1 se não encontrar
}

bool autenticar(const vector<medico>& medicosP, const string& login, const string& senha) {
    for (const auto& medico : medicosP) {
        if (medico.getLogin() == login && medico.getSenha() == senha) {
            return true; // Retorna 0 se a autenticação for bem-sucedida
        }
    }
    return false; // Retorna -1 se não encontrar
}

bool autenticar(const vector<paciente>& pacientesP, const string& login, const string& senha) {
    for (const auto& paciente : pacientesP) {
        if (paciente.getLogin() == login && paciente.getSenha() == senha) {
            return true; // Retorna 0 se a autenticação for bem-sucedida
        }
    }
    return false; // Retorna -1 se não encontrar
}

//Função busca nome e deleta do vetor

void removerdoVetor(vector<secretaria>& pessoasP, const string& nome){
    for (int i = 0; i < pessoasP.size(); i++) {
        if (pessoasP[i].getNome() == nome) {
            pessoasP.erase(pessoasP.begin() + i);
            cout << "Secretária " << nome << " removida com sucesso." << endl;
            return;
        }
    }
}
void removerdoVetor(vector<medico>& pessoasP, const string& nome) {
    for (int i = 0; i < pessoasP.size(); i++) {
        if (pessoasP[i].getNome() == nome) {
            pessoasP.erase(pessoasP.begin() + i);
            cout << "Médico " << nome << " removido com sucesso." << endl;
            return;
        }
    }
}
void removerdoVetor(vector<paciente>& pessoasP, const string& nome) {
    for (int i = 0; i < pessoasP.size(); i++) {
        if (pessoasP[i].getNome() == nome) {
            pessoasP.erase(pessoasP.begin() + i);
            cout << "Paciente " << nome << " removido com sucesso." << endl;
            return;
        }
    }
}

void removerdoVetor(vector<Consulta>& consultasP, int codigoConsulta) {
    for (int i = 0; i < consultasP.size(); i++) {
        if (consultasP[i].getCodigoConsulta() == codigoConsulta) {
            consultasP.erase(consultasP.begin() + i);
            cout << "Consulta #" << codigoConsulta << " removida com sucesso." << endl;
            return;
        }
    }
    cout << "Consulta #" << codigoConsulta << " não encontrada." << endl;
}

// Função para buscar e editar informações de uma instânica de uma classe buscando o nome de uma pessoa

void editarInformacoes(vector<secretaria>& pessoas, const string& nome) {
    for (auto& pessoa : pessoas) {
        if (pessoa.getNome() == nome) {
            cout << "Edição de informações da secretária " << nome << endl;
            
            // Campos específicos de secretaria
            cout << "Novo cargo (0 - secretária, 1 - supervisora): ";
            short novoCargo;
            cin >> novoCargo;
            pessoa.setCargo(novoCargo);
            
            // Limpa o buffer antes de ler strings
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "Digite o novo login: ";
            string novoLogin;
            getline(cin, novoLogin);
            pessoa.setLogin(novoLogin);
            
            cout << "Digite a nova senha: ";
            string novaSenha;
            getline(cin, novaSenha);
            pessoa.setSenha(novaSenha);
            
            cout << "Digite o novo nome: ";
            string novoNome;
            getline(cin, novoNome);
            pessoa.setNome(novoNome);
            
            cout << "Digite o novo CPF (apenas números, 11 dígitos): ";
            string cpfInput;
            getline(cin, cpfInput);
            
            // Converte string do CPF para array de inteiros
            int novoCpf[11]; // Corrigido: array de 11 elementos, não 12
            if (cpfInput.length() == 11) {
                for (int i = 0; i < 11; i++) {
                    novoCpf[i] = cpfInput[i] - '0'; // Converte char para int
                }
                pessoa.setCpf(novoCpf);
            } else {
                cout << "CPF inválido! Mantendo CPF anterior." << endl;
            }
            
            cout << "Digite o novo email: ";
            string novoEmail;
            getline(cin, novoEmail);
            pessoa.setEmail(novoEmail);
            
            cout << "Digite o novo telefone: ";
            string novoTelefone;
            getline(cin, novoTelefone);
            pessoa.setTelefone(novoTelefone);
            
            cout << "Digite o novo WhatsApp: ";
            string novoWhatsapp;
            getline(cin, novoWhatsapp);
            pessoa.setWhatsapp(novoWhatsapp);
            
            cout << "Informações da secretária " << nome << " atualizadas com sucesso!" << endl;
            return;
        }
    }
    cout << "Secretária " << nome << " não encontrada." << endl;
}

void editarInformacoes(vector<medico>& pessoas, const string& nome) {
    for (auto& pessoa : pessoas) {
        if (pessoa.getNome() == nome) {
            cout << "\n=== Edição de informações do médico " << nome << " ===" << endl;
           
            // Limpa o buffer de entrada uma única vez no início
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "Digite o novo login: ";
            string novoLogin;
            getline(cin, novoLogin);
            pessoa.setLogin(novoLogin);
           
            cout << "Digite a nova senha: ";
            string novaSenha;
            getline(cin, novaSenha);
            pessoa.setSenha(novaSenha);
           
            cout << "Digite o novo nome: ";
            string novoNome;
            getline(cin, novoNome);
            pessoa.setNome(novoNome);
           
            cout << "Digite o novo CPF (apenas números, 11 dígitos): ";
            string cpfInput;
            getline(cin, cpfInput);
           
            // Valida e converte string do CPF para array de inteiros
            int novoCpf[11];
            bool cpfValido = false;
            
            if (cpfInput.length() == 11) {
                cpfValido = true;
                for (int i = 0; i < 11; i++) {
                    if (cpfInput[i] >= '0' && cpfInput[i] <= '9') {
                        novoCpf[i] = cpfInput[i] - '0';
                    } else {
                        cpfValido = false;
                        break;
                    }
                }
                
                if (cpfValido) {
                    pessoa.setCpf(novoCpf);
                    cout << "CPF atualizado com sucesso!" << endl;
                } else {
                    cout << "ERRO: CPF contém caracteres inválidos! Mantendo CPF anterior." << endl;
                }
            } else {
                cout << "ERRO: CPF deve ter exatamente 11 dígitos! Mantendo CPF anterior." << endl;
            }
           
            cout << "Digite o novo email: ";
            string novoEmail;
            getline(cin, novoEmail);
            pessoa.setEmail(novoEmail);
           
            cout << "Digite o novo telefone: ";
            string novoTelefone;
            getline(cin, novoTelefone);
            pessoa.setTelefone(novoTelefone);
           
            cout << "Digite o novo WhatsApp: ";
            string novoWhatsapp;
            getline(cin, novoWhatsapp);
            pessoa.setWhatsapp(novoWhatsapp);
           
            cout << "\n✓ Informações do médico " << novoNome << " atualizadas com sucesso!" << endl;
            cout << "================================================" << endl;
            return;
        }
    }
    cout << "❌ Médico \"" << nome << "\" não encontrado." << endl;
}

void editarInformacoes(vector<paciente>& pessoas, const string& nome) {
    for (auto& pessoa : pessoas) {
        if (pessoa.getNome() == nome) {
            cout << "Edição de informações do paciente " << nome << endl;
            
            // Campos específicos de paciente
            cout << "Digite o novo login: ";
            string novoLogin;
            cin.ignore();
            getline(cin, novoLogin);
            pessoa.setLogin(novoLogin);
            
            cout << "Digite a nova senha: ";
            string novaSenha;
            getline(cin, novaSenha);
            pessoa.setSenha(novaSenha);
            
            cout << "Digite o novo nome: ";
            string novoNome;
            getline(cin, novoNome);
            pessoa.setNome(novoNome);
            
            cout << "Digite o novo CPF (apenas números, 11 dígitos): ";
            string cpfInput;
            getline(cin, cpfInput);
            
            // Converte string do CPF para array de inteiros
            int novoCpf[11]; // Corrigido: array de 11 elementos, não 12
            if (cpfInput.length() == 11) {
                for (int i = 0; i < 11; i++) {
                    novoCpf[i] = cpfInput[i] - '0'; // Converte char para int
                }
                pessoa.setCpf(novoCpf);
            } else {
                cout << "CPF inválido! Mantendo CPF anterior." << endl;
            }
            
            cout << "Digite o novo email: ";
            string novoEmail;
            getline(cin, novoEmail);
            pessoa.setEmail(novoEmail);
            
            cout << "Digite o novo telefone: ";
            string novoTelefone;
            getline(cin, novoTelefone);
            pessoa.setTelefone(novoTelefone);
            
            cout << "Digite o novo WhatsApp: ";
            string novoWhatsapp;
            getline(cin, novoWhatsapp);
            pessoa.setWhatsapp(novoWhatsapp);
            
            cout << "Informações do paciente " << nome << " atualizadas com sucesso!" << endl;
            return;
        }
    }
    cout << "Paciente " << nome << " não encontrado." << endl;
}

// Function to edit consultation information
void editarInformacoes(vector<medico>& medicos, vector<paciente>& pacientes, vector<Consulta>& consultas, int codigoConsulta) {
    for (auto& consulta : consultas) {
        if (consulta.getCodigoConsulta() == codigoConsulta) {
            cout << "Edição de informações da consulta #" << codigoConsulta << endl;

            cout << "Digite a nova data (DD/MM/AAAA): ";
            string novaData;
            cin.ignore();
            getline(cin, novaData);
            consulta.setData(novaData);

            cout << "Digite o novo horário (HH:MM): ";
            string novoHorario;
            getline(cin, novoHorario);
            consulta.setHorario(novoHorario);

            cout << "Digite o novo código do médico: ";
            int novoCodigoMedico;
            cin >> novoCodigoMedico;
            consulta.setCodigoMedico(novoCodigoMedico);

            cout << "Digite o novo código do paciente: ";
            int novoCodigoPaciente;
            cin >> novoCodigoPaciente;
            consulta.setCodigoPaciente(novoCodigoPaciente);

            cout << "Informações da consulta #" << codigoConsulta << " atualizadas com sucesso!" << endl;
            return;
        }
    }
    cout << "Consulta #" << codigoConsulta << " não encontrada." << endl;
}
            


//Função para incluir uma nova instância de secretaria, médico ou paciente no vetor correspondente alterando conformemente o código de cada um
void incluirNoVetor(vector<secretaria>& secretariasP, secretaria& s) {
    s.setCodigo(secretariasP.size() + 1); // Define o código como o tamanho atual do vetor + 1
    secretariasP.push_back(s);
}
void incluirNoVetor(vector<medico>& medicosP, medico& m) {
    m.setCodigo(medicosP.size() + 1); // Define o código como o tamanho atual do vetor + 1
    medicosP.push_back(m);
}
void incluirNoVetor(vector<paciente>& pacientesP, paciente& p) {
    p.setCodigo(pacientesP.size() + 1); // Define o código como o tamanho atual do vetor + 1
    pacientesP.push_back(p);
}

//Função para listar as secretárias, médicos e pacientes cadastrados
void listarPessoas(const vector<secretaria>& secretariasP) {
    cout << "\n--- Lista de Secretárias ---" << endl;
    if (secretariasP.empty()) {
        cout << "Nenhuma secretária cadastrada." << endl;
        return;
    }

    for (const auto& s : secretariasP) {
        // Formata o CPF para exibição
        string cpfStr;
        const int* cpfArray = s.getCpf();
        for (int i = 0; i < 11; i++) { // CPF tem 11 dígitos
            cpfStr += to_string(cpfArray[i]);
            if (i == 2 || i == 5) cpfStr += "."; // Formatação do CPF
            if (i == 8) cpfStr += "-";
        }

        cout << "Código: " << s.getCodigo() 
             << " | Nome: " << s.getNome()
             << " | Cargo: " << (s.getCargo() == 1 ? "Supervisora" : "Normal")
             << "\nCPF: " << cpfStr
             << "\nEmail: " << s.getEmail() 
             << " | Telefone: " << s.getTelefone() 
             << " | WhatsApp: " << s.getWhatsapp() 
             << "\n---------------------------------" << endl;
    }
}
void listarPessoas(const vector<medico>& medicosP) {
    cout << "\n--- Lista de Médicos ---" << endl;
    
    if (medicosP.empty()) {
        cout << "Nenhum médico cadastrado." << endl;
        return;
    }
    
    for (const auto& m : medicosP) {
        // Formata o CPF para exibição com validação
        string cpfStr = "";
        const int* cpfArray = m.getCpf();
        
        if (cpfArray != nullptr) {
            bool cpfValido = true;
            for (int i = 0; i < 11; i++) {
                if (cpfArray[i] < 0 || cpfArray[i] > 9) {
                    cpfValido = false;
                    break;
                }
                cpfStr += to_string(cpfArray[i]);
                if (i == 2 || i == 5) cpfStr += ".";
                if (i == 8) cpfStr += "-";
            }
            if (!cpfValido) cpfStr = "CPF inválido";
        } else {
            cpfStr = "CPF não informado";
        }
        
        cout << "Código: " << m.getCodigo()
             << " | Nome: " << m.getNome()
             << "\nCPF: " << cpfStr
             << "\nEmail: " << m.getEmail()
             << " | Telefone: " << m.getTelefone()
             << " | WhatsApp: " << m.getWhatsapp()
             << "\n---------------------------------" << endl;
    }
    
    cout << "Total: " << medicosP.size() << " médico(s) cadastrado(s)." << endl;
}
void listarPessoas(const vector<paciente>& pacientesP) {
    cout << "\n--- Lista de Pacientes ---" << endl;
    if (pacientesP.empty()) {
        cout << "Nenhum paciente cadastrado." << endl;
        return;
    }

    for (const auto& p : pacientesP) {
        // Formata o CPF para exibição
        string cpfStr;
        const int* cpfArray = p.getCpf();
        for (int i = 0; i < 11; i++) { // CPF tem 11 dígitos
            cpfStr += to_string(cpfArray[i]);
            if (i == 2 || i == 5) cpfStr += "."; // Formatação do CPF
            if (i == 8) cpfStr += "-";
        }

        cout << "Código: " << p.getCodigo() 
             << " | Nome: " << p.getNome()
             << " | Médico Responsável: " << p.getCodigoMedicoResponsavel()
             << "\nCPF: " << cpfStr
             << "\nEmail: " << p.getEmail() 
             << " | Telefone: " << p.getTelefone() 
             << " | WhatsApp: " << p.getWhatsapp() 
             << "\n---------------------------------" << endl;
    }
}

void menuSecretariaChefe(vector<secretaria>& secretarias, vector<medico>& medicos, 
                        vector<paciente>& pacientes, vector<Consulta>& consultas, 
                        const secretaria* secretariaLogada) {
    int opcao;
    do {
        cout << "\n=== MENU SECRETÁRIA CHEFE ===" << endl;
        cout << "1. Cadastrar Secretária" << endl;
        cout << "2. Cadastrar Médico" << endl;
        cout << "3. Cadastrar Paciente" << endl;
        cout << "4. Agendar Consulta" << endl;
        cout << "5. Editar Secretária" << endl;
        cout << "6. Editar Médico" << endl;
        cout << "7. Editar Paciente" << endl;
        cout << "8. Editar Consulta" << endl;
        cout << "9. Remover Secretária" << endl;
        cout << "10. Remover Médico" << endl;
        cout << "11. Remover Paciente" << endl;
        cout << "12. Remover Consulta" << endl;
        cout << "13. Consultar Secretárias" << endl;
        cout << "14. Consultar Médicos" << endl;
        cout << "15. Consultar Pacientes" << endl;
        cout << "16. Consultar Agenda Médica" << endl;
        cout << "17. Consultar Agenda de Paciente" << endl;
        cout << "0. Sair" << endl;
        cout << "Escolha uma opção: ";
        cin >> opcao;

        switch(opcao) {
            case 1: { // Cadastrar Secretária
                secretaria nova;
                string nome, login, senha;
                short cargo;
                
                cout << "\n--- Cadastro de Secretária ---" << endl;
                cout << "Nome: ";
                cin.ignore();
                getline(cin, nome);
                nova.setNome(nome);
                
                cout << "Login: ";
                getline(cin, login);
                nova.setLogin(login);
                
                cout << "Senha: ";
                getline(cin, senha);
                nova.setSenha(senha);
                
                cout << "Cargo (0 - Normal, 1 - Supervisora): ";
                cin >> cargo;
                nova.setCargo(cargo);
                
                // Configura CPF padrão (pode ser implementado corretamente depois)
                int cpf[12] = {0};
                nova.setCpf(cpf);
                
                incluirNoVetor(secretarias, nova);
                salvarEmArquivo(secretarias);
                cout << "Secretária cadastrada com sucesso!" << endl;
                break;
            }
            
            case 2: { // Cadastrar Médico
                medico novo;
                string nome, login, senha;
                
                cout << "\n--- Cadastro de Médico ---" << endl;
                cout << "Nome: ";
                cin.ignore();
                getline(cin, nome);
                novo.setNome(nome);
                
                cout << "Login: ";
                getline(cin, login);
                novo.setLogin(login);
                
                cout << "Senha: ";
                getline(cin, senha);
                novo.setSenha(senha);
                
                // Configura CPF padrão
                int cpf[12] = {0};
                novo.setCpf(cpf);
                
                incluirNoVetor(medicos, novo);
                salvarEmArquivo(medicos);
                cout << "Médico cadastrado com sucesso!" << endl;
                break;
            }
            
            case 3: { // Cadastrar Paciente
                paciente novo;
                string nome, login, senha;
                int codMedico;
                
                cout << "\n--- Cadastro de Paciente ---" << endl;
                cout << "Nome: ";
                cin.ignore();
                getline(cin, nome);
                novo.setNome(nome);
                
                cout << "Login: ";
                getline(cin, login);
                novo.setLogin(login);
                
                cout << "Senha: ";
                getline(cin, senha);
                novo.setSenha(senha);
                
                cout << "Código do Médico Responsável: ";
                cin >> codMedico;
                novo.setCodigoMedicoResponsavel(codMedico);
                
                // Configura CPF padrão
                int cpf[12] = {0};
                novo.setCpf(cpf);
                
                incluirNoVetor(pacientes, novo);
                salvarEmArquivo(pacientes);
                cout << "Paciente cadastrado com sucesso!" << endl;
                break;
            }
            
            case 4: { // Agendar Consulta
                Consulta nova;
                string data, horario;
                int codMedico, codPaciente;
                
                cout << "\n--- Agendamento de Consulta ---" << endl;
                cout << "Data (DD/MM/AAAA): ";
                cin >> data;
                nova.setData(data);
                
                cout << "Horário (HH:MM): ";
                cin >> horario;
                nova.setHorario(horario);
                
                cout << "Código do Médico: ";
                cin >> codMedico;
                nova.setCodigoMedico(codMedico);
                
                cout << "Código do Paciente: ";
                cin >> codPaciente;
                nova.setCodigoPaciente(codPaciente);
                
                nova.setCodigoConsulta(consultas.size() + 1);
                consultas.push_back(nova);
                salvarEmArquivo(consultas);
                cout << "Consulta agendada com sucesso! Código: " << nova.getCodigoConsulta() << endl;
                break;
            }
            
            case 5: { // Editar Secretária
                string nome;
                cout << "\n--- Edição de Secretária ---" << endl;
                cout << "Nome da secretária a editar: ";
                cin.ignore();
                getline(cin, nome);
                editarInformacoes(secretarias, nome);
                salvarEmArquivo(secretarias);
                break;
            }
            
            case 6: { // Editar Médico
                string nome;
                cout << "\n--- Edição de Médico ---" << endl;
                cout << "Nome do médico a editar: ";
                cin.ignore();
                getline(cin, nome);
                editarInformacoes(medicos, nome);
                salvarEmArquivo(medicos);
                break;
            }
            
            case 7: { // Editar Paciente
                string nome;
                cout << "\n--- Edição de Paciente ---" << endl;
                cout << "Nome do paciente a editar: ";
                cin.ignore();
                getline(cin, nome);
                editarInformacoes(pacientes, nome);
                salvarEmArquivo(pacientes);
                break;
            }
            
            case 8: { // Editar Consulta
                int codigo;
                cout << "\n--- Edição de Consulta ---" << endl;
                cout << "Código da consulta a editar: ";
                cin >> codigo;
                editarInformacoes(medicos, pacientes, consultas, codigo);
                salvarEmArquivo(consultas);
                break;
            }
            
            case 9: { // Remover Secretária
                string nome;
                cout << "\n--- Remoção de Secretária ---" << endl;
                cout << "Nome da secretária a remover: ";
                cin.ignore();
                getline(cin, nome);
                removerdoVetor(secretarias, nome);
                salvarEmArquivo(secretarias);
                break;
            }
            
            case 10: { // Remover Médico
                string nome;
                cout << "\n--- Remoção de Médico ---" << endl;
                cout << "Nome do médico a remover: ";
                cin.ignore();
                getline(cin, nome);
                removerdoVetor(medicos, nome);
                salvarEmArquivo(medicos);
                break;
            }
            
            case 11: { // Remover Paciente
                string nome;
                cout << "\n--- Remoção de Paciente ---" << endl;
                cout << "Nome do paciente a remover: ";
                cin.ignore();
                getline(cin, nome);
                removerdoVetor(pacientes, nome);
                salvarEmArquivo(pacientes);
                break;
            }
            
            case 12: { // Remover Consulta
                int codigo;
                cout << "\n--- Remoção de Consulta ---" << endl;
                cout << "Código da consulta a remover: ";
                cin >> codigo;
                removerdoVetor(consultas, codigo);
                salvarEmArquivo(consultas);
                break;
            }
            
            case 13: { // Consultar Secretárias
                listarPessoas(secretarias);
                break;
            }
            
            case 14: { // Consultar Médicos
                cout << "\n--- Lista de Médicos ---" << endl;
                for (const auto& m : medicos) {
                    cout << "Código: " << m.getCodigo() << " | Nome: " << m.getNome() << endl;
                }
                break;
            }
            
            case 15: { // Consultar Pacientes
                cout << "\n--- Lista de Pacientes ---" << endl;
                for (const auto& p : pacientes) {
                    cout << "Código: " << p.getCodigo() << " | Nome: " << p.getNome() 
                         << " | Médico Responsável: " << p.getCodigoMedicoResponsavel() << endl;
                }
                break;
            }
            
            case 16: { // Consultar Agenda Médica
                int codMedico;
                cout << "\n--- Consulta de Agenda Médica ---" << endl;
                cout << "Código do médico: ";
                cin >> codMedico;
                
                cout << "\n--- Consultas do Médico ---" << endl;
                for (const auto& c : consultas) {
                    if (c.getCodigoMedico() == codMedico) {
                        c.exibirConsulta();
                    }
                }
                break;
            }
            
            case 17: { // Consultar Agenda de Paciente
                int codPaciente;
                cout << "\n--- Consulta de Agenda do Paciente ---" << endl;
                cout << "Código do paciente: ";
                cin >> codPaciente;
                
                cout << "\n--- Consultas do Paciente ---" << endl;
                for (const auto& c : consultas) {
                    if (c.getCodigoPaciente() == codPaciente) {
                        c.exibirConsulta();
                    }
                }
                break;
            }
            
            case 0:
                cout << "Saindo do menu da secretária chefe..." << endl;
                break;
                
            default:
                cout << "Opção inválida! Tente novamente." << endl;
        }
    } while (opcao != 0);
}

int main() {
    vector<secretaria> secretarias;
    vector<medico> medicos;
    vector<paciente> pacientes;
    vector<Consulta> consultas;

    // Carrega dados dos arquivos
    carregarArquivo(secretarias);
    carregarArquivo(medicos);
    carregarArquivo(pacientes);
    carregarArquivo(consultas);

    // Cria secretária chefe se não existir
    if (secretarias.empty()) {
        secretaria chefe;
        chefe.setNome("Chefe");
        chefe.setLogin("admin");
        chefe.setSenha("admin123");
        chefe.setCargo(1); // 1 - supervisora
        incluirNoVetor(secretarias, chefe);
        salvarEmArquivo(secretarias);
    }

    // Autenticação
    string login, senha;
    cout << "=== LOGIN ===" << endl;
    cout << "Login: ";
    cin >> login;
    cout << "Senha: ";
    cin >> senha;

    // Verifica se é a secretária chefe
    bool autenticado = false;
    const secretaria* secretariaLogada = nullptr;
    
    for (const auto& s : secretarias) {
        if (s.getLogin() == login && s.getSenha() == senha && s.getCargo() == 1) {
            autenticado = true;
            secretariaLogada = &s;
            break;
        }
    }

    if (autenticado) {
        cout << "\nBem-vinda, " << secretariaLogada->getNome() << "!" << endl;
        menuSecretariaChefe(secretarias, medicos, pacientes, consultas, secretariaLogada);
    } else {
        cout << "Acesso negado. Apenas secretárias supervisoras podem acessar este menu." << endl;
    }

    return 0;
}