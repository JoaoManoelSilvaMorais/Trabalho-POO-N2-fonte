#include <fstream>
#include <iostream>
#include <string>
#include <vector> // Adicionado
using namespace std;

class pessoa {
protected:
    string login;
    string senha;
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
    int CodigoSecretaria;
    short cargo; // 0 - secretária, 1 - supervisora
    int codigosMedicos[3]; // Cada secretária pode ter até 3 médicos

public:
    secretaria() {
        cargo = 0;
        for (int i = 0; i < 3; i++) codigosMedicos[i] = -1;

        std::ifstream registroCheck("RegistrosSecretarias.dat", std::ios::binary);
        if (registroCheck.good()) {
            registroCheck.close();
            cout << "arquivo existente\n" << endl;
            return;
        } else {
            registroCheck.close();
            this->setLogin("admin");
            this->setSenha("admin");
            this->setCodigo(0);
            this->setCargo(1);

            std::ofstream registroSaida("RegistrosSecretarias.dat", std::ios::binary);
            registroSaida.write(reinterpret_cast<char*>(this), sizeof(secretaria));
            registroSaida.close();
        }
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

int main() {
    secretaria sec;
    string nome_temp;

    cout << "Digite o nome da secretária: ";
    getline(cin, nome_temp);
    sec.setNome(nome_temp);

    cout << "Nome da secretária: " << sec.getNome() << endl;

    return 0;
}
