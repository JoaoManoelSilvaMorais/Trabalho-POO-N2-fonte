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
    int* getCpf() { return cpf; }

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
    // Implementação futura
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

// Agora usamos os nomes corretos das classes (medico, paciente, secretaria)
void consultarMedicos(const vector<medico>& medicos, const secretaria& s) {
    cout << "\n=== Médicos da Secretária: " << s.getNome() << " ===\n";
    for (const medico& m : medicos) {
        for (int i = 0; i < 3; i++) {
            if (s.getCodigoMedicoAssessorado(i) == m.getCodigo()) {
                cout << "Código: " << m.getCodigo() << " | Nome: " << m.getNome() << " | Email: " << m.getEmail() << endl;
            }
        }
    }
}

void consultarPacientes(const vector<paciente>& pacientes, const vector<medico>& medicos, const secretaria& s) {
    cout << "\n=== Pacientes dos Médicos assessorados por " << s.getNome() << " ===\n";
    for (const paciente& p : pacientes) {
        for (const medico& m : medicos) {
            for (int i = 0; i < 3; i++) {
                if (s.getCodigoMedicoAssessorado(i) == m.getCodigo() &&
                    p.getCodigoMedicoResponsavel() == m.getCodigo()) {
                    cout << "Código: " << p.getCodigo() << " | Nome: " << p.getNome() << " | Telefone: " << p.getTelefone() << endl;
                }
            }
        }
    }
}

void consultarSecretarias(const vector<secretaria>& secretarias, const secretaria& solicitante) {
    if (solicitante.getCargo() != 1) {
        cout << "Apenas secretárias supervisoras podem consultar outras secretárias.\n";
        return;
    }

    cout << "\n=== Lista de Secretárias ===\n";
    for (const secretaria& s : secretarias) {
        cout << "Código: " << s.getCodigo() << " | Nome: " << s.getNome() << " | Cargo: "
             << (s.getCargo() == 1 ? "Supervisora" : "Comum") << endl;
    }
}

void listarConsultas(const vector<Consulta>& consultas, const secretaria& s) {
    cout << "\n=== Consultas dos médicos assessorados por " << s.getNome() << " ===\n";
    for (const Consulta& c : consultas) {
        for (int i = 0; i < 3; i++) {
            if (s.getCodigoMedicoAssessorado(i) == c.getCodigoMedico()) {
                c.exibirConsulta();
            }
        }
    }
}
//A função autenticarLoginSenha e suas sobrecargas verifica se o login e senha fornecidos correspondem a algum registro de paciente, secretária ou médico.
const paciente* autenticarLoginSenha(const vector<paciente>& pacientes, const string& login, const string& senha) {
    for (const auto& p : pacientes) {
        if (p.getLogin() == login && p.getSenha() == senha) {
            cout << "Autenticação bem-sucedida para o paciente: " << p.getNome() << endl;
            return &p;
        }
    }
    cout << "Login ou senha incorretos.\n";
    return nullptr;
}

// Sobrecarga da função autenticarLoginSenha para secretárias
const secretaria* autenticarLoginSenha(const vector<secretaria>& secretarias, const string& login, const string& senha) {
    for (const auto& s : secretarias) {
        if (s.getLogin() == login && s.getSenha() == senha) {
            cout << "Autenticação bem-sucedida para a secretária: " << s.getNome() << endl;
            return &s;
        }
    }
    cout << "Login ou senha incorretos.\n";
    return nullptr;
}
// Sobrecarga da função autenticarLoginSenha para médicos
const medico* autenticarLoginSenha(const vector<medico>& medicos, const string& login, const string& senha) {
    for (const auto& m : medicos) {
        if (m.getLogin() == login && m.getSenha() == senha) {
            cout << "Autenticação bem-sucedida para o médico: " << m.getNome() << endl;
            return &m;
        }
    }
    cout << "Login ou senha incorretos.\n";
    return nullptr;
}

// Função para consultar a agenda de um paciente 
void consultarAgendaPaciente(const vector<Consulta>& consultas, const paciente& p) {
    cout << "\n=== Agenda do Paciente: " << p.getNome() << " ===\n";
    bool encontrouConsulta = false;
    for (const auto& c : consultas) {
        if (c.getCodigoPaciente() == p.getCodigo()) {
            c.exibirConsulta();
            encontrouConsulta = true;
        }
    }
    if (!encontrouConsulta) {
        cout << "Nenhuma consulta encontrada para este paciente.\n";
    }
}


int main() {
    secretaria sec;
    string nome_temp;

    cout << "Digite o nome da secretária: ";
    //getline(cin, nome_temp);
    cin >> nome_temp;
    sec.setNome(nome_temp);

    cout << "Nome da secretária: " << sec.getNome() << endl;

//Adicionado
    vector<paciente> pacientes_mock;
    paciente p1;
    p1.setCodigo(101);
    p1.setNome("Maria Silva");
    p1.setLogin("maria.s");
    p1.setSenha("123");
    p1.setCodigoMedicoResponsavel(1);
    pacientes_mock.push_back(p1);

    paciente p2;
    p2.setCodigo(102);
    p2.setNome("Joao Souza");
    p2.setLogin("joao.s");
    p2.setSenha("abc");
    p2.setCodigoMedicoResponsavel(2);
    pacientes_mock.push_back(p2);

    salvarEmArquivo(pacientes_mock);

    vector<Consulta> consultas_mock;
    Consulta c1;
    c1.setCodigoConsulta(1);
    c1.setData("01/07/2025");
    c1.setHorario("10:00");
    c1.setCodigoMedico(1);
    c1.setCodigoPaciente(101);
    consultas_mock.push_back(c1);

    Consulta c2;
    c2.setCodigoConsulta(2);
    c2.setData("02/07/2025");
    c2.setHorario("14:30");
    c2.setCodigoMedico(2);
    c2.setCodigoPaciente(102);
    consultas_mock.push_back(c2);

    Consulta c3;
    c3.setCodigoConsulta(3);
    c3.setData("03/07/2025");
    c3.setHorario("09:00");
    c3.setCodigoMedico(1);
    c3.setCodigoPaciente(101);
    consultas_mock.push_back(c3);

    salvarEmArquivo(consultas_mock);

    consultas_mock.clear();
    pacientes_mock.clear();
    carregarArquivo(consultas_mock);
    carregarArquivo(pacientes_mock);

    string loginPaciente;
    string senhaPaciente;
    const paciente* pacienteLogado = nullptr;

   
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n--- Autenticação do Paciente ---\n";
    cout << "Login: ";
    getline(cin, loginPaciente);
    cout << "Senha: ";
    getline(cin, senhaPaciente);

    pacienteLogado = autenticarLoginSenha(pacientes_mock, loginPaciente, senhaPaciente);

    if (pacienteLogado != nullptr) {
        consultarAgendaPaciente(consultas_mock, *pacienteLogado);
    } else {
        cout << "Não foi possível autenticar o paciente. Tente novamente.\n";
    }
  

    return 0;
}

