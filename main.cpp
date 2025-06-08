#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <limits>//Adicionado
using namespace std;

class pessoa {
protected:
    int codigo;
    int cpf[12];
    string nome;
    string email;
    string telefone;
    string whatsapp;

public:
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

class secretaria : public pessoa {
private:
    string login;
    string senha;
    short cargo; // 0 - secretária, 1 - supervisora
    int codigosMedicos[3]; // Cada secretária pode ter até 3 médicos

public:
    secretaria() {
        cargo = 0;
        for (int i = 0; i < 3; i++) codigosMedicos[i] = -1;
    }

    void setLogin(string loginin) { login = loginin; }
    void setSenha(string senhain) { senha = senhain; }
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
//Adicionado
const paciente* autenticarPaciente(const vector<paciente>& pacientes, const string& login, const string& senha) {
    for (const auto& p : pacientes) {
        if (p.getLogin() == login && p.getSenha() == senha) {
            cout << "Autenticação bem-sucedida para o paciente: " << p.getNome() << endl;
            return &p;
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
    getline(cin, nome_temp);
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

    string loginPaciente;
    string senhaPaciente;
    const paciente* pacienteLogado = nullptr;

   
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n--- Autenticação do Paciente ---\n";
    cout << "Login: ";
    getline(cin, loginPaciente);
    cout << "Senha: ";
    getline(cin, senhaPaciente);

    pacienteLogado = autenticarPaciente(pacientes_mock, loginPaciente, senhaPaciente);

    if (pacienteLogado != nullptr) {
        consultarAgendaPaciente(consultas_mock, *pacienteLogado);
    } else {
        cout << "Não foi possível autenticar o paciente. Tente novamente.\n";
    }
  

    return 0;
}

